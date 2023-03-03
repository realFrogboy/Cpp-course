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
    std::vector<ast::scopes_t> scopes{};
    scope_t func_scope{};

    public:
    unsigned scopes_depth() const { return scopes.size(); }
    void open_scope()  { scopes.back().push_back({}); }
    void close_scope() { scopes.back().pop_back(); }
    void recover_scopes() { scopes.pop_back(); }
    void hide_scopes() { 
        scopes.push_back({});
        open_scope(); 
    }

    ast::name_t* add_variable(const std::string &name, const int val = -1) {
        return &(scopes.back().back().insert({name, name_t{name, val, 0}}).first->second);
    }

    ast::name_t* find_variable (const std::string &name) {
        auto scope = std::find_if(scopes.back().rbegin(), scopes.back().rend(), [&name](scope_t &scope) {
            auto search = scope.find(name);
            return search != scope.end();
        });
        if (scope == scopes.back().rend()) return nullptr;
        auto search = scope->find(name);
        return &(search->second);
    }

    ast::name_t* add_func(const std::string &name, const func_info &info) {
        return &(func_scope.insert({name, name_t{name, info, 1}}).first->second);
    }

    ast::name_t* add_func(const std::string &name, const int val) {
        return &(func_scope.insert({name, name_t{name, val, 1}}).first->second);
    }

    ast::name_t* find_func(const std::string &name) {
        auto search = func_scope.find(name);
        if (search == func_scope.end()) return nullptr;
        return &(search->second);
    }
};

struct node_info final {
    Scopes &scopes;
    bool is_return = 0;
    std::vector<int> arg_list;

    node_info(Scopes &scopes_) : scopes{scopes_} {}

    void init_func_args(const std::vector<std::string> &signature) const {
        for (unsigned idx = 0, sz = signature.size(); idx < sz; ++idx)
            scopes.add_variable(signature[idx], arg_list[sz - idx - 1]);
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
    node_info &n_info;

    public:
    node_t *lhs = nullptr;
    node_t *rhs = nullptr;
    node_t *cond = nullptr;

    node_t(node_info &n_info_, const std::string &msg_, node_t *lhs_ = nullptr, node_t *rhs_ = nullptr, node_t *cond_ = nullptr) : 
            dump{std::make_unique<Dump>(msg_)}, n_info{n_info_}, lhs{lhs_}, rhs{rhs_}, cond{cond_} {}
    virtual int eval() const = 0;
    virtual ~node_t() {}

    void graph_node(std::ofstream &file, int &num) const { dump->dump(this, file, num); }
    void connect_node(std::ofstream &file, int &num) const { dump->connect(this, file, num); } 
};

struct g_t final : node_t {
    g_t(node_info &n_info_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{n_info_, ">", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() > rhs->eval()); }
};

struct l_t final : node_t {
    l_t(node_info &n_info_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{n_info_, "<", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() < rhs->eval()); }
};

struct e_t final : node_t {
    e_t(node_info &n_info_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{n_info_, "==", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() == rhs->eval()); }
};

struct ne_t final : node_t {
    ne_t(node_info &n_info_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{n_info_, "!=", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() != rhs->eval()); }
};

struct ge_t final : node_t {
    ge_t(node_info &n_info_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{n_info_, ">=", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() >= rhs->eval()); }
};

struct le_t final : node_t {
    le_t(node_info &n_info_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{n_info_, "<=", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() <= rhs->eval()); }
};

struct add_t final : node_t {
    add_t(node_info &n_info_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{n_info_, "+", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() + rhs->eval()); }
};

struct sub_t final : node_t {
    sub_t(node_info &n_info_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{n_info_, "-", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() - rhs->eval()); }
};

struct mul_t final : node_t {
    mul_t(node_info &n_info_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{n_info_, "*", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() * rhs->eval()); }
};

struct div_t final : node_t {
    div_t(node_info &n_info_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{n_info_, "/", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() / rhs->eval()); }
};

struct remainder_t final : node_t {
    remainder_t(node_info &n_info_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{n_info_, "%", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() % rhs->eval()); }
};

struct pow_t final : node_t {
    pow_t(node_info &n_info_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{n_info_, "**", lhs_, rhs_} {}
    int eval() const override;
};

struct b_and_t final : node_t {
    b_and_t(node_info &n_info_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{n_info_, "&", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() & rhs->eval()); }
};

struct b_or_t final : node_t {
    b_or_t(node_info &n_info_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{n_info_, "|", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() | rhs->eval()); }
};

struct xor_t final : node_t {
    xor_t(node_info &n_info_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{n_info_, "^", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() ^ rhs->eval()); }
};

struct l_shift_t final : node_t {
    l_shift_t(node_info &n_info_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{n_info_, "<<", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() << rhs->eval()); }
};

struct r_shift_t final : node_t {
    r_shift_t(node_info &n_info_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{n_info_, ">>", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() >> rhs->eval()); }
};

struct l_and_t final : node_t {
    l_and_t(node_info &n_info_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{n_info_, "&&", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() && rhs->eval()); }
};

struct l_or_t final : node_t {
    l_or_t(node_info &n_info_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{n_info_, "||", lhs_, rhs_} {}
    int eval() const override { return (lhs->eval() || rhs->eval()); }
};

struct assign_t final : node_t {
    assign_t(node_info &n_info_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{n_info_, "=", lhs_, rhs_} {}
    int eval() const override;
};

class func_assign_t final : public node_t {
    func_info info;
    public:
    func_assign_t(node_info &n_info_, const func_info &info_, node_t *lhs_, node_t *rhs_ = nullptr) : node_t{n_info_, "=", lhs_, info_.root, rhs_}, info{info_} {}
    int eval() const override;
};

struct scolon_t final : node_t {
    scolon_t(node_info &n_info_, node_t *lhs_, node_t *rhs_, node_t* = nullptr) : node_t{n_info_, ";", lhs_, rhs_} {}
    int eval() const override;
};

struct minus_t final : node_t {
    minus_t(node_info &n_info_, node_t *lhs_, node_t* = nullptr, node_t* = nullptr) : node_t{n_info_, "-", lhs_} {}
    int eval() const override { return -lhs->eval(); }
};

struct plus_t final : node_t {
    plus_t(node_info &n_info_, node_t *lhs_, node_t* = nullptr, node_t* = nullptr) : node_t{n_info_, "+", lhs_} {}
    int eval() const override { return lhs->eval(); }
};

struct not_t final : node_t {
    not_t(node_info &n_info_, node_t *lhs_, node_t* = nullptr, node_t* = nullptr) : node_t{n_info_, "!", lhs_} {}
    int eval() const override { return !lhs->eval(); }
};

struct b_not_t final : node_t {
    b_not_t(node_info &n_info_, node_t *lhs_, node_t* = nullptr, node_t* = nullptr) : node_t{n_info_, "~", lhs_} {}
    int eval() const override { return ~lhs->eval(); }
};

struct pr_increment_t final : node_t {
    pr_increment_t(node_info &n_info_, node_t *lhs_, node_t* = nullptr, node_t* = nullptr) : node_t{n_info_, "++", lhs_} {}
    int eval() const override;
};

struct pr_decrement_t final : node_t {
    pr_decrement_t(node_info &n_info_, node_t *lhs_, node_t* = nullptr, node_t* = nullptr) : node_t{n_info_, "--", lhs_} {}
    int eval() const override;
};

class num_t final : public node_t {
    int value = 0;

    public:
    num_t(node_info &n_info_, const int v) : node_t{n_info_, std::to_string(v)}, value{v} {}
    int get_value() const { return value; }
    int eval() const override { return value; }
};

class variable_t : public node_t {
    std::string name;

    public:
    variable_t(node_info &n_info_, const std::string &name_) : node_t{n_info_, name_}, name{name_} {}
    std::string get_name() const { return name; }
};

struct scalar_variable final : variable_t {
    scalar_variable(node_info &n_info_, const std::string &name_) : variable_t{n_info_, name_} {}
    int eval() const override;
};

class func_variable final : public variable_t {
    node_t *args;
    public:
    func_variable(node_info &n_info_, const std::string &name_, node_t *args_) : variable_t{n_info_, name_}, args{args_} {}
    int eval() const override;
};

struct print_t final : node_t {
    print_t(node_info &n_info_, node_t *lhs_, node_t* = nullptr, node_t* = nullptr) : node_t{n_info_, "print", lhs_} {}
    int eval() const override;
};

struct abs_t final : node_t {
    abs_t(node_info &n_info_, node_t *lhs_, node_t* = nullptr, node_t* = nullptr) : node_t{n_info_, "abs", lhs_} {}
    int eval() const override { return std::abs(lhs->eval()); }
};

struct get_t final : node_t {
    get_t(node_info &n_info_, node_t* = nullptr, node_t* = nullptr, node_t* = nullptr) : node_t{n_info_, "?", nullptr} {}
    int eval() const override;
};

struct if_t final : node_t {
    if_t(node_info &n_info_, node_t *lhs_, node_t *rhs_, node_t *cond_) : node_t{n_info_, "if", lhs_, rhs_, cond_} {}
    int eval() const override;
};

struct while_t final : node_t {
    while_t(node_info &n_info_, node_t *lhs_, node_t *rhs_, node_t *cond_) : node_t{n_info_, "while", lhs_, rhs_, cond_} {}
    int eval() const override;
};

struct block final : node_t {
    block(node_info &n_info_, node_t* lhs_, node_t* = nullptr, node_t* = nullptr) : node_t{n_info_, "block", lhs_} {}
    int eval() const override { 
        int res = 0;
        n_info.scopes.open_scope();
        if (lhs) res = lhs->eval();
        n_info.scopes.close_scope();
        return res;
    }
};

struct arg_list_insertion final : node_t {
    arg_list_insertion(node_info &n_info_, node_t* lhs_, node_t* = nullptr, node_t* = nullptr) : node_t{n_info_, "arg_list", lhs_} {}
    int eval() const override {
        int res = 0;
        if (lhs) res = lhs->eval();
        n_info.arg_list.push_back(res);
        return res;
    }
};

struct return_t final : node_t {
    return_t(node_info &n_info_, node_t* lhs_, node_t* = nullptr, node_t* = nullptr) : node_t(n_info_, "return", lhs_) {}
    int eval() const override { 
        int res = 0;
        if (lhs) res = lhs->eval();
        n_info.is_return = 1;
        return res;
    }
};

class tree_t final {
    node_t *root;
    std::vector<std::unique_ptr<node_t>> nodes;
    Scopes scopes;
    node_info n_info;

    public:

    tree_t(node_t *root_ = nullptr) : root{root_}, n_info{scopes} {
        scopes.hide_scopes();
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
        nodes.emplace_back(std::make_unique<nodeT>(n_info, lhs, rhs, cond));
        root = nodes.back().get();
        return root;
    }

    node_t* ast_insert(const int val) {
        nodes.emplace_back(std::make_unique<num_t>(n_info, val));
        root = nodes.back().get();
        return root;
    }

    node_t* ast_insert(const std::string &var) {
        nodes.emplace_back(std::make_unique<scalar_variable>(n_info, var));
        root = nodes.back().get();
        return root;
    }

    node_t* ast_insert(const std::string &var, node_t *args) {
        nodes.emplace_back(std::make_unique<func_variable>(n_info, var, args));
        root = nodes.back().get();
        return root;
    }

    node_t* ast_insert(const func_info &info, node_t* lhs, node_t *rhs = nullptr) {
        nodes.emplace_back(std::make_unique<func_assign_t>(n_info, info, lhs, rhs));
        root = nodes.back().get();
        return root;
    }

    int evaluate() const { return root->eval(); }
    void dump() const;
};

} // ast
