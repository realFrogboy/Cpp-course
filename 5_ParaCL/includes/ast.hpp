#pragma once

#include <vector>
#include <memory>
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

class binop_t;
class unop_t;
class num_t;
class variable_t;
class func_t;
class flow_t;

struct name_t {
    std::string name;
    double value = 0;
};

class binop_dump final {
    std::string get_binop_operation(const binop_t *node) const;
    public:
    void dump(const binop_t *node, std::ofstream &file, int &num) const;
    void connect(const binop_t *node, std::ofstream &file, int &num) const;
};

class unop_dump final {
    std::string get_unop_operation(const unop_t *node) const;
    public:
    void dump(const unop_t *node, std::ofstream &file, int &num) const;
    void connect(const unop_t *node, std::ofstream &file, int &num) const;
};

class num_dump final {
    public:
    void dump(const num_t *node, std::ofstream &file, int &num) const;
    void connect(const num_t *node, std::ofstream &file, int &num) const;
};

class variable_dump final {
    public:
    void dump(const variable_t *node, std::ofstream &file, int &num) const;
    void connect(const variable_t *node, std::ofstream &file, int &num) const;
};

class func_dump final {
    std::string get_func_d(const func_t *node) const;
    public:
    void dump(const func_t *node, std::ofstream &file, int &num) const;
    void connect(const func_t *node, std::ofstream &file, int &num) const;
};

class flow_dump final {
    std::string get_flow_d(const flow_t *node) const;
    public:
    void dump(const flow_t *node, std::ofstream &file, int &num) const;
    void connect(const flow_t *node, std::ofstream &file, int &num) const;
};

class node_t {
    node_type type;

    public:

    node_t(const node_type t) : type{t} {}

    node_type get_type() const { return type; }

    virtual int eval(std::unordered_map<std::string, ast::name_t> &variables) = 0;

    virtual void graph_node(std::ofstream& file, int &num) = 0;
    virtual void connect_node(std::ofstream& file, int &num) = 0; 

    virtual ~node_t() {}
};


class binop_t final : public node_t {
    binop_type operation;
    std::unique_ptr<binop_dump> dump;

    public:

    node_t *lhs = nullptr;
    node_t *rhs = nullptr;

    binop_t(const binop_type op, std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : 
        node_t{node_type::BINOP}, operation{op}, dump{std::move(dump_)}, lhs{lhs_}, rhs{rhs_} {}

    binop_t(binop_t &&node) : node_t{node_type::BINOP}, dump{std::move(node.dump)} {
        std::swap(lhs, node.lhs);
        std::swap(rhs, node.rhs);
        operation = node.operation;
    }

    binop_type get_operation() const { return operation; }

    int eval(std::unordered_map<std::string, ast::name_t> &variables) override;

    void graph_node(std::ofstream& file, int &num) override {
        dump->dump(this, file, num);
    }
    void connect_node(std::ofstream& file, int &num) override {
        dump->connect(this, file, num);
    }
};

class unop_t final : public node_t {
    unop_type operation;
    std::unique_ptr<unop_dump> dump;

    public:

    node_t *lhs = nullptr;

    unop_t(const unop_type op, std::unique_ptr<unop_dump>dump_, node_t *lhs_) : 
        node_t{node_type::UNOP}, operation{op}, dump{std::move(dump_)}, lhs{lhs_} {}

    unop_t(unop_t &&node) : node_t{node_type::UNOP}, dump{std::move(node.dump)} {
        std::swap(lhs, node.lhs);
        operation = node.operation;
        dump = std::move(node.dump);
    }

    unop_type get_operation() const { return operation; }

    int eval(std::unordered_map<std::string, ast::name_t> &variables) override;

    void graph_node(std::ofstream& file, int &num) override {
        dump->dump(this, file, num);
    }
    void connect_node(std::ofstream& file, int &num) override {
        dump->connect(this, file, num);
    }
};

class num_t final : public node_t {
    int value = 0;
    std::unique_ptr<num_dump> dump;

    public:

    num_t(const int v, std::unique_ptr<num_dump>dump_) : node_t{node_type::NUMBER}, value(v), dump{std::move(dump_)} {}

    num_t(num_t &&node) : node_t{node_type::NUMBER}, value{node.value}, dump{std::move(node.dump)} {}

    int get_value() const { return value; }

    int eval(std::unordered_map<std::string, ast::name_t> &variables) override;

    void graph_node(std::ofstream& file, int &num) override {
        dump->dump(this, file, num);
    }
    void connect_node(std::ofstream& file, int &num) override {
        dump->connect(this, file, num);
    }
};

class variable_t final : public node_t {
    name_t name;
    std::unique_ptr<variable_dump> dump;

    public:

    variable_t(const name_t name_, std::unique_ptr<variable_dump>dump_) : node_t{node_type::VARIABLE}, name(name_), dump{std::move(dump_)} {}

    variable_t(variable_t &&node) : node_t{node_type::VARIABLE}, name{node.name}, dump{std::move(node.dump)} {}

    name_t get_name() const { return name; }

    void set_value(int value) { name.value = value; }

    int eval(std::unordered_map<std::string, ast::name_t> &variables) override;

    void graph_node(std::ofstream& file, int &num) override {
        dump->dump(this, file, num);
    }
    void connect_node(std::ofstream& file, int &num) override {
        dump->connect(this, file, num);
    }
};

class func_t final : public node_t {
    func_type func_id;
    std::unique_ptr<func_dump> dump;

    public :

    node_t *lhs = nullptr;

    func_t(const int func_id_, std::unique_ptr<func_dump>dump_, node_t *lhs_) : 
        node_t{node_type::FUNCTION}, func_id{static_cast<func_type>(func_id_)}, dump{std::move(dump_)}, lhs{lhs_} {}

    func_t(func_t &&node) : node_t{node_type::FUNCTION}, dump{std::move(node.dump)} {
        std::swap(lhs, node.lhs);
        func_id = node.func_id;
    }

    func_type get_func() const { return func_id; }

    int eval(std::unordered_map<std::string, ast::name_t> &variables) override;

    void graph_node(std::ofstream& file, int &num) override {
        dump->dump(this, file, num);
    }
    void connect_node(std::ofstream& file, int &num) override {
        dump->connect(this, file, num);
    }
};

class flow_t final : public node_t {
    flow_type flow_id;
    std::unique_ptr<flow_dump> dump;

    public:

    node_t *cond = nullptr;
    node_t *lhs = nullptr;
    node_t *rhs = nullptr;

    flow_t(const flow_type flow_id_, std::unique_ptr<flow_dump>dump_,  node_t *cond_, node_t *lhs_, node_t *rhs_) :
        node_t{node_type::FLOW}, flow_id{flow_id_}, dump{std::move(dump_)}, cond{cond_}, lhs{lhs_}, rhs{rhs_}  {}

    flow_t(flow_t &&node) : node_t{node_type::FLOW}, dump{std::move(node.dump)} {
        std::swap(cond, node.cond);
        std::swap(lhs, node.lhs);
        std::swap(rhs, node.rhs);
        flow_id = node.flow_id;
    }

    flow_type get_flow() const { return flow_id; }

    int eval(std::unordered_map<std::string, ast::name_t> &variables) override;

    void graph_node(std::ofstream& file, int &num) override {
        dump->dump(this, file, num);
    }
    void connect_node(std::ofstream& file, int &num) override {
        dump->connect(this, file, num);
    }
};

class tree_t final {

    node_t *root;
    std::vector<node_t*> nodes;
    std::unordered_map<std::string, ast::name_t> variables{};

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

    int evaluate() { return root->eval(variables); }

    void dump() const;

    ~tree_t() {
        for(auto node : nodes) {
            delete node;
        }
    }
};

} // ast