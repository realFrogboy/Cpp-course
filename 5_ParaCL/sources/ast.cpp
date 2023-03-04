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

    if (node->cond) node->cond->graph_node(file, num);
    if (node->lhs)  node->lhs->graph_node(file, num);
    if (node->rhs)  node->rhs->graph_node(file, num);

    if (!curr) num = 0;
}

void Dump::connect(const node_t *node, std::ofstream &file, int &num) const {
    int curr = num;

    if (node->cond != nullptr) {
        num++;
        file << "node" << curr << " -> node" << num << ";\n";
        node->cond->connect_node(file, num);
    }

    if (node->lhs != nullptr) {
        num++;
        file << "node" << curr << " -> node" << num << ";\n";
        node->lhs->connect_node(file, num);
    }

    if (node->rhs != nullptr) {
        num++;
        file << "node" << curr << " -> node" << num << ";\n";
        node->rhs->connect_node(file, num);
    }

    if (!curr) num = 0;
}

int assign_t::eval() const {
    std::string var = static_cast<scalar_variable*>(lhs)->get_name();
    auto search = n_info.scopes.find_variable(var);
    if (search == nullptr) search = n_info.scopes.add_variable(var);
    search->value = rhs->eval();
    return std::get<int>(search->value);
}

int func_assign_t::eval() const {
    if (cond) {
        std::string func_name = static_cast<scalar_variable*>(cond)->get_name();
        auto search = n_info.scopes.find_func(func_name);
        if (search == nullptr)
            search = n_info.scopes.add_func(func_name, info);
        else 
            search->value = info;
    }

    std::string var = static_cast<scalar_variable*>(lhs)->get_name();
    auto search = n_info.scopes.find_variable(var);
    if (search == nullptr) search = n_info.scopes.add_variable(var);
    search->value = info;
    return 0;
}

int pr_increment_t::eval() const {
    std::string var = static_cast<scalar_variable*>(lhs)->get_name();
    auto search = n_info.scopes.find_variable(var);
    if (search == nullptr) throw std::runtime_error("can't find variable");
    search->value = lhs->eval() + 1;
    return std::get<int>(search->value);
} 

int pr_decrement_t::eval() const{
    std::string var = static_cast<scalar_variable*>(lhs)->get_name();
    auto search = n_info.scopes.find_variable(var);
    if (search == nullptr) throw std::runtime_error("can't find variable");
    search->value = lhs->eval() - 1;
    return std::get<int>(search->value);
} 

int pow_t::eval() const {
    return std::pow(lhs->eval(), rhs->eval());
}

int scolon_t::eval() const {
    int res = 0;
    if (lhs != nullptr) res = lhs->eval();
    if (rhs != nullptr && !n_info.is_return) res = rhs->eval();
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
    int res = lhs->eval();
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
    if (cond->eval()) {
        if (lhs) 
            res = lhs->eval();
    } else if (rhs && !n_info.is_return) 
        res = rhs->eval();
    n_info.scopes.close_scope();
    return res;
}

int while_t::eval() const {
    int res = 0;
    n_info.scopes.open_scope();
    while (lhs && cond->eval() && !n_info.is_return)
        res = lhs->eval();
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
