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
    binop_dump(const std::string op) : operation{op} {}
    void dump(const binop_t *node, std::ofstream &file, int &num) const;
    void connect(const binop_t *node, std::ofstream &file, int &num) const;
};

class unop_dump final {
    std::string operation;
    public:
    unop_dump(const std::string op) : operation{op} {}
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
    std::string func;
    public:
    func_dump(const std::string func_) : func{func_} {}
    void dump(const func_t *node, std::ofstream &file, int &num) const;
    void connect(const func_t *node, std::ofstream &file, int &num) const;
};

class flow_dump final {
    std::string flow;
    public:
    flow_dump(const std::string flow_) : flow{flow_} {}
    void dump(const flow_t *node, std::ofstream &file, int &num) const;
    void connect(const flow_t *node, std::ofstream &file, int &num) const;
};

class node_t {
    public:
    virtual int eval(std::unordered_map<std::string, ast::name_t> &variables) = 0;
    virtual void graph_node(std::ofstream& file, int &num) = 0;
    virtual void connect_node(std::ofstream& file, int &num) = 0; 
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

    void graph_node(std::ofstream& file, int &num) override {
        dump->dump(this, file, num);
    }
    void connect_node(std::ofstream& file, int &num) override {
        dump->connect(this, file, num);
    }
};

class g_t final : public binop_t {
    public:
    g_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override { return (lhs->eval(variables) > rhs->eval(variables)); }
};

class l_t final : public binop_t {
    public:
    l_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override { return (lhs->eval(variables) < rhs->eval(variables)); }
};

class e_t final : public binop_t {
    public:
    e_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override { return (lhs->eval(variables) == rhs->eval(variables)); }
};

class ne_t final : public binop_t {
    public:
    ne_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override { return (lhs->eval(variables) != rhs->eval(variables)); }
};

class ge_t final : public binop_t {
    public:
    ge_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override { return (lhs->eval(variables) >= rhs->eval(variables)); }
};

class le_t final : public binop_t {
    public:
    le_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override { return (lhs->eval(variables) <= rhs->eval(variables)); }
};

class add_t final : public binop_t {
    public:
    add_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override { return (lhs->eval(variables) + rhs->eval(variables)); }
};

class sub_t final : public binop_t {
    public:
    sub_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override { return (lhs->eval(variables) - rhs->eval(variables)); }
};

class mul_t final : public binop_t {
    public:
    mul_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override { return (lhs->eval(variables) * rhs->eval(variables)); }
};

class div_t final : public binop_t {
    public:
    div_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override { return (lhs->eval(variables) / rhs->eval(variables)); }
};

class remainder_t final : public binop_t {
    public:
    remainder_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override { return (lhs->eval(variables) % rhs->eval(variables)); }
};

class pow_t final : public binop_t {
    public:
    pow_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override;
};

class b_and_t final : public binop_t {
    public:
    b_and_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override { return (lhs->eval(variables) & rhs->eval(variables)); }
};

class b_or_t final : public binop_t {
    public:
    b_or_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override { return (lhs->eval(variables) | rhs->eval(variables)); }
};

class xor_t final : public binop_t {
    public:
    xor_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override { return (lhs->eval(variables) ^ rhs->eval(variables)); }
};

class l_shift_t final : public binop_t {
    public:
    l_shift_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override { return (lhs->eval(variables) << rhs->eval(variables)); }
};

class r_shift_t final : public binop_t {
    public:
    r_shift_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override { return (lhs->eval(variables) >> rhs->eval(variables)); }
};

class l_and_t final : public binop_t {
    public:
    l_and_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override { return (lhs->eval(variables) && rhs->eval(variables)); }
};

class l_or_t final : public binop_t {
    public:
    l_or_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override { return (lhs->eval(variables) || rhs->eval(variables)); }
};

class assign_t final : public binop_t {
    public:
    assign_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override;
};

class add_a_t final : public binop_t {
    public:
    add_a_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override;
};

class sub_a_t final : public binop_t {
    public:
    sub_a_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override;
};

class mult_a_t final : public binop_t {
    public:
    mult_a_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override;
};

class div_a_t final : public binop_t {
    public:
    div_a_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override;
};

class remainder_a_t final : public binop_t {
    public:
    remainder_a_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override;
};

class l_shift_a_t final : public binop_t {
    public:
    l_shift_a_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override;
};

class r_shift_a_t final : public binop_t {
    public:
    r_shift_a_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override;
};

class b_and_a_t final : public binop_t {
    public:
    b_and_a_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override;
};

class b_or_a_t final : public binop_t {
    public:
    b_or_a_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override;
};

class xor_a_t final : public binop_t {
    public:
    xor_a_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override;
};

class scolon_t final : public binop_t {
    public:
    scolon_t(std::unique_ptr<binop_dump>dump_, node_t *lhs_, node_t *rhs_) : binop_t{std::move(dump_), lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override;
};

class unop_t : public node_t {
    std::unique_ptr<unop_dump> dump;

    public:

    node_t *lhs = nullptr;

    unop_t(std::unique_ptr<unop_dump>dump_, node_t *lhs_) : dump{std::move(dump_)}, lhs{lhs_} {}

    unop_t(unop_t &&node) : dump{std::move(node.dump)} {
        std::swap(lhs, node.lhs);
    }

    void graph_node(std::ofstream& file, int &num) override {
        dump->dump(this, file, num);
    }
    void connect_node(std::ofstream& file, int &num) override {
        dump->connect(this, file, num);
    }
};

class minus_t final : public unop_t {
    public:
    minus_t(std::unique_ptr<unop_dump>dump_, node_t *lhs_) : unop_t{std::move(dump_), lhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override { return -lhs->eval(variables); }
};

class plus_t final : public unop_t {
    public:
    plus_t(std::unique_ptr<unop_dump>dump_, node_t *lhs_) : unop_t{std::move(dump_), lhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override { return lhs->eval(variables); }
};

class not_t final : public unop_t {
    public:
    not_t(std::unique_ptr<unop_dump>dump_, node_t *lhs_) : unop_t{std::move(dump_), lhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override { return !lhs->eval(variables); }
};

class b_not_t final : public unop_t {
    public:
    b_not_t(std::unique_ptr<unop_dump>dump_, node_t *lhs_) : unop_t{std::move(dump_), lhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override { return ~lhs->eval(variables); }
};

class pr_increment_t final : public unop_t {
    public:
    pr_increment_t(std::unique_ptr<unop_dump>dump_, node_t *lhs_) : unop_t{std::move(dump_), lhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override;
};

class pr_decrement_t final : public unop_t {
    public:
    pr_decrement_t(std::unique_ptr<unop_dump>dump_, node_t *lhs_) : unop_t{std::move(dump_), lhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override;
};

class num_t final : public node_t {
    int value = 0;
    std::unique_ptr<num_dump> dump;

    public:

    num_t(const int v, std::unique_ptr<num_dump>dump_) : value(v), dump{std::move(dump_)} {}

    num_t(num_t &&node) : value{node.value}, dump{std::move(node.dump)} {}

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

    variable_t(const name_t name_, std::unique_ptr<variable_dump>dump_) : name{name_}, dump{std::move(dump_)} {}

    variable_t(variable_t &&node) : name{node.name}, dump{std::move(node.dump)} {}

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

class func_t : public node_t {
    std::unique_ptr<func_dump> dump;

    public :

    node_t *lhs = nullptr;

    func_t(std::unique_ptr<func_dump>dump_, node_t *lhs_) : dump{std::move(dump_)}, lhs{lhs_} {}

    func_t(func_t &&node) : dump{std::move(node.dump)} {
        std::swap(lhs, node.lhs);
    }

    void graph_node(std::ofstream& file, int &num) override {
        dump->dump(this, file, num);
    }
    void connect_node(std::ofstream& file, int &num) override {
        dump->connect(this, file, num);
    }
};

class print_t final : public func_t {
    public:
    print_t(std::unique_ptr<func_dump>dump_, node_t *lhs_) : func_t{std::move(dump_), lhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override;
};

class scan_t final : public func_t {
    public:
    scan_t(std::unique_ptr<func_dump>dump_, node_t *lhs_) : func_t{std::move(dump_), lhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override;
};

class abs_t final : public func_t {
    public:
    abs_t(std::unique_ptr<func_dump>dump_, node_t *lhs_) : func_t{std::move(dump_), lhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override { return std::abs(lhs->eval(variables)); }
};

class get_t final : public func_t {
    public:
    get_t(std::unique_ptr<func_dump>dump_) : func_t{std::move(dump_), nullptr} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override;
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

    void graph_node(std::ofstream& file, int &num) override {
        dump->dump(this, file, num);
    }
    void connect_node(std::ofstream& file, int &num) override {
        dump->connect(this, file, num);
    }
};

class if_t final : public flow_t {
    public:
    if_t(std::unique_ptr<flow_dump>dump_, node_t *cond_, node_t *lhs_, node_t *rhs_) : flow_t{std::move(dump_), cond_, lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override;
};

class while_t final : public flow_t {
    public:
    while_t(std::unique_ptr<flow_dump>dump_, node_t *cond_, node_t *lhs_, node_t *rhs_) : flow_t{std::move(dump_), cond_, lhs_, rhs_} {}
    int eval(std::unordered_map<std::string, ast::name_t> &variables) override;
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

    bool is_valid() {
        if (root) return 1;
        else return 0;
    }

    std::unordered_map<std::string, name_t>::iterator find_variable(const std::string &str) { return variables.find(str); }
    std::unordered_map<std::string, name_t>::iterator variables_end() { return variables.end(); }
    std::pair<std::unordered_map<std::string, name_t>::iterator, bool> add_variable(const std::string &str) { return variables.insert({str, name_t{str, 0, 0}});;}

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