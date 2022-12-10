#pragma once

#include <vector>
#include <unordered_map>

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
    SCAN,
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

    num_t(num_t &&node) : node_t{node_type::NUMBER}, value{node.value} {}

    int get_value() const { return value; }
};

class variable_t final : public node_t {
    name_t name;

    public:

    variable_t(name_t name_) : node_t{node_type::VARIABLE}, name(name_) {}

    variable_t(variable_t &&node) : node_t{node_type::VARIABLE}, name{node.name} {}

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

    static std::string get_binop_operation(const binop_t *node);

    static std::string get_unop_operation(const unop_t *node);

    static std::string get_func(const func_t *node);

    static std::string get_flow(const flow_t *node);

    void graph_binop_node(const node_t *node, std::ofstream &file);

    void graph_unop_node(const node_t *node, std::ofstream &file);

    void graph_num_node(const node_t *node, std::ofstream &file);

    void graph_var_node(const node_t *node, std::ofstream &file);

    void graph_func_node(const node_t *node, std::ofstream &file);

    void graph_flow_node(const node_t *node, std::ofstream &file);

    void connect_bin_node(const node_t *node, std::ofstream& file);

    void connect_un_node(const node_t *node, std::ofstream& file);

    void connect_flow_node(const node_t *node, std::ofstream& file);

    public:

    void graph_node(const node_t *node, std::ofstream& file);

    void connect_node(const node_t *node, std::ofstream& file);
};

class tree_t final {

    node_t *root;
    std::vector<node_t*> nodes;
    std::unordered_map<std::string, ast::name_t> variables{};

    int eval(const node_t *node);

    int evaluate_binop(const binop_t *node);

    int evaluate_unop(const unop_t *node);

    int evaluate_num(const num_t *node) { return node->get_value(); }

    int evaluate_var(const variable_t *node);

    int evaluate_func(const func_t *node);

    int evaluate_flow(const flow_t *node);

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

    int evaluate() { return eval(root); }

    void dump() const;

    ~tree_t() {
        for(auto node : nodes) {
            delete node;
        }
    }
};

} // ast