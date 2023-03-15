#pragma once

#include <vector>
#include <memory>
#include <variant>
#include <unordered_map>

namespace ast {

class node_t;

enum class flag {
    IF_TRUE,
    IF_FALSE,

    WHILE_TRUE,
    WHILE_FALSE,

    FUNC_ENTRY,

    BLOCK_ENTRY,
    BLOCK_EXIT,

    AND,

    NOT_DEFINED
};

struct func_info final {
    node_t *root;
    std::vector<std::string> signature;
};

struct name_t final {
    std::string name;
    std::variant<int, func_info> value;
    bool is_init = 0;
};

using scope_t  = std::unordered_map<std::string, ast::name_t>;
using scopes_t = std::vector<scope_t>;

class Scopes final {
    std::vector<ast::scopes_t> scopes;
    scope_t glob_scope;

    public:
    unsigned scopes_depth() const { return scopes.size(); }
    unsigned scopes_level() const { return scopes.back().size(); }
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

    ast::name_t* add_global(const std::string &name, const func_info &info) {
        return &(glob_scope.insert({name, name_t{name, info, 0}}).first->second);
    }

    ast::name_t* add_global(const std::string &name, const int val) {
        return &(glob_scope.insert({name, name_t{name, val, 0}}).first->second);
    }

    ast::name_t* find_global(const std::string &name) {
        auto search = glob_scope.find(name);
        if (search == glob_scope.end()) return nullptr;
        return &(search->second);
    }

    ast::name_t* find_all_scopes(const std::string &name) {
        auto search = find_variable(name);
        if (search == nullptr) 
            search = find_global(name);
        return search;
    }
};

struct node_info final {
    Scopes &scopes;
    bool is_return = 0;
    std::vector<int> arg_list;

    node_info(Scopes &scopes_) : scopes{scopes_} {}
    void init_func_args(const std::vector<std::string> &signature, std::vector<int> &results) const;
};

struct eval_info final {
    std::vector<int> results{0};
    node_t *root = nullptr;
    flag fl = flag::NOT_DEFINED;
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
    std::vector<node_t*> children;

    node_t(node_info &n_info_, const std::string &msg_, const std::vector<node_t*> &children_ = {}) : 
            dump{std::make_unique<Dump>(msg_)}, n_info{n_info_}, children{children_} {}
    virtual void eval(eval_info &e_info) const = 0;
    virtual ~node_t() {}

    void graph_node(std::ofstream &file, int &num) const { dump->dump(this, file, num); }
    void connect_node(std::ofstream &file, int &num) const { dump->connect(this, file, num); } 
};

struct g_t final : node_t {
    g_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, ">", children_} {}
    void eval(eval_info &e_info) const override {
        int rhs = e_info.results.back(); e_info.results.pop_back();
        int lhs = e_info.results.back(); e_info.results.pop_back();
        e_info.results.push_back(lhs > rhs);
    }
};

struct l_t final : node_t {
    l_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "<", children_} {}
    void eval(eval_info &e_info) const override { 
        int rhs = e_info.results.back(); e_info.results.pop_back();
        int lhs = e_info.results.back(); e_info.results.pop_back(); 
        e_info.results.push_back(lhs < rhs);
    }
};

struct e_t final : node_t {
    e_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "==", children_} {}
    void eval(eval_info &e_info) const override { 
        int rhs = e_info.results.back(); e_info.results.pop_back();
        int lhs = e_info.results.back(); e_info.results.pop_back(); 
        e_info.results.push_back(lhs == rhs);
    }
};

struct ne_t final : node_t {
    ne_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "!=", children_} {}
    void eval(eval_info &e_info) const override {
        int rhs = e_info.results.back(); e_info.results.pop_back();
        int lhs = e_info.results.back(); e_info.results.pop_back(); 
        e_info.results.push_back(lhs != rhs);
    }
};

struct ge_t final : node_t {
    ge_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, ">=", children_} {}
    void eval(eval_info &e_info) const override { 
        int rhs = e_info.results.back(); e_info.results.pop_back();
        int lhs = e_info.results.back(); e_info.results.pop_back(); 
        e_info.results.push_back(lhs >= rhs);
    }
};

struct le_t final : node_t {
    le_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "<=", children_} {}
    void eval(eval_info &e_info) const override { 
        int rhs = e_info.results.back(); e_info.results.pop_back();
        int lhs = e_info.results.back(); e_info.results.pop_back();
        e_info.results.push_back(lhs <= rhs); 
    }
};

struct add_t final : node_t {
    add_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "+", children_} {}
    void eval(eval_info &e_info) const override { 
        int rhs = e_info.results.back(); e_info.results.pop_back();
        int lhs = e_info.results.back(); e_info.results.pop_back(); 
        e_info.results.push_back(lhs + rhs);
    }
};

struct sub_t final : node_t {
    sub_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "-", children_} {}
    void eval(eval_info &e_info) const override { 
        int rhs = e_info.results.back(); e_info.results.pop_back();
        int lhs = e_info.results.back(); e_info.results.pop_back(); 
        e_info.results.push_back(lhs - rhs);
    }
};

struct mul_t final : node_t {
    mul_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "*", children_} {}
    void eval(eval_info &e_info) const override { 
        int rhs = e_info.results.back(); e_info.results.pop_back();
        int lhs = e_info.results.back(); e_info.results.pop_back(); 
        e_info.results.push_back(lhs * rhs);
    }
};

struct div_t final : node_t {
    div_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "/", children_} {}
    void eval(eval_info &e_info) const override { 
        int rhs = e_info.results.back(); e_info.results.pop_back();
        int lhs = e_info.results.back(); e_info.results.pop_back();
        e_info.results.push_back(lhs / rhs);
    }
};

struct remainder_t final : node_t {
    remainder_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "%", children_} {}
    void eval(eval_info &e_info) const override {
        int rhs = e_info.results.back(); e_info.results.pop_back();
        int lhs = e_info.results.back(); e_info.results.pop_back();
        e_info.results.push_back(lhs % rhs);
    }
};

struct pow_t final : node_t {
    pow_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "**", children_} {}
    void eval(eval_info &e_info) const override;
};

struct b_and_t final : node_t {
    b_and_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "&", children_} {}
    void eval(eval_info &e_info) const override { 
        int rhs = e_info.results.back(); e_info.results.pop_back();
        int lhs = e_info.results.back(); e_info.results.pop_back(); 
        e_info.results.push_back(lhs & rhs);
    }
};

struct b_or_t final : node_t {
    b_or_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "|", children_} {}
    void eval(eval_info &e_info) const override { 
        int rhs = e_info.results.back(); e_info.results.pop_back();
        int lhs = e_info.results.back(); e_info.results.pop_back(); 
        e_info.results.push_back(lhs | rhs);
    }
};

struct xor_t final : node_t {
    xor_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "^", children_} {}
    void eval(eval_info &e_info) const override {
        int rhs = e_info.results.back(); e_info.results.pop_back();
        int lhs = e_info.results.back(); e_info.results.pop_back(); 
        e_info.results.push_back(lhs ^ rhs); 
    }
};

struct l_shift_t final : node_t {
    l_shift_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "<<", children_} {}
    void eval(eval_info &e_info) const override { 
        int rhs = e_info.results.back(); e_info.results.pop_back();
        int lhs = e_info.results.back(); e_info.results.pop_back(); 
        e_info.results.push_back(lhs << rhs);
    }
};

struct r_shift_t final : node_t {
    r_shift_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, ">>", children_} {}
    void eval(eval_info &e_info) const override { 
        int rhs = e_info.results.back(); e_info.results.pop_back();
        int lhs = e_info.results.back(); e_info.results.pop_back(); 
        e_info.results.push_back(lhs >> rhs);
    }
};

struct l_and_t final : node_t {
    l_and_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "&&", children_} {}
    void eval(eval_info &e_info) const override { 
        int rhs = e_info.results.back(); e_info.results.pop_back();
        int lhs = e_info.results.back(); e_info.results.pop_back();
        e_info.results.push_back(lhs && rhs); 
    }
};

struct l_or_t final : node_t {
    l_or_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "||", children_} {}
    void eval(eval_info &e_info) const override { 
        int rhs = e_info.results.back(); e_info.results.pop_back();
        int lhs = e_info.results.back(); e_info.results.pop_back();
        e_info.results.push_back(lhs || rhs); 
    }
};

struct assign_t final : node_t {
    assign_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "=", children_} {}
    void eval(eval_info &e_info) const override;
};

class func_assign_t final : public node_t {
    func_info info;
    std::string func_name;
    public:
    func_assign_t(node_info &n_info_, const func_info &info_, const std::string &func_name_, const std::vector<node_t*> &children_) : node_t{n_info_, "=", children_}, info{info_}, func_name{func_name_} {}
    void eval(eval_info &) const override;
};

struct scolon_t final : node_t {
    scolon_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, ";", children_} {}
    void eval(eval_info &e_info) const override;
};

struct minus_t final : node_t {
    minus_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "-", children_} {}
    void eval(eval_info &e_info) const override { 
        int lhs = e_info.results.back(); e_info.results.pop_back();
        e_info.results.push_back(-lhs);
    }
};

struct plus_t final : node_t {
    plus_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "+", children_} {}
    void eval(eval_info &e_info) const override { 
        int lhs = e_info.results.back(); e_info.results.pop_back();
        e_info.results.push_back(lhs);
    }
};

struct not_t final : node_t {
    not_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "!", children_} {}
    void eval(eval_info &e_info) const override { 
        int lhs = e_info.results.back(); e_info.results.pop_back();
        e_info.results.push_back(!lhs);
    }
};

struct b_not_t final : node_t {
    b_not_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "~", children_} {}
    void eval(eval_info &e_info) const override {
        int lhs = e_info.results.back(); e_info.results.pop_back();
        e_info.results.push_back(~lhs); 
    }
};

struct pr_increment_t final : node_t {
    pr_increment_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "++", children_} {}
    void eval(eval_info &e_info) const override;
};

struct pr_decrement_t final : node_t {
    pr_decrement_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "--", children_} {}
    void eval(eval_info &e_info) const override;
};

class num_t final : public node_t {
    int value = 0;
    public:
    num_t(node_info &n_info_, const int v) : node_t{n_info_, std::to_string(v)}, value{v} {}
    int get_value() const { return value; }
    void eval(eval_info &e_info) const override { 
        e_info.results.push_back(value);
    }
};

class scalar_variable final : public node_t {
    std::string name;
    public:
    scalar_variable(node_info &n_info_, const std::string &name_) : node_t{n_info_, name_}, name{name_} {}
    void eval(eval_info &e_info) const override;
    std::string get_name() const { return name; }
};

struct print_t final : node_t {
    print_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "print", children_} {}
    void eval(eval_info &e_info) const override;
};

struct abs_t final : node_t {
    abs_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "abs", children_} {}
    void eval(eval_info &e_info) const override { 
        int lhs = e_info.results.back(); e_info.results.pop_back();
        e_info.results.push_back(std::abs(lhs));
    }
};

struct get_t final : node_t {
    get_t(node_info &n_info_, const std::vector<node_t*> &) : node_t{n_info_, "?"} {}
    void eval(eval_info &e_info) const override;
};

struct if_t final : node_t {
    if_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "if", children_} {}
    void eval(eval_info &e_info) const override;
};

struct while_t final : node_t {
    while_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "while", children_} {}
    void eval(eval_info &e_info) const override;
};

struct block final : node_t {
    block(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "block", children_} {}
    void eval(eval_info &) const override {}
};

struct return_t final : node_t {
    return_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "return", children_} {}
    void eval(eval_info &e_info) const override { e_info.fl = flag::BLOCK_EXIT; }
};

struct open_scope final : node_t {
    open_scope(node_info &n_info, const std::vector<node_t*> &) : node_t{n_info, "{"} {}
    void eval(eval_info &) const override { n_info.scopes.open_scope(); }
};

struct close_scope final : node_t {
    close_scope(node_info &n_info, const std::vector<node_t*> &) : node_t{n_info, "}"} {}
    void eval(eval_info &) const override { n_info.scopes.close_scope(); }
};

struct recover_scopes final : node_t {
    recover_scopes(node_info &n_info, const std::vector<node_t*> &) : node_t{n_info, "recover_scope"} {}
    void eval(eval_info &) const override { n_info.scopes.recover_scopes(); }
};

struct func_init final : node_t {
    std::string func_name;
    func_init(node_info &n_info, const std::string &name_) : node_t{n_info, name_}, func_name{name_} {}
    void eval(eval_info &e_info) const override {
        auto search = n_info.scopes.find_variable(func_name);
        if (search == nullptr) 
            search = n_info.scopes.find_global(func_name);
        if (search == nullptr) throw std::runtime_error("can't find function");

        n_info.scopes.hide_scopes();

        func_info info = std::get<func_info>(search->value);
        n_info.init_func_args(info.signature, e_info.results);

        e_info.fl = flag::FUNC_ENTRY;
        e_info.root = info.root;
    }
};

struct block_init final : node_t {
    block_init(node_info &n_info, const std::vector<node_t*> &) : node_t{n_info, "block_init"} {}
    void eval(eval_info &e_info) const override {
         n_info.scopes.open_scope();
         e_info.fl = flag::BLOCK_ENTRY;
    }
};

struct block_exec final : node_t {
    block_exec(node_info &n_info, const std::vector<node_t*> &children_) : node_t{n_info, "block_exec", children_} {}
    void eval(eval_info &e_info) const override { 
        n_info.scopes.close_scope();
        e_info.fl = flag::BLOCK_EXIT;
    }
};

struct and_indicator final : node_t {
    and_indicator(node_info &n_info, const std::vector<node_t*> &) : node_t{n_info, "and_indicator"} {}
    void eval(eval_info &e_info) const override {e_info.fl = flag::AND; }
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
    node_t* ast_insert(const std::vector<node_t*> &children = {}) {
        nodes.emplace_back(std::make_unique<nodeT>(n_info, children));
        root = nodes.back().get();
        return root;
    }

    node_t* ast_insert(const int val) {
        nodes.emplace_back(std::make_unique<num_t>(n_info, val));
        root = nodes.back().get();
        return root;
    }

    template <typename nodeT>
    node_t* ast_insert(const std::string &var) {
        nodes.emplace_back(std::make_unique<nodeT>(n_info, var));
        root = nodes.back().get();
        return root;
    }

    node_t* ast_insert(const func_info &info, const std::string &func_name, const std::vector<node_t*> &children) {
        nodes.emplace_back(std::make_unique<func_assign_t>(n_info, info, func_name, children));
        root = nodes.back().get();
        return root;
    }

    void traversal() const;
    void dump() const;
};

} // ast
