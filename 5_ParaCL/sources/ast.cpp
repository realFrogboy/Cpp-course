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
        child->graph_node(file, num);

    if (!curr) num = 0;
}

void Dump::connect(const node_t *node, std::ofstream &file, int &num) const {
    int curr = num;

    for (auto child : node->children) {
        num++;
        file << "node" << curr << " -> node" << num << ";\n";
        child->connect_node(file, num);
    }

    if (!curr) num = 0;
}

int assign_t::eval() const {
    std::string var = static_cast<scalar_variable*>(children[0])->get_name();
    auto search = n_info.scopes.find_variable(var);
    if (search == nullptr) search = n_info.scopes.add_variable(var);
    search->value = children[1]->eval();
    return std::get<int>(search->value);
}

int func_assign_t::eval() const {
    if (children[1]) {
        std::string func_name = static_cast<scalar_variable*>(children[1])->get_name();
        auto search = n_info.scopes.find_func(func_name);
        if (search == nullptr)
            search = n_info.scopes.add_func(func_name, info);
        else 
            search->value = info;
    }

    std::string var = static_cast<scalar_variable*>(children[0])->get_name();
    auto search = n_info.scopes.find_variable(var);
    if (search == nullptr) search = n_info.scopes.add_variable(var);
    search->value = info;
    return 0;
}

int pr_increment_t::eval() const {
    std::string var = static_cast<scalar_variable*>(children[0])->get_name();
    auto search = n_info.scopes.find_variable(var);
    if (search == nullptr) throw std::runtime_error("can't find variable");
    search->value = children[0]->eval() + 1;
    return std::get<int>(search->value);
} 

int pr_decrement_t::eval() const{
    std::string var = static_cast<scalar_variable*>(children[0])->get_name();
    auto search = n_info.scopes.find_variable(var);
    if (search == nullptr) throw std::runtime_error("can't find variable");
    search->value = children[0]->eval() - 1;
    return std::get<int>(search->value);
} 

int pow_t::eval() const {
    return std::pow(children[0]->eval(), children[1]->eval());
}

int scolon_t::eval() const {
    int res = 0;
    if (children[0] != nullptr) res = children[0]->eval();
    if (children[1] != nullptr && !n_info.is_return) res = children[1]->eval();
    return res;
}

int scalar_variable::eval() const {
    std::string name = get_name();
    auto search = n_info.scopes.find_variable(name);
    if (search == nullptr) throw std::runtime_error("can't find scalar");
    return std::get<int>(search->value);
}

int func_variable::eval() const {
    std::string name = get_name();
    auto search = n_info.scopes.find_func(name);
    if (search == nullptr) 
        search = n_info.scopes.find_variable(name);
    if (search == nullptr) throw std::runtime_error("can't find function");

    if (args) args->eval();

    n_info.scopes.hide_scopes();

    func_info info = std::get<func_info>(search->value);
    n_info.init_func_args(info.signature);
    n_info.arg_list.clear();

    int res = info.root->eval();
    n_info.is_return = 0;

    n_info.scopes.recover_scopes();
    return res;
}

int print_t::eval() const {
    int res = children[0]->eval();
    std::cout << res << std::endl; 
    return res;
}

int get_t::eval() const {
    if ((std::cin >> std::ws).eof()) 
        throw std::runtime_error("reached input file EOF");
    int res = get<int>();
    return res;
}

int if_t::eval() const { 
    int res = 0;
    n_info.scopes.open_scope();
    if (children[2]->eval()) {
        if (children[0]) 
            res = children[0]->eval();
    } else if (children[1] && !n_info.is_return) 
        res = children[1]->eval();
    n_info.scopes.close_scope();
    return res;
}

int while_t::eval() const {
    int res = 0;
    n_info.scopes.open_scope();
    while (children[0] && children[1]->eval() && !n_info.is_return)
        res = children[0]->eval();
    n_info.scopes.close_scope();
    return res;
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
