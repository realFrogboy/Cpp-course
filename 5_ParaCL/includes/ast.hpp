#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>

#include "pow.hpp"

namespace ast {

enum node_type {
    BINOP = 0,
    UNOP,
    NUMBER,
    VARIABLE,
    FUNCTION,
    FLOW
};

enum binop_type {
    G = 1,
    L,
    NE,
    E,
    GE,
    LE,

    ADD,
    SUB,
    MUL,
    DIV,
    POW,

    ASSIGN,

    SCOLON
};

enum unop_type {
    MINUS = 1
};

enum func_type {
    PRINT = 1,
    ABS
};

enum flow_type {
    IF = 1,
    WHILE
};

struct name_t {
    std::string name;
    double value = 0;
};

class node_t {
    node_type type;

    public:

    node_t(node_type t) : type{t} {}

    node_type get_type() const { return type; }

    virtual ~node_t() {}
};


class binop_t final : public node_t {
    binop_type operation;

    public:

    node_t *lhs = nullptr;
    node_t *rhs = nullptr;

    binop_t(binop_type op, node_t *lhs_, node_t *rhs_) : node_t{node_type::BINOP}, operation{op}, lhs{lhs_}, rhs{rhs_} {}

    binop_t(binop_t &&node) : node_t{node_type::BINOP} {
        std::swap(lhs, node.lhs);
        std::swap(rhs, node.rhs);
        operation = node.operation;
    }

    binop_type get_operation() const { return operation; }
};

class unop_t final : public node_t {
    unop_type operation;

    public:

    node_t *lhs = nullptr;

    unop_t(unop_type op, node_t *lhs_) : node_t{node_type::UNOP}, operation{op}, lhs{lhs_} {}

    unop_t(unop_t &&node) : node_t{node_type::UNOP} {
        std::swap(lhs, node.lhs);
        operation = node.operation;
    }

    unop_type get_operation() const { return operation; }
};

class num_t final : public node_t {
    int value = 0;

    public:

    num_t(int v) : node_t{node_type::NUMBER}, value(v) {}

    num_t(num_t &&node) : node_t{node_type::NUMBER} {
        value = node.value;
    }

    int get_value() const { return value; }
};

class variable_t final : public node_t {
    name_t name;

    public:

    variable_t(name_t name_) : node_t{node_type::VARIABLE}, name(name_) {}

    variable_t(variable_t &&node) : node_t{node_type::VARIABLE} {
        name = node.name;
    }

    name_t get_name() const { return name; }

    void set_value(int value) { name.value = value; }
};

class func_t final : public node_t {
    func_type func_id;

    public :

    node_t *lhs = nullptr;

    func_t(int func_id_, node_t *lhs_) : node_t{node_type::FUNCTION}, func_id{static_cast<func_type>(func_id_)}, lhs{lhs_} {}

    func_t(func_t &&node) : node_t{node_type::FUNCTION} {
        std::swap(lhs, node.lhs);
        func_id = node.func_id;
    }

    func_type get_func() const { return func_id; }
};

class flow_t final : public node_t {
    flow_type flow_id;

    public:

    node_t *cond = nullptr;
    node_t *lhs = nullptr;
    node_t *rhs = nullptr;

    flow_t(flow_type flow_id_, node_t *cond_, node_t *lhs_, node_t *rhs_) : node_t{node_type::FLOW}, flow_id{flow_id_}, cond{cond_}, lhs{lhs_}, rhs{rhs_}  {}

    flow_t(flow_t &&node) : node_t{node_type::FLOW} {
        std::swap(cond, node.cond);
        std::swap(lhs, node.lhs);
        std::swap(rhs, node.rhs);
        flow_id = node.flow_id;
    }

    flow_type get_flow() const { return flow_id; }
};

class tree_dump final {

    int num = 0;

    static std::string get_binop_operation(const binop_t *node) {
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
        };
        return "error";
    }

    static std::string get_unop_operation(const unop_t *node) {
        switch(node->get_operation()) {
            case 1: return "-";
        };
        return "error";
    }

    static std::string get_func(const func_t *node) {
        switch(node->get_func()) {
            case 1: return "print";
            case 2: return "abs";
        };
        return "error";
    }

    static std::string get_flow(const flow_t *node) {
        switch(node->get_flow()) {
            case 1: return "if";
            case 2: return "while";
        };
        return "error";
    }

    public:

    void graph_node(const node_t *node, std::ofstream& file) {
        //if (node == nullptr) return;

        switch(node->get_type()) {
            case node_type::BINOP:    graph_binop_node(node, file); break;
            case node_type::UNOP:     graph_unop_node(node, file); break;
            case node_type::NUMBER:   graph_num_node(node, file); break;
            case node_type::VARIABLE: graph_var_node(node, file); break;
            case node_type::FUNCTION: graph_func_node(node, file); break;
            case node_type::FLOW:     graph_flow_node(node, file);
        }
    }

    void graph_binop_node(const node_t *node, std::ofstream &file) {
        const binop_t *b_node = static_cast<const binop_t*>(node);
        int curr = num;

        std::string op = get_binop_operation(b_node);
        file << "\tnode" << num << " [shape = \"invtriangle\", style = \"filled\", fillcolor = \"pink\", label = \"" << op << "\"];\n";
        num++;

        graph_node(b_node->lhs, file);
        graph_node(b_node->rhs, file);

        if (!curr) num = 0;
    }

    void graph_unop_node(const node_t *node, std::ofstream &file) {
        const unop_t *u_node = static_cast<const unop_t*>(node);
        int curr = num;

        std::string op = get_unop_operation(u_node);
        file << "\tnode" << num << " [shape = \"record\", fontcolor = \"white\", style = \"filled\", fillcolor = \"grey28\", label = \"" << op << "\"];\n";
        num++;

        graph_node(u_node->lhs, file);

        if (!curr) num = 0;
    }

    void graph_num_node(const node_t *node, std::ofstream &file) {
        int curr = num;
        
        file << "\tnode" << num << " [shape = \"record\", style = \"filled\", fillcolor = \"greenyellow\", label = \"" << static_cast<const num_t*>(node)->get_value() << "\"];\n";
        num++;

        if (!curr) num = 0;
    }

    void graph_var_node(const node_t *node, std::ofstream &file) {
        int curr = num;
        
        file << "\tnode" << num << " [shape = \"circle\", style = \"filled\", fillcolor = \"lightskyblue1\", label = \"" << static_cast<const variable_t*>(node)->get_name().name << "\"];\n";
        num++;

        if (!curr) num = 0;
    }

    void graph_func_node(const node_t *node, std::ofstream &file) {
        const func_t *f_node = static_cast<const func_t*>(node);
        int curr = num;

        std::string func = get_func(f_node);
        file << "\tnode" << num << " [shape = \"diamond\", style = \"filled\", fillcolor = \"red\", label = \"" << func << "\"];\n";
        num++;

        graph_node(f_node->lhs, file);

        if (!curr) num = 0;
    }

    void graph_flow_node(const node_t *node, std::ofstream &file) {
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

    void connect_node(const node_t *node, std::ofstream& file) {
        switch(node->get_type()) {
            case node_type::BINOP:    connect_bin_node(node, file); break;
            case node_type::FUNCTION:
            case node_type::UNOP:     connect_un_node(node, file); break;
            case node_type::NUMBER:   
            case node_type::VARIABLE: break;
            case node_type::FLOW:     connect_flow_node(node, file);
        }
    }

    void connect_bin_node(const node_t *node, std::ofstream& file) {
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

    void connect_un_node(const node_t *node, std::ofstream& file) {
        const unop_t *u_node = static_cast<const unop_t*>(node);
        int curr = num;

        if (u_node->lhs != nullptr) {
            num++;
            file << "node" << curr << " -> node" << num << ";\n";

            connect_node(u_node->lhs, file);
        }

        if (!curr) num = 0;
    }

    void connect_flow_node(const node_t *node, std::ofstream& file) {
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
};

class tree_t final {

    node_t *root;
    std::vector<node_t*> nodes;
    std::unordered_map<std::string, ast::name_t> variables{};

    int eval(const node_t *node) {
        switch (node->get_type()) {
            case node_type::BINOP   : return evaluate_binop(static_cast<const binop_t*>(node));
            case node_type::UNOP    : return evaluate_unop (static_cast<const unop_t*>(node));
            case node_type::NUMBER  : return evaluate_num  (static_cast<const num_t*>(node));
            case node_type::VARIABLE: return evaluate_var  (static_cast<const variable_t*>(node));
            case node_type::FUNCTION: return evaluate_func (static_cast<const func_t*>(node));
            case node_type::FLOW    : return evaluate_flow (static_cast<const flow_t*>(node));
            default: std::cout << "No such function" << std::endl;
        }
        return 0;
    }

    int evaluate_binop(const binop_t *node) {
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
                break;
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
                break;
            }
            default: std::cout << "No such function" << std::endl;
        }
        return 0;
    }

    int evaluate_unop(const unop_t *node) {
        switch (node->get_operation()) {
            case unop_type::MINUS: return -eval(node->lhs); break;
            default: std::cout << "No such function" << std::endl;
        }
        return 0;
    }

    int evaluate_num(const num_t *node) { return node->get_value(); }

    int evaluate_var(const variable_t *node) {
        auto search = variables.find(node->get_name().name);
        return search->second.value;
    }

    int evaluate_func(const func_t *node) {
        switch (node->get_func()) {
            case func_type::ABS  : return std::abs(eval(node->lhs));
            case func_type::PRINT: std::cout << eval(node->lhs) << std::endl; break;
            default: std::cout << "No such function" << std::endl;
        }
        return 0;
    }

    int evaluate_flow(const flow_t *node) {
        switch (node->get_flow()) {
            case flow_type::IF: {
                if (eval(node->cond)) {
                    eval(node->lhs);
                } else if (node->rhs != nullptr) {
                    eval(node->rhs);
                }
                break;
            }
            case flow_type::WHILE: {
                while (eval(node->cond)) {
                    eval(node->lhs);
                }
                break;
            }
            default: std::cout << "No such function" << std::endl;
        }
        return 0;
    }

    public:

    tree_t(node_t *root_ = nullptr) : root(root_) {}

    tree_t(const tree_t&) = delete;
    tree_t &operator=(const tree_t&) = delete;

    tree_t(tree_t&&) = delete;
    tree_t &operator=(tree_t&&) = delete;

    std::unordered_map<std::string, name_t>::iterator find_variable(const std::string &str) { return variables.find(str); }
    std::unordered_map<std::string, name_t>::iterator variables_end() { return variables.end(); }
    std::pair<std::unordered_map<std::string, name_t>::iterator, bool> add_variable(const std::string &str) { return variables.insert({str, name_t{str, 0}});;}

    template<typename nodeT>
    node_t* ast_insert(nodeT &&node) {
        nodeT *n_node = new nodeT(std::move(node));
        nodes.push_back(n_node);
        root = n_node;
        return n_node;
    }

    int evaluate() {
        return eval(root);
    }

    ~tree_t() {
        for(auto node : nodes) {
            delete node;
        }
    }

    void dump() const {
        std::ofstream file("tree_dump.dot");
        file << "digraph tree {\n";

        tree_dump dump{};
        dump.graph_node(root, file);
        dump.connect_node(root, file);

        file << "}";
        return;
    }

};

} // ast