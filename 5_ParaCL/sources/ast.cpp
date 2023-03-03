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
    auto search = scopes.find_variable(var);
    if (search == nullptr) search = scopes.add_variable(var);
    search->value = rhs->eval();
    return std::get<int>(search->value);
}

int func_assign_t::eval() const {
    std::string var = static_cast<scalar_variable*>(lhs)->get_name();
    auto search = scopes.find_variable(var);
    if (search == nullptr) search = scopes.add_variable(var);
    search->value = info;
    return 0;
}

int pr_increment_t::eval() const {
    std::string var = static_cast<scalar_variable*>(lhs)->get_name();
    auto search = scopes.find_variable(var);
    if (search == nullptr) throw std::runtime_error("can't find variable");
    search->value = lhs->eval() + 1;
    return std::get<int>(search->value);
} 

int pr_decrement_t::eval() const{
    std::string var = static_cast<scalar_variable*>(lhs)->get_name();
    auto search = scopes.find_variable(var);
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
    if (rhs != nullptr && !scopes.get_return_status()) res = rhs->eval();
    return res;
}

int scalar_variable::eval() const {
    std::string name = get_name();
    auto search = scopes.find_variable(name);
    if (search == nullptr) {
        search = scopes.add_variable(name);
        return 0;
    }
    return std::get<int>(search->value);
}

int func_variable::eval() const {
    std::string name = get_name();
    auto search = scopes.find_variable(name);
    if (search == nullptr) throw std::runtime_error("can't find function");

    if (args) args->eval();

    scopes.hide_scopes();

    func_info info = std::get<func_info>(search->value);
    scopes.init_func_args(info.signature);

    int res = info.root->eval();
    scopes.remove_return_status();

    scopes.recover_scopes();
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
    scopes.open_scope();
    if (cond->eval()) {
        if (lhs) 
            res = lhs->eval();
    } else if (rhs && !scopes.get_return_status()) 
        res = rhs->eval();
    scopes.close_scope();
    return res;
}

int while_t::eval() const {
    int res = 0;
    scopes.open_scope();
    while (lhs && cond->eval() && !scopes.get_return_status())
        res = lhs->eval();
    scopes.close_scope();
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
