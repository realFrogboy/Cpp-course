#pragma once

#include <vector>
#include <memory>
#include <cstring>
#include <type_traits>
#include <unordered_map>

namespace ast {

class node_t;

struct name_t {
    std::string name;
    int value = 0;
    bool is_init = 0;
};

using scope_t  = std::unordered_map<std::string, ast::name_t>;
using scopes_t = std::vector<scope_t>;

class Dump final {
    std::string msg;
    public:
    Dump(const std::string &msg_) : msg{msg_} {}
    void dump(const node_t *node, std::ofstream &file, int &num) const;
    void connect(const node_t *node, std::ofstream &file, int &num) const;
};

struct node_t {
    protected:
    std::unique_ptr<Dump> dump;
    scopes_t &scopes;

    public:
    node_t *lhs = nullptr;
    node_t *rhs = nullptr;
    node_t *cond = nullptr;

    node_t(scopes_t &scopes_, const std::string &msg_, node_t *lhs_ = nullptr, node_t *rhs_ = nullptr, node_t *cond_ = nullptr) : 
            dump{std::make_unique<Dump>(msg_)}, scopes{scopes_}, lhs{lhs_}, rhs{rhs_}, cond{cond_} {}
    virtual int eval() const = 0;
    void graph_node(std::ofstream &file, int &num) const {
        dump->dump(this, file, num);
    }
    void connect_node(std::ofstream &file, int &num) const {
        dump->connect(this, file, num);
    } 
    virtual ~node_t() {}

    scopes_t::reverse_iterator find_variable(const std::string &name) const {
        auto scope = std::find_if(scopes.rbegin(), scopes.rend(), [&name](scope_t &scope) {
            auto search = scope.find(name);
            return search != scope.end();
        });
        return scope;
    }
};

struct g_t final : node_t {
    g_t(scopes_t &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, ">", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() > rhs->eval()); }
};

struct l_t final : node_t {
    l_t(scopes_t &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "<", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() < rhs->eval()); }
};

struct e_t final : node_t {
    e_t(scopes_t &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "==", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() == rhs->eval()); }
};

struct ne_t final : node_t {
    ne_t(scopes_t &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "!=", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() != rhs->eval()); }
};

struct ge_t final : node_t {
    ge_t(scopes_t &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, ">=", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() >= rhs->eval()); }
};

struct le_t final : node_t {
    le_t(scopes_t &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "<=", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() <= rhs->eval()); }
};

struct add_t final : node_t {
    add_t(scopes_t &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "+", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() + rhs->eval()); }
};

struct sub_t final : node_t {
    sub_t(scopes_t &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "-", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() - rhs->eval()); }
};

struct mul_t final : node_t {
    mul_t(scopes_t &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "*", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() * rhs->eval()); }
};

struct div_t final : node_t {
    div_t(scopes_t &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "/", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() / rhs->eval()); }
};

struct remainder_t final : node_t {
    remainder_t(scopes_t &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "%", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() % rhs->eval()); }
};

struct pow_t final : node_t {
    pow_t(scopes_t &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "**", lhs_, rhs_} {}
    int eval() const override;
};

struct b_and_t final : node_t {
    b_and_t(scopes_t &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "&", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() & rhs->eval()); }
};

struct b_or_t final : node_t {
    b_or_t(scopes_t &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "|", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() | rhs->eval()); }
};

struct xor_t final : node_t {
    xor_t(scopes_t &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "^", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() ^ rhs->eval()); }
};

struct l_shift_t final : node_t {
    l_shift_t(scopes_t &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "<<", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() << rhs->eval()); }
};

struct r_shift_t final : node_t {
    r_shift_t(scopes_t &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, ">>", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() >> rhs->eval()); }
};

struct l_and_t final : node_t {
    l_and_t(scopes_t &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "&&", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() && rhs->eval()); }
};

struct l_or_t final : node_t {
    l_or_t(scopes_t &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "||", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() || rhs->eval()); }
};

struct assign_t final : node_t {
    assign_t(scopes_t &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "=", lhs_, rhs_} {}
    int eval() const override;
};

struct scolon_t final : node_t {
    scolon_t(scopes_t &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, ";", lhs_, rhs_} {}
    int eval() const override;
};

struct minus_t final : node_t {
    minus_t(scopes_t &scopes_, node_t *lhs_, node_t* = nullptr, node_t* = nullptr) : node_t{scopes_, "-", lhs_} {}
    int eval() const override { return -lhs->eval(); }
};

struct plus_t final : node_t {
    plus_t(scopes_t &scopes_, node_t *lhs_, node_t* = nullptr, node_t* = nullptr) : node_t{scopes_, "+", lhs_} {}
    int eval() const override { return lhs->eval(); }
};

struct not_t final : node_t {
    not_t(scopes_t &scopes_, node_t *lhs_, node_t* = nullptr, node_t* = nullptr) : node_t{scopes_, "!", lhs_} {}
    int eval() const override { return !lhs->eval(); }
};

struct b_not_t final : node_t {
    b_not_t(scopes_t &scopes_, node_t *lhs_, node_t* = nullptr, node_t* = nullptr) : node_t{scopes_, "~", lhs_} {}
    int eval() const override { return ~lhs->eval(); }
};

struct pr_increment_t final : node_t {
    pr_increment_t(scopes_t &scopes_, node_t *lhs_, node_t* = nullptr, node_t* = nullptr) : node_t{scopes_, "++", lhs_} {}
    int eval() const override;
};

struct pr_decrement_t final : node_t {
    pr_decrement_t(scopes_t &scopes_, node_t *lhs_, node_t* = nullptr, node_t* = nullptr) : node_t{scopes_, "--", lhs_} {}
    int eval() const override;
};

class num_t final : public node_t {
    int value = 0;

    public:

    num_t(scopes_t &scopes_, const int v) : node_t{scopes_, std::to_string(v)}, value{v} {}

    int get_value() const { return value; }
    int eval() const override { return value; }
};

class variable_t final : public node_t {
    std::string name;

    public:

    variable_t(scopes_t &scopes_, const std::string &name_) : node_t{scopes_, name_}, name{name_} {}

    std::string get_name() const { return name; }
    int eval() const override;
};

struct print_t final : node_t {
    print_t(scopes_t &scopes_, node_t *lhs_, node_t* = nullptr, node_t* = nullptr) : node_t{scopes_, "print", lhs_} {}
    int eval() const override;
};

struct abs_t final : node_t {
    abs_t(scopes_t &scopes_, node_t *lhs_, node_t* = nullptr, node_t* = nullptr) : node_t{scopes_, "abs", lhs_} {}
    int eval() const override { return std::abs(lhs->eval()); }
};

struct get_t final : node_t {
    get_t(scopes_t &scopes_, node_t* = nullptr, node_t* = nullptr, node_t* = nullptr) : node_t{scopes_, "?", nullptr} {}
    int eval() const override;
};

struct if_t final : node_t {
    if_t(scopes_t &scopes_, node_t *lhs_, node_t *rhs_, node_t *cond_) : node_t{scopes_, "if", lhs_, rhs_, cond_} {}
    int eval() const override;
};

struct while_t final : node_t {
    while_t(scopes_t &scopes_, node_t *lhs_, node_t *rhs_, node_t *cond_) : node_t{scopes_, "while", lhs_, rhs_, cond_} {}
    int eval() const override;
};

class tree_t final {
    node_t *root;
    std::vector<std::unique_ptr<node_t>> nodes;
    scopes_t scopes{};

    public:

    tree_t(node_t *root_ = nullptr) : root{root_} {
        scopes.push_back({});
    }

    tree_t(const tree_t&) = delete;
    tree_t &operator=(const tree_t&) = delete;

    tree_t(tree_t&&) = delete;
    tree_t &operator=(tree_t&&) = delete;

    bool is_valid() const {
        if (root) return 1;
        else return 0;
    }

    template <typename nodeT>
    node_t* ast_insert(node_t *lhs = nullptr, node_t *rhs = nullptr, node_t *cond = nullptr) {
        nodes.emplace_back(std::make_unique<nodeT>(scopes, lhs, rhs, cond));
        root = nodes.back().get();
        return root;
    }

    node_t* ast_insert(const int val) {
        nodes.emplace_back(std::make_unique<num_t>(scopes, val));
        root = nodes.back().get();
        return root;
    }

    node_t* ast_insert(const std::string &var) {
        nodes.emplace_back(std::make_unique<variable_t>(scopes, var));
        root = nodes.back().get();
        return root;
    }

    int evaluate() { return root->eval(); }

    void dump() const;
};

} // ast