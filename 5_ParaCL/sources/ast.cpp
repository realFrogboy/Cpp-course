#include <string>
#include <limits>
#include <fstream>
#include <iostream>

#include "ast.hpp"
#include "pow.hpp"

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

}

namespace ast {

void binop_dump::dump(const binop_t *node, std::ofstream &file, int &num) const {
    int curr = num;

    file << "\tnode" << num << " [shape = \"invtriangle\", style = \"filled\", fillcolor = \"pink\", label = \"" << operation << "\"];\n";
    num++;

    if (node->lhs) node->lhs->graph_node(file, num);
    if (node->rhs) node->rhs->graph_node(file, num);

    if (!curr) num = 0;
}

void unop_dump::dump(const unop_t *node, std::ofstream &file, int &num) const {
    int curr = num;

    file << "\tnode" << num << " [shape = \"record\", fontcolor = \"white\", style = \"filled\", fillcolor = \"grey28\", label = \"" << operation << "\"];\n";
    num++;

    node->lhs->graph_node(file, num);

    if (!curr) num = 0;
}

void num_dump::dump(const num_t *node, std::ofstream &file, int &num) const {
    int curr = num;
    
    file << "\tnode" << num << " [shape = \"record\", style = \"filled\", fillcolor = \"greenyellow\", label = \"" << node->get_value() << "\"];\n";
    num++;

    if (!curr) num = 0;
}

void variable_dump::dump(const variable_t *node, std::ofstream &file, int &num) const {
    int curr = num;
    
    file << "\tnode" << num << " [shape = \"circle\", style = \"filled\", fillcolor = \"lightskyblue1\", label = \"" << node->get_name() << "\"];\n";
    num++;

    if (!curr) num = 0;
}

void func_dump::dump(const func_t *node, std::ofstream &file, int &num) const {
    int curr = num;

    file << "\tnode" << num << " [shape = \"diamond\", style = \"filled\", fillcolor = \"red\", label = \"" << func << "\"];\n";
    num++;

    if (node->lhs) node->lhs->graph_node(file, num);

    if (!curr) num = 0;
}

void flow_dump::dump(const flow_t *node, std::ofstream &file, int &num) const {
    int curr = num;

    file << "\tnode" << num << " [shape = \"invhouse\", style = \"filled\", fillcolor = \"gold\", label = \"" << flow << "\"];\n";
    num++;

    node->cond->graph_node(file, num);
    if (node->lhs) node->lhs->graph_node(file, num);

    if (node->rhs) node->rhs->graph_node(file, num);

    if (!curr) num = 0;
}

void binop_dump::connect(const binop_t *node, std::ofstream& file, int &num) const {
    int curr = num;

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

void unop_dump::connect(const unop_t *node, std::ofstream& file, int &num) const {
    int curr = num;

    if (node->lhs != nullptr) {
        num++;
        file << "node" << curr << " -> node" << num << ";\n";

        node->lhs->connect_node(file, num);
    }

    if (!curr) num = 0;
}

void num_dump::connect(const num_t *node, std::ofstream& file, int &num) const { return; }
void variable_dump::connect(const variable_t *node, std::ofstream& file, int &num) const { return; }

void func_dump::connect(const func_t *node, std::ofstream& file, int &num) const {
    int curr = num;

    if (node->lhs != nullptr) {
        num++;
        file << "node" << curr << " -> node" << num << ";\n";

        node->lhs->connect_node(file, num);
    }

    if (!curr) num = 0;
}

void flow_dump::connect(const flow_t *node, std::ofstream& file, int &num) const {
    int curr = num;

    num++;
    file << "node" << curr << " -> node" << num << ";\n";

    node->cond->connect_node(file, num);

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

int assign_t::eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) {
    std::string var = static_cast<variable_t*>(lhs)->get_name();
    size_t n = variables.size();
    std::unordered_map<std::string, name_t>::iterator search;
    for (int idx = n - 1; idx >= 0; --idx)
        search = variables[idx].find(var);
    if (search == variables[0].end()) {
        auto new_var = variables[n-1].insert({var, name_t{var, 0, 0}});
        search = new_var.first;
    }
    search->second.value = rhs->eval(variables);
    return search->second.value;
}

int pr_increment_t::eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) {
    std::string var = static_cast<variable_t*>(lhs)->get_name();
    size_t n = variables.size();
    std::unordered_map<std::string, name_t>::iterator search;
    for (int idx = n - 1; idx >= 0; --idx)
        search = variables[idx].find(var);
    search->second.value = lhs->eval(variables) + 1;
    return search->second.value;
} 

int pr_decrement_t::eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) {
    std::string var = static_cast<variable_t*>(lhs)->get_name();
    size_t n = variables.size();
    std::unordered_map<std::string, name_t>::iterator search;
    for (int idx = n - 1; idx >= 0; --idx)
        search = variables[idx].find(var);
    search->second.value = lhs->eval(variables) - 1;
    return search->second.value;
} 

int pow_t::eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) {
    int degree = rhs->eval(variables);
    if (degree < 0) {
        int res = pow(lhs->eval(variables), -degree);
        return 1/res;
    } else {
        int res = pow(lhs->eval(variables), degree);
        return res;
    }
}

int scolon_t::eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) {
    if (lhs != nullptr) lhs->eval(variables);
    if (rhs != nullptr) rhs->eval(variables);
    return 0;
}

int num_t::eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) {
    return value;
}

int variable_t::eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) {
    std::string name = get_name();
    size_t n = variables.size();
    std::unordered_map<std::string, name_t>::iterator search;
    for (int idx = n - 1; idx >= 0; --idx)
        search = variables[idx].find(name);
    if (search == variables[0].end()) {
        auto new_var = variables[n-1].insert({name, name_t{name, 0, 0}});
        search = new_var.first;
    }
    return search->second.value;
}

int print_t::eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) {
    int res = lhs->eval(variables);
    std::cout << res << std::endl; 
    return res;
}

int get_t::eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) {
    if ((std::cin >> std::ws).eof()) 
        throw std::runtime_error("reached input file EOF");
    int res = get<int>();
    return res;
}

int if_t::eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) { 
    std::unordered_map<std::string, name_t> if_scope;
    variables.push_back(if_scope);
    if (cond->eval(variables)) {
        std::unordered_map<std::string, name_t> scope;
        variables.push_back(scope);
        if (lhs) lhs->eval(variables);
        variables.pop_back();
    } else if (rhs != nullptr) {
        std::unordered_map<std::string, name_t> scope;
        variables.push_back(scope);
        if (rhs) rhs->eval(variables);
        variables.pop_back();
    }
    variables.pop_back();
    return 0;
}

int while_t::eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) {
    std::unordered_map<std::string, name_t> while_scope;
    variables.push_back(while_scope);
    while (cond->eval(variables)) {
        std::unordered_map<std::string, name_t> scope;
        variables.push_back(scope);
        if (lhs) lhs->eval(variables);
        variables.pop_back();
    }
    variables.pop_back();
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