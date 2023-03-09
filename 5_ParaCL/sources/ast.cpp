#include <cmath>
#include <string>
#include <limits>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "ast.hpp"

namespace {

void cls() {
    std::cout << "Incorrect input" << std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

template <typename T>
T get() {
    T val;
    std::cin >> val;

    while (!(std::cin && (std::isspace(std::cin.peek()) || std::cin.eof()))) {
        cls();
        std::cin >> val;
    }
    return val;
}

} //namespace

namespace ast {

void Dump::dump(const node_t *node, std::ofstream &file, int &num) const {
    int curr = num;

    file << "\tnode" << num << " [shape = \"record\", label = \"" << msg << "\"];\n";
    num++;

    for (auto child : node->children)
        if (child) child->graph_node(file, num);

    if (!curr) num = 0;
}

void Dump::connect(const node_t *node, std::ofstream &file, int &num) const {
    int curr = num;

    for (auto child : node->children) {
        if (!child) continue;
        num++;
        file << "node" << curr << " -> node" << num << ";\n";
        child->connect_node(file, num);
    }

    if (!curr) num = 0;
}

void node_info::init_func_args(const std::vector<std::string> &signature, std::vector<int> &results) const {
    std::for_each(signature.begin(), signature.end(), [&results, this](const std::string &name){
        scopes.add_variable(name, results.back());
        results.pop_back();
    });
}

void assign_t::eval(eval_info &e_info) const {
    std::string var = static_cast<scalar_variable*>(children[0])->get_name();
    auto search = n_info.scopes.find_all_scopes(var);
    if (search == nullptr) throw std::runtime_error("can't find variable");

    int rhs = e_info.results.back(); e_info.results.pop_back();
    e_info.results.pop_back();
    search->value = rhs;
    e_info.results.push_back(rhs);
}

void func_assign_t::eval(eval_info &) const {
    if (!func_name.empty()) {
        auto search = n_info.scopes.find_global(func_name);
        if (search == nullptr)
            search = n_info.scopes.add_global(func_name, info);
        else 
            search->value = info;
    }

    std::string var = static_cast<scalar_variable*>(children[0])->get_name();
    auto search = n_info.scopes.find_all_scopes(var);
    if (search == nullptr) throw std::runtime_error("can't find function");
    search->value = info;
}

void pr_increment_t::eval(eval_info &e_info) const {
    std::string var = static_cast<scalar_variable*>(children[0])->get_name();
    auto search = n_info.scopes.find_all_scopes(var);
    if (search == nullptr) throw std::runtime_error("can't find variable");

    int lhs = e_info.results.back(); e_info.results.pop_back();
    search->value = lhs + 1;
    e_info.results.push_back(lhs + 1);
} 

void pr_decrement_t::eval(eval_info &e_info) const {
    std::string var = static_cast<scalar_variable*>(children[0])->get_name();
    auto search = n_info.scopes.find_all_scopes(var);
    if (search == nullptr) throw std::runtime_error("can't find variable");

    int lhs = e_info.results.back(); e_info.results.pop_back();
    search->value = lhs - 1;
    e_info.results.push_back(lhs - 1);
} 

void pow_t::eval(eval_info &e_info) const {
    int rhs = e_info.results.back(); e_info.results.pop_back();
    int lhs = e_info.results.back(); e_info.results.pop_back();
    e_info.results.push_back(std::pow(lhs, rhs)); 
}

void scolon_t::eval(eval_info &e_info) const {
    if (children[1]) {
        int rhs = e_info.results.back();
        e_info.results.pop_back();
        e_info.results.pop_back();
        e_info.results.push_back(rhs);
    }
}

void scalar_variable::eval(eval_info &e_info) const {
    auto search = n_info.scopes.find_all_scopes(name);
    if (search == nullptr)
        (n_info.scopes.scopes_depth() == 1 && n_info.scopes.scopes_level() == 1) ? search = n_info.scopes.add_global(name, 0) :
                                                                                   search = n_info.scopes.add_variable(name, 0);
    e_info.results.push_back(std::get<int>(search->value));
}

void print_t::eval(eval_info &e_info) const {
    int res = e_info.results.back();
    std::cout << res << std::endl; 
}

void get_t::eval(eval_info &e_info) const {
    if ((std::cin >> std::ws).eof()) 
        throw std::runtime_error("reached input file EOF");
    int res = get<int>();
    e_info.results.push_back(res);
}

void if_t::eval(eval_info &e_info) const { 
    int cond = e_info.results.back(); e_info.results.pop_back();
    (cond) ? e_info.fl = flag::IF_TRUE : e_info.fl = flag::IF_FALSE;
}

void while_t::eval(eval_info &e_info) const {
    int cond = e_info.results.back(); e_info.results.pop_back();
    (cond) ? e_info.fl = flag::WHILE_TRUE : e_info.fl = flag::WHILE_FALSE;
}

void tree_t::traversal() const {
    node_t *root_ = root;
    unsigned currentRootIndex = 0;
    std::vector<std::pair<node_t*, unsigned>> stack;
    std::vector<node_t*> return_point;
    eval_info e_info{};

    bool have_children = 1;

    while (have_children || stack.size() > 0) {
        if (have_children) {
            stack.push_back(std::pair(root_, currentRootIndex));
            currentRootIndex = 0;

            if (root_ && root_->children.size() >= 1)
                root_ = root_->children[0];
            else
                    have_children = 0;
            continue;
        }

        auto tmp = stack.back();
        stack.pop_back();
        if (tmp.first) 
            tmp.first->eval(e_info);
        else e_info.results.push_back(0);

        while (stack.size() > 0 && (tmp.second == stack.back().first->children.size() - 1 || e_info.fl == flag::WHILE_FALSE) &&
                    e_info.fl != flag::WHILE_TRUE && e_info.fl != flag::RETURN) {
            if (e_info.fl == flag::WHILE_FALSE && tmp.second == 0)
                e_info.results.push_back(0);
            
            tmp = stack.back();
            stack.pop_back();

            e_info.fl = flag::NOT_DEFINED;
            
            tmp.first->eval(e_info);
        }

        if (stack.size() > 0) {
            switch (e_info.fl) {
                case flag::AND:
                    (e_info.results.back()) ? root_ = stack.back().first->children[tmp.second + 1] : root_ = nullptr;
                    currentRootIndex = tmp.second + 1;
                    break;
                case flag::IF_FALSE: 
                    root_ = stack.back().first->children[tmp.second + 2];
                    currentRootIndex = tmp.second + 2;
                    break;
                case flag::IF_TRUE:
                    root_ = stack.back().first->children[tmp.second + 1];
                    currentRootIndex = tmp.second + 2;
                    break;
                case flag::WHILE_TRUE:
                    if (tmp.second != 0) {
                        root_ = stack.back().first->children[tmp.second - 1];
                        currentRootIndex = tmp.second - 1;
                        e_info.results.pop_back();
                    } else {
                        root_ = stack.back().first->children[tmp.second + 1];
                        currentRootIndex = tmp.second + 1;
                    }
                    break;
                case flag::FUNC_ENTRY:
                    return_point.push_back(stack.back().first);
                    root_ = e_info.root;
                    currentRootIndex = 0;
                    break;
                case flag::FUNC_EXIT: 
                    return_point.pop_back();
                    root_ = stack.back().first->children[2];
                    currentRootIndex = 2;
                    break;
                case flag::BLOCK_ENTRY:
                    return_point.push_back(stack.back().first);
                    root_ = stack.back().first->children[tmp.second + 1];
                    currentRootIndex = tmp.second + 1;
                    break;
                case flag::BLOCK_EXIT:
                    return_point.pop_back();
                    root_ = stack.back().first->children[2];
                    currentRootIndex = 2;
                    break;
                case flag::RETURN: {
                    auto return_ptr = return_point.back();
                    return_point.pop_back();

                    auto stack_it = (std::find_if(stack.rbegin(), stack.rend(), [return_ptr](auto curr){return (curr.first == return_ptr);}) + 1).base();
                    stack.erase(stack_it + 1, stack.end());

                    root_ = stack_it->first->children[2];
                    currentRootIndex = 2;
                    break;
                }
                default:
                    root_ = stack.back().first->children[tmp.second + 1];
                    currentRootIndex = tmp.second + 1;
            };
            e_info.fl = flag::NOT_DEFINED;
            have_children = 1;
        }
    }
}

void tree_t::dump() const {
    std::ofstream file("tree_dump.dot");
    file << "digraph tree {\n";

    int num = 0;

    root->graph_node(file, num);
    root->connect_node(file, num);

    file << "}";
    return;
}

} // ast
