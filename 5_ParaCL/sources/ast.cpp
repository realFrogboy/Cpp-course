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
    std::string var = static_cast<variable_t*>(lhs)->get_name();
    auto scope = find_variable(var);
    scope_t::iterator search;
    (scope == scopes.rend()) ? search = scopes.back().insert({var, name_t{var, 0, 0}}).first : search = scope->find(var);
    search->second.value = rhs->eval();
    return search->second.value;
}

int pr_increment_t::eval() const {
    std::string var = static_cast<variable_t*>(lhs)->get_name();
    auto scope = find_variable(var);
    auto search = scope->find(var);
    search->second.value = lhs->eval() + 1;
    return search->second.value;
} 

int pr_decrement_t::eval() const{
    std::string var = static_cast<variable_t*>(lhs)->get_name();
    auto scope = find_variable(var);
    auto search = scope->find(var);
    search->second.value = lhs->eval() - 1;
    return search->second.value;
} 

int pow_t::eval() const {
    return std::pow(lhs->eval(), rhs->eval());
}

int scolon_t::eval() const {
    if (lhs != nullptr) lhs->eval();
    if (rhs != nullptr) rhs->eval();
    return 0;
}

int variable_t::eval() const {
    std::string name = get_name();
    auto scope = find_variable(name);
    if (scope == scopes.rend()) {
        scopes.back().insert({name, name_t{name, 0, 0}});
        return 0;
    }
    auto search = scope->find(name);
    return search->second.value;
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
    scopes.push_back({});
    if (cond->eval()) {
        if (lhs) {
            scopes.push_back({});
            lhs->eval();
            scopes.pop_back();
        }
    } else if (rhs) {
        scopes.push_back({});
        rhs->eval();
        scopes.pop_back();
    }
    scopes.pop_back();
    return 0;
}

int while_t::eval() const {
    scopes.push_back({});
    while (lhs && cond->eval()) {
        scopes.push_back({});
        lhs->eval();
        scopes.pop_back();
    }
    scopes.pop_back();
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