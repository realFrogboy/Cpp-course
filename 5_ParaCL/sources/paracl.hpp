#pragma once

#include <string>
#include <fstream>

enum node_type {
    BINOP,
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
    int value = 0;
};

class node_t {
    node_type type;

    public:

    node_t(node_type t) : type{t} {}

    node_type get_type() const { return type; }

    virtual ~node_t() {}
};


class binop_t : public node_t {
    binop_type operation;

    public:

    node_t *lhs = nullptr;
    node_t *rhs = nullptr;

    binop_t(binop_type op, node_t *lhs_, node_t *rhs_) : node_t{node_type::BINOP}, operation{op}, lhs{lhs_}, rhs{rhs_} {}

    binop_type get_operation() const { return operation; }
};

class unop_t : public node_t {
    unop_type operation;

    public:

    node_t *lhs = nullptr;

    unop_t(unop_type op, node_t *lhs_) : node_t{node_type::UNOP}, operation{op}, lhs{lhs_} {}

    unop_type get_operation() const { return operation; }
};

class num_t : public node_t {
    int value = 0;

    public:

    num_t(int v) : node_t{node_type::NUMBER}, value(v) {}

    int get_value() const { return value; }
};

class variable_t : public node_t {
    name_t name;

    public:

    variable_t(name_t name_) : node_t{node_type::VARIABLE}, name(name_) {}

    name_t get_name() const { return name; }
};

class func_t : public node_t {
    func_type func_id;

    public :

    node_t *lhs = nullptr;

    func_t(int func_id_, node_t *lhs_) : node_t{node_type::FUNCTION}, func_id{static_cast<func_type>(func_id_)}, lhs{lhs_} {}

    func_type get_func() const { return func_id; }
};

class flow_t : public node_t {
    flow_type flow_id;

    public:

    node_t *cond = nullptr;
    node_t *lhs = nullptr;
    node_t *rhs = nullptr;

    flow_t(flow_type flow_id_, node_t *cond_, node_t *lhs_, node_t *rhs_) : node_t{node_type::FLOW}, flow_id{flow_id_}, cond{cond_}, lhs{lhs_}, rhs{rhs_}  {}

    flow_type get_flow() const { return flow_id; }
};

class tree_dump {

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

class tree_t {

    node_t *root;

    public:

    tree_t(node_t *root_) : root(root_) {}

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