#include <string>
#include <fstream>
#include <iostream>

#include "ast.hpp"
#include "pow.hpp"

namespace ast {

std::string tree_dump::get_binop_operation(const binop_t *node) {
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

std::string tree_dump::get_unop_operation(const unop_t *node) {
    switch(node->get_operation()) {
        case 1: return "-";
        default: throw std::runtime_error("no such unary operation");
    }
}

std::string tree_dump::get_func(const func_t *node) {
    switch(node->get_func()) {
        case 1: return "print";
        case 2: return "scan";
        case 3: return "abs";
        default: throw std::runtime_error("no such function");
    }
}

std::string tree_dump::get_flow(const flow_t *node) {
    switch(node->get_flow()) {
        case 1: return "if";
        case 2: return "while";
        default: throw std::runtime_error("no such flow");
    }
}

void tree_dump::graph_node(const node_t *node, std::ofstream& file) {
    //if (node == nullptr) return;

    switch(node->get_type()) {
        case node_type::BINOP:    graph_binop_node(node, file); break;
        case node_type::UNOP:     graph_unop_node(node, file); break;
        case node_type::NUMBER:   graph_num_node(node, file); break;
        case node_type::VARIABLE: graph_var_node(node, file); break;
        case node_type::FUNCTION: graph_func_node(node, file); break;
        case node_type::FLOW:     graph_flow_node(node, file); break;
        default: throw std::runtime_error("no such node type");
    }
}

void tree_dump::graph_binop_node(const node_t *node, std::ofstream &file) {
    const binop_t *b_node = static_cast<const binop_t*>(node);
    int curr = num;

    std::string op = get_binop_operation(b_node);
    file << "\tnode" << num << " [shape = \"invtriangle\", style = \"filled\", fillcolor = \"pink\", label = \"" << op << "\"];\n";
    num++;

    graph_node(b_node->lhs, file);
    graph_node(b_node->rhs, file);

    if (!curr) num = 0;
}

void tree_dump::graph_unop_node(const node_t *node, std::ofstream &file) {
    const unop_t *u_node = static_cast<const unop_t*>(node);
    int curr = num;

    std::string op = get_unop_operation(u_node);
    file << "\tnode" << num << " [shape = \"record\", fontcolor = \"white\", style = \"filled\", fillcolor = \"grey28\", label = \"" << op << "\"];\n";
    num++;

    graph_node(u_node->lhs, file);

    if (!curr) num = 0;
}

void tree_dump::graph_num_node(const node_t *node, std::ofstream &file) {
    int curr = num;
    
    file << "\tnode" << num << " [shape = \"record\", style = \"filled\", fillcolor = \"greenyellow\", label = \"" << static_cast<const num_t*>(node)->get_value() << "\"];\n";
    num++;

    if (!curr) num = 0;
}

void tree_dump::graph_var_node(const node_t *node, std::ofstream &file) {
    int curr = num;
    
    file << "\tnode" << num << " [shape = \"circle\", style = \"filled\", fillcolor = \"lightskyblue1\", label = \"" << static_cast<const variable_t*>(node)->get_name().name << "\"];\n";
    num++;

    if (!curr) num = 0;
}

void tree_dump::graph_func_node(const node_t *node, std::ofstream &file) {
    const func_t *f_node = static_cast<const func_t*>(node);
    int curr = num;

    std::string func = get_func(f_node);
    file << "\tnode" << num << " [shape = \"diamond\", style = \"filled\", fillcolor = \"red\", label = \"" << func << "\"];\n";
    num++;

    graph_node(f_node->lhs, file);

    if (!curr) num = 0;
}

void tree_dump::graph_flow_node(const node_t *node, std::ofstream &file) {
    const flow_t *f_node = static_cast<const flow_t*>(node);
    int curr = num;

    std::string flow = get_flow(f_node);
    file << "\tnode" << num << " [shape = \"invhouse\", style = \"filled\", fillcolor = \"antiquewhite\", label = \"" << flow << "\"];\n";
    num++;

    graph_node(f_node->cond, file);
    graph_node(f_node->lhs, file);

    if (f_node->rhs) graph_node(f_node->rhs, file);

    if (!curr) num = 0;
}

void tree_dump::connect_node(const node_t *node, std::ofstream& file) {
    switch(node->get_type()) {
        case node_type::BINOP:    connect_bin_node(node, file); break;
        case node_type::FUNCTION:
        case node_type::UNOP:     connect_un_node(node, file); break;
        case node_type::NUMBER:   
        case node_type::VARIABLE: break;
        case node_type::FLOW:     connect_flow_node(node, file); break;
        default: throw std::runtime_error("no such node type");
    }
}

void tree_dump::connect_bin_node(const node_t *node, std::ofstream& file) {
    const binop_t *b_node = static_cast<const binop_t*>(node);
    int curr = num;

    if (b_node->lhs != nullptr) {
        num++;
        file << "node" << curr << " -> node" << num << ";\n";

        connect_node(b_node->lhs, file);
    }

    if (b_node->rhs != nullptr) {
        num++;
        file << "node" << curr << " -> node" << num << ";\n";

        connect_node(b_node->rhs, file);
    }

    if (!curr) num = 0;
}

void tree_dump::connect_un_node(const node_t *node, std::ofstream& file) {
    const unop_t *u_node = static_cast<const unop_t*>(node);
    int curr = num;

    if (u_node->lhs != nullptr) {
        num++;
        file << "node" << curr << " -> node" << num << ";\n";

        connect_node(u_node->lhs, file);
    }

    if (!curr) num = 0;
}

void tree_dump::connect_flow_node(const node_t *node, std::ofstream& file) {
    const flow_t *f_node = static_cast<const flow_t*>(node);
    int curr = num;

    num++;
    file << "node" << curr << " -> node" << num << ";\n";

    connect_node(f_node->cond, file);

    if (f_node->lhs != nullptr) {
        num++;
        file << "node" << curr << " -> node" << num << ";\n";

        connect_node(f_node->lhs, file);
    }

    if (f_node->rhs != nullptr) {
        num++;
        file << "node" << curr << " -> node" << num << ";\n";

        connect_node(f_node->rhs, file);
    }

    if (!curr) num = 0;
}

int tree_t::eval(const node_t *node) {
    switch (node->get_type()) {
        case node_type::BINOP   : return evaluate_binop(static_cast<const binop_t*>(node));
        case node_type::UNOP    : return evaluate_unop (static_cast<const unop_t*>(node));
        case node_type::NUMBER  : return evaluate_num  (static_cast<const num_t*>(node));
        case node_type::VARIABLE: return evaluate_var  (static_cast<const variable_t*>(node));
        case node_type::FUNCTION: return evaluate_func (static_cast<const func_t*>(node));
        case node_type::FLOW    : return evaluate_flow (static_cast<const flow_t*>(node));
        default: throw std::runtime_error("no such function");
    }
}

int tree_t::evaluate_binop(const binop_t *node) {
    switch (node->get_operation()) {
        case binop_type::G: return (eval(node->lhs) > eval(node->rhs));
        case binop_type::L: return (eval(node->lhs) < eval(node->rhs));
        case binop_type::E: return (eval(node->lhs) == eval(node->rhs));
        case binop_type::NE: return (eval(node->lhs) != eval(node->rhs));
        case binop_type::GE: return (eval(node->lhs) >= eval(node->rhs));
        case binop_type::LE: return (eval(node->lhs) <= eval(node->rhs));
        case binop_type::ADD: return (eval(node->lhs) + eval(node->rhs));
        case binop_type::SUB: return (eval(node->lhs) - eval(node->rhs));
        case binop_type::MUL: return (eval(node->lhs) * eval(node->rhs));
        case binop_type::DIV: return (eval(node->lhs) / eval(node->rhs));
        case binop_type::ASSIGN: {
            std::string var = static_cast<variable_t*>(node->lhs)->get_name().name;
            auto search = variables.find(var);
            search->second.value = eval(node->rhs);
            return search->second.value;
        }
        case binop_type::POW: {
            int degree = eval(node->rhs);
            if (degree < 0) {
                int res = pow(eval(node->lhs), -degree);
                return 1/res;
            } else {
                int res = pow(eval(node->lhs), degree);
                return res;
            }
        }
        case binop_type::SCOLON: {
            if (node->lhs != nullptr) eval(node->lhs);
            if (node->rhs != nullptr) eval(node->rhs);
            return 0;
        }
        default: throw std::runtime_error("no such binary opertion");
    }
}

int tree_t::evaluate_unop(const unop_t *node) {
    switch (node->get_operation()) {
        case unop_type::MINUS: return -eval(node->lhs); break;
        default: throw std::runtime_error("no such unary opertion");
    }
}

int tree_t::evaluate_var(const variable_t *node) {
    auto search = variables.find(node->get_name().name);
    return search->second.value;
}

int tree_t::evaluate_func(const func_t *node) {
    switch (node->get_func()) {
        case func_type::ABS  : return std::abs(eval(node->lhs));
        case func_type::PRINT: std::cout << eval(node->lhs) << std::endl; return 0;
        case func_type::SCAN : {
            std::string var = static_cast<variable_t*>(node->lhs)->get_name().name;
            auto search = variables.find(var);
            std::cin >> search->second.value;
            return search->second.value;
        }
        default: throw std::runtime_error("no such function");
    }
}

int tree_t::evaluate_flow(const flow_t *node) {
    switch (node->get_flow()) {
        case flow_type::IF: {
            if (eval(node->cond)) {
                eval(node->lhs);
            } else if (node->rhs != nullptr) {
                eval(node->rhs);
            }
            return 0;
        }
        case flow_type::WHILE: {
            while (eval(node->cond)) {
                eval(node->lhs);
            }
            return 0;
        }
        default: throw std::runtime_error("no such flow");
    }
}

void tree_t::dump() const {
    std::ofstream file("tree_dump.dot");
    file << "digraph tree {\n";

    tree_dump dump{};
    dump.graph_node(root, file);
    dump.connect_node(root, file);

    file << "}";
    return;
}

} // ast