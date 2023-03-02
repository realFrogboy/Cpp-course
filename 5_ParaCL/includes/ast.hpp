#pragma once

#include <vector>
#include <memory>
#include <variant>
#include <unordered_map>

namespace ast {

class node_t;

struct func_info {
    node_t *root;
    std::vector<std::string> signature;
};

struct name_t {
    std::string name;
    std::variant<int, func_info> value;
    bool is_init = 0;
};

using scope_t  = std::unordered_map<std::string, ast::name_t>;
using scopes_t = std::vector<scope_t>;

class Scopes final {
    scopes_t scopes{};
    scopes_t hiden_scopes{};
    std::vector<int> arg_list{};

    public:
    void open_scope() { scopes.push_back({}); }
    void close_scope() { scopes.pop_back(); }
    void hide_scopes() { hiden_scopes.swap(scopes); scopes.push_back({}); }
    void recover_scopes() { scopes.swap(hiden_scopes); hiden_scopes.clear(); }
    void add_arg_list(const int val) { arg_list.push_back(val); }

    ast::name_t* find_variable(const std::string &name) {
        auto scope = std::find_if(scopes.rbegin(), scopes.rend(), [&name](scope_t &scope) {
            auto search = scope.find(name);
            return search != scope.end();
        });
        if (scope == scopes.rend()) return nullptr;
        auto search = scope->find(name);
        return &(search->second);
    }

    ast::name_t* add_variable(const std::string &name) {
        return &(scopes.back().insert({name, name_t{name, 0, 0}}).first->second);
    }

    ast::name_t* add_init_variable(const std::string &name, const int val) {
        return &(scopes.back().insert({name, name_t{name, val, 1}}).first->second);
    }

    void init_func_args(const std::vector<std::string> &signature) {
        for (unsigned idx = 0, sz = signature.size(); idx < sz; ++idx)
            scopes.back().insert({signature[idx], name_t{signature[idx], arg_list[sz - idx - 1], 1}});
        arg_list.clear();
    }
};

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
    Scopes &scopes;

    public:
    node_t *lhs = nullptr;
    node_t *rhs = nullptr;
    node_t *cond = nullptr;

    node_t(Scopes &scopes_, const std::string &msg_, node_t *lhs_ = nullptr, node_t *rhs_ = nullptr, node_t *cond_ = nullptr) : 
            dump{std::make_unique<Dump>(msg_)}, scopes{scopes_}, lhs{lhs_}, rhs{rhs_}, cond{cond_} {}
    virtual int eval() const = 0;
    virtual ~node_t() {}

    void graph_node(std::ofstream &file, int &num) const { dump->dump(this, file, num); }
    void connect_node(std::ofstream &file, int &num) const { dump->connect(this, file, num); } 
};

struct g_t final : node_t {
    g_t(Scopes &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, ">", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() > rhs->eval()); }
};

struct l_t final : node_t {
    l_t(Scopes &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "<", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() < rhs->eval()); }
};

struct e_t final : node_t {
    e_t(Scopes &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "==", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() == rhs->eval()); }
};

struct ne_t final : node_t {
    ne_t(Scopes &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "!=", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() != rhs->eval()); }
};

struct ge_t final : node_t {
    ge_t(Scopes &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, ">=", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() >= rhs->eval()); }
};

struct le_t final : node_t {
    le_t(Scopes &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "<=", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() <= rhs->eval()); }
};

struct add_t final : node_t {
    add_t(Scopes &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "+", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() + rhs->eval()); }
};

struct sub_t final : node_t {
    sub_t(Scopes &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "-", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() - rhs->eval()); }
};

struct mul_t final : node_t {
    mul_t(Scopes &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "*", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() * rhs->eval()); }
};

struct div_t final : node_t {
    div_t(Scopes &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "/", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() / rhs->eval()); }
};

struct remainder_t final : node_t {
    remainder_t(Scopes &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "%", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() % rhs->eval()); }
};

struct pow_t final : node_t {
    pow_t(Scopes &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "**", lhs_, rhs_} {}
    int eval() const override;
};

struct b_and_t final : node_t {
    b_and_t(Scopes &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "&", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() & rhs->eval()); }
};

struct b_or_t final : node_t {
    b_or_t(Scopes &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "|", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() | rhs->eval()); }
};

struct xor_t final : node_t {
    xor_t(Scopes &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "^", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() ^ rhs->eval()); }
};

struct l_shift_t final : node_t {
    l_shift_t(Scopes &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "<<", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() << rhs->eval()); }
};

struct r_shift_t final : node_t {
    r_shift_t(Scopes &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, ">>", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() >> rhs->eval()); }
};

struct l_and_t final : node_t {
    l_and_t(Scopes &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "&&", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() && rhs->eval()); }
};

struct l_or_t final : node_t {
    l_or_t(Scopes &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "||", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() || rhs->eval()); }
};

struct assign_t final : node_t {
    assign_t(Scopes &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, "=", lhs_, rhs_} {}
    int eval() const override;
};

class func_assign_t final : public node_t {
    func_info info;
    public:
    func_assign_t(Scopes &scopes_, node_t *lhs_, const func_info &info_) : node_t{scopes_, "=", lhs_, info_.root}, info{info_} {}
    int eval() const override;
};

struct scolon_t final : node_t {
    scolon_t(Scopes &scopes_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{scopes_, ";", lhs_, rhs_} {}
    int eval() const override;
};

struct minus_t final : node_t {
    minus_t(Scopes &scopes_, node_t *lhs_, node_t* = nullptr, node_t* = nullptr) : node_t{scopes_, "-", lhs_} {}
    int eval() const override { return -lhs->eval(); }
};

struct plus_t final : node_t {
    plus_t(Scopes &scopes_, node_t *lhs_, node_t* = nullptr, node_t* = nullptr) : node_t{scopes_, "+", lhs_} {}
    int eval() const override { return lhs->eval(); }
};

struct not_t final : node_t {
    not_t(Scopes &scopes_, node_t *lhs_, node_t* = nullptr, node_t* = nullptr) : node_t{scopes_, "!", lhs_} {}
    int eval() const override { return !lhs->eval(); }
};

struct b_not_t final : node_t {
    b_not_t(Scopes &scopes_, node_t *lhs_, node_t* = nullptr, node_t* = nullptr) : node_t{scopes_, "~", lhs_} {}
    int eval() const override { return ~lhs->eval(); }
};

struct pr_increment_t final : node_t {
    pr_increment_t(Scopes &scopes_, node_t *lhs_, node_t* = nullptr, node_t* = nullptr) : node_t{scopes_, "++", lhs_} {}
    int eval() const override;
};

struct pr_decrement_t final : node_t {
    pr_decrement_t(Scopes &scopes_, node_t *lhs_, node_t* = nullptr, node_t* = nullptr) : node_t{scopes_, "--", lhs_} {}
    int eval() const override;
};

class num_t final : public node_t {
    int value = 0;

    public:
    num_t(Scopes &scopes_, const int v) : node_t{scopes_, std::to_string(v)}, value{v} {}
    int get_value() const { return value; }
    int eval() const override { return value; }
};

class variable_t : public node_t {
    std::string name;

    public:
    variable_t(Scopes &scopes_, const std::string &name_) : node_t{scopes_, name_}, name{name_} {}
    std::string get_name() const { return name; }
};

struct scalar_variable final : variable_t {
    scalar_variable(Scopes &scopes_, const std::string &name_) : variable_t{scopes_, name_} {}
    int eval() const override;
};

class func_variable final : public variable_t {
    node_t *args;
    public:
    func_variable(Scopes &scopes_, const std::string &name_, node_t *args_) : variable_t{scopes_, name_}, args{args_} {}
    int eval() const override;
};

struct print_t final : node_t {
    print_t(Scopes &scopes_, node_t *lhs_, node_t* = nullptr, node_t* = nullptr) : node_t{scopes_, "print", lhs_} {}
    int eval() const override;
};

struct abs_t final : node_t {
    abs_t(Scopes &scopes_, node_t *lhs_, node_t* = nullptr, node_t* = nullptr) : node_t{scopes_, "abs", lhs_} {}
    int eval() const override { return std::abs(lhs->eval()); }
};

struct get_t final : node_t {
    get_t(Scopes &scopes_, node_t* = nullptr, node_t* = nullptr, node_t* = nullptr) : node_t{scopes_, "?", nullptr} {}
    int eval() const override;
};

struct if_t final : node_t {
    if_t(Scopes &scopes_, node_t *lhs_, node_t *rhs_, node_t *cond_) : node_t{scopes_, "if", lhs_, rhs_, cond_} {}
    int eval() const override;
};

struct while_t final : node_t {
    while_t(Scopes &scopes_, node_t *lhs_, node_t *rhs_, node_t *cond_) : node_t{scopes_, "while", lhs_, rhs_, cond_} {}
    int eval() const override;
};

struct block final : node_t {
    block(Scopes &scopes_, node_t* lhs_, node_t* = nullptr, node_t* = nullptr) : node_t{scopes_, "block", lhs_} {}
    int eval() const override { 
        int res = 0;
        scopes.open_scope();
        if (lhs) res = lhs->eval();
        scopes.close_scope();
        return res;
    }
};

struct arg_list_insertion final : node_t {
    arg_list_insertion(Scopes &scopes_, node_t* lhs_, node_t* = nullptr, node_t* = nullptr) : node_t{scopes_, "arg_list", lhs_} {}
    int eval() const override {
        int res = 0;
        if (lhs) res = lhs->eval();
        scopes.add_arg_list(res);
        return res;
    }
};

class tree_t final {
    node_t *root;
    std::vector<std::unique_ptr<node_t>> nodes;
    Scopes scopes;

    public:

    tree_t(node_t *root_ = nullptr) : root{root_} {
        scopes.open_scope();
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
        nodes.emplace_back(std::make_unique<scalar_variable>(scopes, var));
        root = nodes.back().get();
        return root;
    }

    node_t* ast_insert(const std::string &var, node_t *args) {
        nodes.emplace_back(std::make_unique<func_variable>(scopes, var, args));
        root = nodes.back().get();
        return root;
    }

    node_t* ast_insert(node_t* lhs, const func_info &info) {
        nodes.emplace_back(std::make_unique<func_assign_t>(scopes, lhs, info));
        root = nodes.back().get();
        return root;
    }

    int evaluate() { return root->eval(); }
    void dump() const;
};

} // ast
