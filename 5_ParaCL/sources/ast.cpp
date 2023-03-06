#include <cmath>
#include <string>
#include <limits>
#include <fstream>
#include <iostream>

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

int assign_t::eval(eval_info &e_info) const {
    std::string var = static_cast<scalar_variable*>(children[0])->get_name();
    auto search = n_info.scopes.find_variable(var);
    if (search == nullptr) throw std::runtime_error("can't find variable");

    int rhs = e_info.results.back(); e_info.results.pop_back();
    e_info.results.pop_back();
    search->value = rhs;
    e_info.results.push_back(rhs);
    return std::get<int>(search->value);
}

int func_assign_t::eval(eval_info &e_info) const {
    if (!func_name.empty()) {
        auto search = n_info.scopes.find_func(func_name);
        if (search == nullptr)
            search = n_info.scopes.add_func(func_name, info);
        else 
            search->value = info;
    }

    std::string var = static_cast<scalar_variable*>(children[0])->get_name();
    auto search = n_info.scopes.find_variable(var);
    if (search == nullptr) throw std::runtime_error("can't find function");
    search->value = info;
    return 0;
}

int pr_increment_t::eval(eval_info &e_info) const {
    std::string var = static_cast<scalar_variable*>(children[0])->get_name();
    auto search = n_info.scopes.find_variable(var);
    if (search == nullptr) throw std::runtime_error("can't find variable");

    int lhs = e_info.results.back(); e_info.results.pop_back();
    search->value = lhs + 1;
    e_info.results.push_back(lhs + 1);
    return std::get<int>(search->value);
} 

int pr_decrement_t::eval(eval_info &e_info) const{
    std::string var = static_cast<scalar_variable*>(children[0])->get_name();
    auto search = n_info.scopes.find_variable(var);
    if (search == nullptr) throw std::runtime_error("can't find variable");

    int lhs = e_info.results.back(); e_info.results.pop_back();
    search->value = lhs - 1;
    e_info.results.push_back(lhs - 1);
    return std::get<int>(search->value);
} 

int pow_t::eval(eval_info &e_info) const {
    int rhs = e_info.results.back(); e_info.results.pop_back();
    int lhs = e_info.results.back(); e_info.results.pop_back();
    e_info.results.push_back(std::pow(lhs, rhs)); 
    return std::pow(lhs, rhs);
}

int scolon_t::eval(eval_info &e_info) const {
    if (children[1]) {
        int rhs = e_info.results.back();
        e_info.results.pop_back();
        e_info.results.pop_back();
        e_info.results.push_back(rhs);
    }
    return e_info.results.back();
}

int scalar_variable::eval(eval_info &e_info) const {
    std::string name = get_name();
    auto search = n_info.scopes.find_variable(name);
    if (search == nullptr)
        search = n_info.scopes.add_variable(name, 0);
    e_info.results.push_back(std::get<int>(search->value));
    return std::get<int>(search->value);
}

int print_t::eval(eval_info &e_info) const {
    int res = e_info.results.back();
    std::cout << res << std::endl; 
    return res;
}

int get_t::eval(eval_info &e_info) const {
    if ((std::cin >> std::ws).eof()) 
        throw std::runtime_error("reached input file EOF");
    int res = get<int>();
    e_info.results.push_back(res);
    return res;
}

int if_t::eval(eval_info &e_info) const { 
    int cond = e_info.results.back(); e_info.results.pop_back();
    (cond) ? e_info.fl = flag::IF_TRUE : e_info.fl = flag::IF_FALSE;
    return 0;
}

int while_t::eval(eval_info &e_info) const {
    int cond = e_info.results.back(); e_info.results.pop_back();
    (cond) ? e_info.fl = flag::WHILE_TRUE : e_info.fl = flag::WHILE_FALSE;
    return 0;
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
