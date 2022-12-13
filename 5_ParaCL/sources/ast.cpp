#include <string>
#include <fstream>
#include <iostream>

#include "ast.hpp"
#include "pow.hpp"

namespace ast {

std::string binop_dump::get_binop_operation(const binop_t *node) const {
    switch(node->get_operation()) {
        case 1: return ">";
        case 2: return "<";
        case 3: return "!=";
        case 4: return "==";
        case 5: return ">=";
        case 6: return "<=";
        case 7: return "+";
        case 8: return "-";
        case 9: return "*";
        case 10: return "/";
        case 11: return "**";
        case 12: return "=";
        case 13: return ";";
        default: throw std::runtime_error("no such binary operation");
    }
}

std::string unop_dump::get_unop_operation(const unop_t *node) const {
    switch(node->get_operation()) {
        case 1: return "-";
        default: throw std::runtime_error("no such unary operation");
    }
}

std::string func_dump::get_func_d(const func_t *node) const {
    switch(node->get_func()) {
        case 1: return "print";
        case 2: return "scan";
        case 3: return "abs";
        default: throw std::runtime_error("no such function");
    }
}

std::string flow_dump::get_flow_d(const flow_t *node) const {
    switch(node->get_flow()) {
        case 1: return "if";
        case 2: return "while";
        default: throw std::runtime_error("no such flow");
    }
}

void binop_dump::dump(const binop_t *node, std::ofstream &file, int &num) const {
    int curr = num;

    std::string op = get_binop_operation(node);
    file << "\tnode" << num << " [shape = \"invtriangle\", style = \"filled\", fillcolor = \"pink\", label = \"" << op << "\"];\n";
    num++;

    node->lhs->graph_node(file, num);
    node->rhs->graph_node(file, num);

    if (!curr) num = 0;
}

void unop_dump::dump(const unop_t *node, std::ofstream &file, int &num) const {
    int curr = num;

    std::string op = get_unop_operation(node);
    file << "\tnode" << num << " [shape = \"record\", fontcolor = \"white\", style = \"filled\", fillcolor = \"grey28\", label = \"" << op << "\"];\n";
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
    
    file << "\tnode" << num << " [shape = \"circle\", style = \"filled\", fillcolor = \"lightskyblue1\", label = \"" << node->get_name().name << "\"];\n";
    num++;

    if (!curr) num = 0;
}

void func_dump::dump(const func_t *node, std::ofstream &file, int &num) const {
    int curr = num;

    std::string func = get_func_d(node);
    file << "\tnode" << num << " [shape = \"diamond\", style = \"filled\", fillcolor = \"red\", label = \"" << func << "\"];\n";
    num++;

    node->lhs->graph_node(file, num);

    if (!curr) num = 0;
}

void flow_dump::dump(const flow_t *node, std::ofstream &file, int &num) const {
    int curr = num;

    std::string flow = get_flow_d(node);
    file << "\tnode" << num << " [shape = \"invhouse\", style = \"filled\", fillcolor = \"antiquewhite\", label = \"" << flow << "\"];\n";
    num++;

    node->cond->graph_node(file, num);
    node->lhs->graph_node(file, num);

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

int binop_t::eval(std::unordered_map<std::string, ast::name_t> &variables) {
    switch (operation) {
        case binop_type::G: return (lhs->eval(variables) > rhs->eval(variables));
        case binop_type::L: return (lhs->eval(variables) < rhs->eval(variables));
        case binop_type::E: return (lhs->eval(variables) == rhs->eval(variables));
        case binop_type::NE: return (lhs->eval(variables) != rhs->eval(variables));
        case binop_type::GE: return (lhs->eval(variables) >= rhs->eval(variables));
        case binop_type::LE: return (lhs->eval(variables) <= rhs->eval(variables));
        case binop_type::ADD: return (lhs->eval(variables) + rhs->eval(variables));
        case binop_type::SUB: return (lhs->eval(variables) - rhs->eval(variables));
        case binop_type::MUL: return (lhs->eval(variables) * rhs->eval(variables));
        case binop_type::DIV: return (lhs->eval(variables) / rhs->eval(variables));
        case binop_type::ASSIGN: {
            std::string var = static_cast<variable_t*>(lhs)->get_name().name;
            auto search = variables.find(var);
            search->second.value = rhs->eval(variables);
            return search->second.value;
        }
        case binop_type::POW: {
            int degree = rhs->eval(variables);
            if (degree < 0) {
                int res = pow(lhs->eval(variables), -degree);
                return 1/res;
            } else {
                int res = pow(lhs->eval(variables), degree);
                return res;
            }
        }
        case binop_type::SCOLON: {
            if (lhs != nullptr) lhs->eval(variables);
            if (rhs != nullptr) rhs->eval(variables);
            return 0;
        }
        default: throw std::runtime_error("no such binary opertion");
    }
}

int unop_t::eval(std::unordered_map<std::string, ast::name_t> &variables) {
    switch (operation) {
        case unop_type::MINUS: return -lhs->eval(variables); break;
        default: throw std::runtime_error("no such unary opertion");
    }
}

int num_t::eval(std::unordered_map<std::string, ast::name_t> &variables) {
    return value;
}

int variable_t::eval(std::unordered_map<std::string, ast::name_t> &variables) {
    auto search = variables.find(name.name);
    return search->second.value;
}

int func_t::eval(std::unordered_map<std::string, ast::name_t> &variables) {
    switch (func_id) {
        case func_type::ABS  : return std::abs(lhs->eval(variables));
        case func_type::PRINT: std::cout << lhs->eval(variables) << std::endl; return 0;
        case func_type::SCAN : {
            std::string var = static_cast<variable_t*>(lhs)->get_name().name;
            auto search = variables.find(var);
            std::cin >> search->second.value;
            return search->second.value;
        }
        default: throw std::runtime_error("no such function");
    }
}

int flow_t::eval(std::unordered_map<std::string, ast::name_t> &variables) {
    switch (flow_id) {
        case flow_type::IF: {
            if (cond->eval(variables)) {
                lhs->eval(variables);
            } else if (rhs != nullptr) {
                rhs->eval(variables);
            }
            return 0;
        }
        case flow_type::WHILE: {
            while (cond->eval(variables)) {
                lhs->eval(variables);
            }
            return 0;
        }
        default: throw std::runtime_error("no such flow");
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