#include <cmath>
#include <string>
#include <limits>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string_view>

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

void var_assign_t::eval(eval_info &e_info) const {
    auto search = find_name();

    int rhs = e_info.get_result();
    search->value = rhs;

    e_info.results.erase(std::prev(e_info.results.end(), 2));
}

void func_assign_t::eval(eval_info &) const {
    if (!func_name.empty()) {
        auto search = n_info.scopes.find_global(func_name);
        if (search == nullptr)
            search = n_info.scopes.add_global(func_name, info);
        else 
            search->value = info;
    }

    auto search = find_name();
    search->value = info;
}

void pr_increment_t::eval(eval_info &e_info) const {
    auto search = find_name();
    int lhs = e_info.remove_result();
    search->value = lhs + 1;
    e_info.add_result(lhs + 1);
} 

void pr_decrement_t::eval(eval_info &e_info) const {
    auto search = find_name();
    int lhs = e_info.remove_result();
    search->value = lhs - 1;
    e_info.add_result(lhs - 1);
}

void scalar_variable::eval(eval_info &e_info) const {
    auto search = n_info.scopes.find_all_scopes(name);
    if (search == nullptr) {
        if (n_info.scopes.is_global()) 
            search = n_info.scopes.add_global(name, 0);
        else 
            search = n_info.scopes.add_variable(name, 0);
    }

    e_info.add_result(std::get<int>(search->value));
}

void print_t::eval(eval_info &e_info) const {
    int res = e_info.results.back();
    std::cout << res << std::endl; 
}

void get_t::eval(eval_info &e_info) const {
    if ((std::cin >> std::ws).eof()) 
        throw std::runtime_error("reached input file EOF");
    int res = get<int>();
    e_info.add_result(res);
}

void if_t::eval(eval_info &e_info) const { 
    int cond = e_info.remove_result();
    if (cond)
        e_info.fl = flag::IF_TRUE;
    else 
        e_info.fl = flag::IF_FALSE;
}

void while_t::eval(eval_info &e_info) const {
    int cond = e_info.remove_result();
    if (cond) 
        e_info.fl = flag::WHILE_TRUE; 
    else 
        e_info.fl = flag::WHILE_FALSE;
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
