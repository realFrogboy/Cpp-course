#pragma once

#include <vector>
#include <memory>
#include <cstring>
#include <unordered_map>

namespace ast {

class binop_t;
class unop_t;
class num_t;
class variable_t;
class func_t;
class flow_t;

struct name_t {
    std::string name;
    int value = 0;
    bool is_init = 0;
};

class binop_dump final {
    std::string operation;
    public:
    binop_dump(const std::string &op) : operation{op} {}
    void dump(const binop_t *node, std::ofstream &file, int &num) const;
    void connect(const binop_t *node, std::ofstream &file, int &num) const;
};

class unop_dump final {
    std::string operation;
    public:
    unop_dump(const std::string &op) : operation{op} {}
    void dump(const unop_t *node, std::ofstream &file, int &num) const;
    void connect(const unop_t *node, std::ofstream &file, int &num) const;
};

struct num_dump final {
    void dump(const num_t *node, std::ofstream &file, int &num) const;
    void connect(const num_t *node, std::ofstream &file, int &num) const;
};

struct variable_dump final {
    void dump(const variable_t *node, std::ofstream &file, int &num) const;
    void connect(const variable_t *node, std::ofstream &file, int &num) const;
};

class func_dump final {
    std::string func;
    public:
    func_dump(const std::string &func_) : func{func_} {}
    void dump(const func_t *node, std::ofstream &file, int &num) const;
    void connect(const func_t *node, std::ofstream &file, int &num) const;
};

class flow_dump final {
    std::string flow;
    public:
    flow_dump(const std::string &flow_) : flow{flow_} {}
    void dump(const flow_t *node, std::ofstream &file, int &num) const;
    void connect(const flow_t *node, std::ofstream &file, int &num) const;
};

struct node_t {
    virtual int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const = 0;
    virtual void graph_node(std::ofstream &file, int &num) const = 0;
    virtual void connect_node(std::ofstream &file, int &num) const = 0; 
    virtual ~node_t() {}
};


class binop_t : public node_t {
    std::unique_ptr<binop_dump> dump;

    public:

    node_t *lhs = nullptr;
    node_t *rhs = nullptr;

    binop_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : dump{std::move(dump_)}, lhs{lhs_}, rhs{rhs_} {}

    binop_t(binop_t &&node) : dump{std::move(node.dump)} {
        std::swap(lhs, node.lhs);
        std::swap(rhs, node.rhs);
    }

    void graph_node(std::ofstream &file, int &num) const override {
        dump->dump(this, file, num);
    }
    void connect_node(std::ofstream &file, int &num) const override {
        dump->connect(this, file, num);
    }
};

struct g_t final : binop_t {
    g_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override { return (lhs->eval(variables) > rhs->eval(variables)); }
};

struct l_t final : binop_t {
    l_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override { return (lhs->eval(variables) < rhs->eval(variables)); }
};

struct e_t final : binop_t {
    e_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override { return (lhs->eval(variables) == rhs->eval(variables)); }
};

struct ne_t final : binop_t {
    ne_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override { return (lhs->eval(variables) != rhs->eval(variables)); }
};

struct ge_t final : binop_t {
    ge_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override { return (lhs->eval(variables) >= rhs->eval(variables)); }
};

struct le_t final : binop_t {
    le_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override { return (lhs->eval(variables) <= rhs->eval(variables)); }
};

struct add_t final : binop_t {
    add_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override { return (lhs->eval(variables) + rhs->eval(variables)); }
};

struct sub_t final : binop_t {
    sub_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override { return (lhs->eval(variables) - rhs->eval(variables)); }
};

struct mul_t final : binop_t {
    mul_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override { return (lhs->eval(variables) * rhs->eval(variables)); }
};

struct div_t final : binop_t {
    div_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override { return (lhs->eval(variables) / rhs->eval(variables)); }
};

struct remainder_t final : binop_t {
    remainder_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override { return (lhs->eval(variables) % rhs->eval(variables)); }
};

struct pow_t final : binop_t {
    pow_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override;
};

struct b_and_t final : binop_t {
    b_and_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override { return (lhs->eval(variables) & rhs->eval(variables)); }
};

struct b_or_t final : binop_t {
    b_or_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override { return (lhs->eval(variables) | rhs->eval(variables)); }
};

struct xor_t final : binop_t {
    xor_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override { return (lhs->eval(variables) ^ rhs->eval(variables)); }
};

struct l_shift_t final : binop_t {
    l_shift_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override { return (lhs->eval(variables) << rhs->eval(variables)); }
};

struct r_shift_t final : binop_t {
    r_shift_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override { return (lhs->eval(variables) >> rhs->eval(variables)); }
};

struct l_and_t final : binop_t {
    l_and_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override { return (lhs->eval(variables) && rhs->eval(variables)); }
};

struct l_or_t final : binop_t {
    l_or_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override { return (lhs->eval(variables) || rhs->eval(variables)); }
};

struct assign_t final : binop_t {
    assign_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override;
};

struct scolon_t final : binop_t {
    scolon_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override;
};

class unop_t : public node_t {
    std::unique_ptr<unop_dump> dump;

    public:

    node_t *lhs = nullptr;

    unop_t(std::unique_ptr<unop_dump>dump_, node_t *lhs_) : dump{std::move(dump_)}, lhs{lhs_} {}

    unop_t(unop_t &&node) : dump{std::move(node.dump)} {
        std::swap(lhs, node.lhs);
    }

    void graph_node(std::ofstream &file, int &num) const override {
        dump->dump(this, file, num);
    }
    void connect_node(std::ofstream &file, int &num) const override {
        dump->connect(this, file, num);
    }
};

struct minus_t final : unop_t {
    minus_t(std::unique_ptr<unop_dump>dump_, node_t *lhs_) : unop_t{std::move(dump_), lhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override { return -lhs->eval(variables); }
};

struct plus_t final : unop_t {
    plus_t(std::unique_ptr<unop_dump>dump_, node_t *lhs_) : unop_t{std::move(dump_), lhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override { return lhs->eval(variables); }
};

struct not_t final : unop_t {
    not_t(std::unique_ptr<unop_dump>dump_, node_t *lhs_) : unop_t{std::move(dump_), lhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override { return !lhs->eval(variables); }
};

struct b_not_t final : unop_t {
    b_not_t(std::unique_ptr<unop_dump>dump_, node_t *lhs_) : unop_t{std::move(dump_), lhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override { return ~lhs->eval(variables); }
};

struct pr_increment_t final : unop_t {
    pr_increment_t(std::unique_ptr<unop_dump>dump_, node_t *lhs_) : unop_t{std::move(dump_), lhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override;
};

struct pr_decrement_t final : unop_t {
    pr_decrement_t(std::unique_ptr<unop_dump>dump_, node_t *lhs_) : unop_t{std::move(dump_), lhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override;
};

class num_t final : public node_t {
    int value = 0;
    std::unique_ptr<num_dump> dump;

    public:

    num_t(const int v, std::unique_ptr<num_dump>dump_) : value{v}, dump{std::move(dump_)} {}

    num_t(num_t &&node) : value{node.value}, dump{std::move(node.dump)} {}

    int get_value() const { return value; }

    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override { return value; }

    void graph_node(std::ofstream &file, int &num) const override {
        dump->dump(this, file, num);
    }
    void connect_node(std::ofstream &file, int &num) const override {
        dump->connect(this, file, num);
    }
};

class variable_t final : public node_t {
    name_t name;
    std::unique_ptr<variable_dump> dump;

    public:

    variable_t(const name_t name_, std::unique_ptr<variable_dump>dump_) : name{name_}, dump{std::move(dump_)} {}

    variable_t(variable_t &&node) : name{node.name}, dump{std::move(node.dump)} {}

    std::string get_name() const { return name.name; }

    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override;

    void graph_node(std::ofstream &file, int &num) const override {
        dump->dump(this, file, num);
    }
    void connect_node(std::ofstream &file, int &num) const override {
        dump->connect(this, file, num);
    }
};

class func_t : public node_t {
    std::unique_ptr<func_dump> dump;

    public :

    node_t *lhs = nullptr;

    func_t(std::unique_ptr<func_dump>dump_, node_t *lhs_) : dump{std::move(dump_)}, lhs{lhs_} {}

    func_t(func_t &&node) : dump{std::move(node.dump)} {
        std::swap(lhs, node.lhs);
    }

    void graph_node(std::ofstream &file, int &num) const override {
        dump->dump(this, file, num);
    }
    void connect_node(std::ofstream &file, int &num) const override {
        dump->connect(this, file, num);
    }
};

struct print_t final : func_t {
    print_t(std::unique_ptr<func_dump>dump_, node_t *lhs_) : func_t{std::move(dump_), lhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override;
};

struct abs_t final : func_t {
    abs_t(std::unique_ptr<func_dump>dump_, node_t *lhs_) : func_t{std::move(dump_), lhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override { return std::abs(lhs->eval(variables)); }
};

struct get_t final : func_t {
    get_t(std::unique_ptr<func_dump>dump_) : func_t{std::move(dump_), nullptr} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override;
};

class flow_t : public node_t {
    std::unique_ptr<flow_dump> dump;

    public:

    node_t *cond = nullptr;
    node_t *lhs = nullptr;
    node_t *rhs = nullptr;

    flow_t(std::unique_ptr<flow_dump>dump_,  node_t *cond_, node_t *lhs_, node_t *rhs_) : dump{std::move(dump_)}, cond{cond_}, lhs{lhs_}, rhs{rhs_} {}

    flow_t(flow_t &&node) : dump{std::move(node.dump)} {
        std::swap(cond, node.cond);
        std::swap(lhs, node.lhs);
        std::swap(rhs, node.rhs);
    }

    void graph_node(std::ofstream &file, int &num) const override {
        dump->dump(this, file, num);
    }
    void connect_node(std::ofstream &file, int &num) const override {
        dump->connect(this, file, num);
    }
};

struct if_t final : flow_t {
    if_t(std::unique_ptr<flow_dump>dump_, node_t *cond_, node_t *lhs_, node_t *rhs_) : flow_t{std::move(dump_), cond_, lhs_, rhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override;
};

struct while_t final : flow_t {
    while_t(std::unique_ptr<flow_dump>dump_, node_t *cond_, node_t *lhs_, node_t *rhs_) : flow_t{std::move(dump_), cond_, lhs_, rhs_} {}
    int eval(std::vector<std::unordered_map<std::string, ast::name_t>> &variables) const override;
};

class tree_t final {

    node_t *root;
    std::vector<node_t*> nodes;
    std::vector<std::unordered_map<std::string, ast::name_t>> variables{};

    public:

    tree_t(node_t *root_ = nullptr) : root{root_} {
        std::unordered_map<std::string, ast::name_t> global;
        variables.push_back(global);
    }

    tree_t(const tree_t&) = delete;
    tree_t &operator=(const tree_t&) = delete;

    tree_t(tree_t&&) = delete;
    tree_t &operator=(tree_t&&) = delete;

    bool is_valid() const {
        if (root) return 1;
        else return 0;
    }

    template<typename nodeT>
    node_t* ast_insert(nodeT &node) {
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