#pragma once

#include <vector>
#include <memory>
#include <variant>
#include <unordered_map>

namespace ast {

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

class node_t;

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
    void open_scope()  { scopes.back().push_back({}); }
    void close_scope() { scopes.back().pop_back(); }
    void recover_scopes() { scopes.pop_back(); }
    void hide_scopes() { 
        scopes.push_back({});
        open_scope(); 
    }

    bool is_global() const {
        return (scopes.size() == 1 && scopes.back().size() == 1);
    }

    ast::name_t* add_variable(const std::string &name, const int val = -1) {
        return &(scopes.back().back().insert({name, name_t{name, val, 0}}).first->second);
    }

    ast::name_t* find_variable (const std::string_view &name) {
        auto scope = std::find_if(scopes.back().rbegin(), scopes.back().rend(), [&name](scope_t &scope) {
            auto search = scope.find(std::string{name});
            return search != scope.end();
        });
        if (scope == scopes.back().rend()) return nullptr;
        auto search = scope->find(std::string{name});
        return &(search->second);
    }

    ast::name_t* add_global(const std::string &name, const func_info &info) {
        return &(glob_scope.insert({name, name_t{name, info, 0}}).first->second);
    }

    ast::name_t* add_global(const std::string &name, const int val) {
        return &(glob_scope.insert({name, name_t{name, val, 0}}).first->second);
    }

    ast::name_t* find_global(const std::string_view &name) {
        auto search = glob_scope.find(std::string{name});
        if (search == glob_scope.end()) return nullptr;
        return &(search->second);
    }

    ast::name_t* find_all_scopes(const std::string_view &name) {
        auto search = find_variable(name);
        if (search == nullptr) 
            search = find_global(name);
        return search;
    }
};

struct eval_info final {
    std::vector<int> results{0};
    node_t *root = nullptr;
    flag fl = flag::NOT_DEFINED;

    void add_result(const int res) { results.push_back(res); }
    int  get_result() { return results.back(); }
    
    int  remove_result() { 
        int res = results.back();
        results.pop_back(); 
        return res;
    }

    void clear_flag() { fl = flag::NOT_DEFINED; }
    
    void clear() {
        results.clear();
        results.push_back(0);
        clear_flag();
    }
};

struct node_info final {
    Scopes &scopes;
    bool is_return = 0;
    std::vector<int> arg_list;

    node_info(Scopes &scopes_) : scopes{scopes_} {}
    void init_func_args(const std::vector<std::string> &signature, std::vector<int> &results) const;
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

class binop_t : public node_t {
    virtual int op(const int a, const int b) const = 0;
    public:
    binop_t(node_info &n_info_, const std::string &dump, const std::vector<node_t*> &children_) : node_t{n_info_, dump, children_} {}
    void eval(eval_info &e_info) const override { 
        int rhs = e_info.remove_result();
        int lhs = e_info.remove_result();
        e_info.add_result(op(lhs, rhs));
    }
};

class g_t final : public binop_t {
    int op(const int a, const int b) const override { return a > b; }
    public:
    g_t(node_info &n_info_, const std::vector<node_t*> &children_) : binop_t{n_info_, ">", children_} {}
};

class l_t final : public binop_t {
    int op(const int a, const int b) const override { return a < b; }
    public:
    l_t(node_info &n_info_, const std::vector<node_t*> &children_) : binop_t{n_info_, "<", children_} {}
};

class e_t final : public binop_t {
    int op(const int a, const int b) const override { return a == b; }
    public:
    e_t(node_info &n_info_, const std::vector<node_t*> &children_) : binop_t{n_info_, "==", children_} {}
};

class ne_t final : public binop_t {
    int op(const int a, const int b) const override { return a != b; }
    public:
    ne_t(node_info &n_info_, const std::vector<node_t*> &children_) : binop_t{n_info_, "!=", children_} {}
};

class ge_t final : public binop_t {
    int op(const int a, const int b) const override { return a >= b; }
    public:
    ge_t(node_info &n_info_, const std::vector<node_t*> &children_) : binop_t{n_info_, ">=", children_} {}
};

class le_t final : public binop_t {
    int op(const int a, const int b) const override { return a <= b; }
    public:
    le_t(node_info &n_info_, const std::vector<node_t*> &children_) : binop_t{n_info_, "<=", children_} {}
};

class add_t final : public binop_t {
    int op(const int a, const int b) const override { return a + b; }
    public:
    add_t(node_info &n_info_, const std::vector<node_t*> &children_) : binop_t{n_info_, "+", children_} {}
};

class sub_t final : public binop_t {
    int op(const int a, const int b) const override { return a - b; }
    public:
    sub_t(node_info &n_info_, const std::vector<node_t*> &children_) : binop_t{n_info_, "-", children_} {}
};

class mul_t final : public binop_t {
    int op(const int a, const int b) const override { return a * b; }
    public:
    mul_t(node_info &n_info_, const std::vector<node_t*> &children_) : binop_t{n_info_, "*", children_} {}
};

class div_t final : public binop_t {
    int op(const int a, const int b) const override { return a / b; }
    public:
    div_t(node_info &n_info_, const std::vector<node_t*> &children_) : binop_t{n_info_, "/", children_} {}
};

class remainder_t final : public binop_t {
    int op(const int a, const int b) const override { return a % b; }
    public:
    remainder_t(node_info &n_info_, const std::vector<node_t*> &children_) : binop_t{n_info_, "%", children_} {}
};

class pow_t final : public binop_t {
    int op(const int a, const int b) const override { return std::pow(a, b); }
    public:
    pow_t(node_info &n_info_, const std::vector<node_t*> &children_) : binop_t{n_info_, "**", children_} {}
};

class b_and_t final : public binop_t {
    int op(const int a, const int b) const override { return a & b; }
    public:
    b_and_t(node_info &n_info_, const std::vector<node_t*> &children_) : binop_t{n_info_, "&", children_} {}
};

class b_or_t final : public binop_t {
    int op(const int a, const int b) const override { return a | b; }
    public:
    b_or_t(node_info &n_info_, const std::vector<node_t*> &children_) : binop_t{n_info_, "|", children_} {}
};

class xor_t final : public binop_t {
    int op(const int a, const int b) const override { return a ^ b; }
    public:
    xor_t(node_info &n_info_, const std::vector<node_t*> &children_) : binop_t{n_info_, "^", children_} {}
};

class l_shift_t final : public binop_t {
    int op(const int a, const int b) const override { return a << b; }
    public:
    l_shift_t(node_info &n_info_, const std::vector<node_t*> &children_) : binop_t{n_info_, "<<", children_} {}
};

class r_shift_t final : public binop_t {
    int op(const int a, const int b) const override { return a >> b; }
    public:
    r_shift_t(node_info &n_info_, const std::vector<node_t*> &children_) : binop_t{n_info_, ">>", children_} {}
};

class l_and_t final : public binop_t {
    int op(const int a, const int b) const override { return a && b; }
    public:
    l_and_t(node_info &n_info_, const std::vector<node_t*> &children_) : binop_t{n_info_, "&&", children_} {}
};

class l_or_t final : public binop_t {
    int op(const int a, const int b) const override { return a || b; }
    public:
    l_or_t(node_info &n_info_, const std::vector<node_t*> &children_) : binop_t{n_info_, "||", children_} {}
};

class scalar_variable final : public node_t {
    std::string name;
    public:
    scalar_variable(node_info &n_info_, const std::string &name_) : node_t{n_info_, name_}, name{name_} {}
    void eval(eval_info &e_info) const override;
    std::string get_name() const { return name; }
};

struct assign_t : node_t {
    ast::name_t* find_name() const {
        std::string_view var = static_cast<ast::scalar_variable*>(children[0])->get_name();
        auto search = n_info.scopes.find_all_scopes(var);
        if (search == nullptr) throw std::runtime_error("can't find variable");
        return search;
    }
    assign_t(node_info &n_info_, const std::string &dump, const std::vector<node_t*> &children_) : node_t{n_info_, dump, children_} {}
};

struct var_assign_t final : assign_t {
    var_assign_t(node_info &n_info_, const std::vector<node_t*> &children_) : assign_t{n_info_, "=", children_} {}
    void eval(eval_info &e_info) const override;
};

class func_assign_t final : public assign_t {
    func_info info;
    std::string func_name;
    public:
    func_assign_t(node_info &n_info_, const func_info &info_, const std::string &func_name_, const std::vector<node_t*> &children_) : assign_t{n_info_, "=", children_}, info{info_}, func_name{func_name_} {}
    void eval(eval_info &) const override;
};

struct pr_increment_t final : assign_t {
    pr_increment_t(node_info &n_info_, const std::vector<node_t*> &children_) : assign_t{n_info_, "++", children_} {}
    void eval(eval_info &e_info) const override;
};

struct pr_decrement_t final : assign_t {
    pr_decrement_t(node_info &n_info_, const std::vector<node_t*> &children_) : assign_t{n_info_, "--", children_} {}
    void eval(eval_info &e_info) const override;
};

class unop_t : public node_t {
    virtual int op(const int a) const = 0;
    public:
    unop_t(node_info &n_info_, const std::string &dump, const std::vector<node_t*> &children_) : node_t{n_info_, dump, children_} {}
    void eval(eval_info &e_info) const override { 
        int lhs = e_info.remove_result();
        e_info.add_result(op(lhs));
    }
};

class minus_t final : public unop_t {
    int op(const int a) const override { return -a; }
    public:
    minus_t(node_info &n_info_, const std::vector<node_t*> &children_) : unop_t{n_info_, "-", children_} {}
};

class plus_t final : public unop_t {
    int op(const int a) const override { return a; }
    public:
    plus_t(node_info &n_info_, const std::vector<node_t*> &children_) : unop_t{n_info_, "+", children_} {}
};

class not_t final : public unop_t {
    int op(const int a) const override { return !a; }
    public:
    not_t(node_info &n_info_, const std::vector<node_t*> &children_) : unop_t{n_info_, "!", children_} {}
};

class b_not_t final : public unop_t {
    int op(const int a) const override { return ~a; }
    public:
    b_not_t(node_info &n_info_, const std::vector<node_t*> &children_) : unop_t{n_info_, "~", children_} {}
};

class abs_t final : public unop_t {
    int op(const int a) const override { return std::abs(a); }
    public:
    abs_t(node_info &n_info_, const std::vector<node_t*> &children_) : unop_t{n_info_, "abs", children_} {}
};

class num_t final : public node_t {
    int value = 0;
    public:
    num_t(node_info &n_info_, const int v) : node_t{n_info_, std::to_string(v)}, value{v} {}
    int get_value() const { return value; }
    void eval(eval_info &e_info) const override { 
        e_info.add_result(value);
    }
};

struct print_t final : node_t {
    print_t(node_info &n_info_, const std::vector<node_t*> &children_) : node_t{n_info_, "print", children_} {}
    void eval(eval_info &e_info) const override;
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
    void eval(eval_info &e_info) const override { e_info.fl = flag::AND; }
};

class tree_t final {
    class traversal_t final {
        struct numbered_node_t final {
            node_t *node;
            unsigned number;

            numbered_node_t(node_t *node_, const unsigned num) : node{node_}, number{num} {}
        };

        struct state_t final {
            node_t *stack_point;
            long unsigned results_size;

            state_t(node_t *point, const long unsigned size) : stack_point{point}, results_size{size} {}
        };

        class states_t final {
            std::vector<state_t> states;

            public:
            void save_state(const state_t &return_info) {
                states.push_back(return_info);
            }

            state_t remove_state() {
                auto state = states.back();
                states.pop_back();
                return state;
            }

            void clear() { states.clear(); }
        };

        numbered_node_t eval_stack_top() {
            auto tmp = stack.back();
            stack.pop_back();
            tmp.node->eval(e_info);
            return tmp;
        }

        void next_children(const unsigned curr_node) {
            root = stack.back().node->children[curr_node + 1];
            currentRootIndex = curr_node + 1;
        }

        void add_node_to_stack() {
            stack.push_back({root, currentRootIndex});
            currentRootIndex = 0;

            if (root->children.size() >= 1)
                root = root->children[0];
            else
                root = nullptr;
        }

        bool is_last_child(const unsigned curr_node) const {
            return (curr_node == stack.back().node->children.size() - 1 || e_info.fl == flag::WHILE_FALSE);
        }

        bool is_while_true() const {
            return e_info.fl == flag::WHILE_TRUE;
        }

        bool is_block_end() const {
            return e_info.fl == flag::BLOCK_EXIT;
        }

        bool is_first_while_check(const unsigned curr_node) const {
            return (curr_node != 0);
        }

        void return_state(const state_t &state) {
            auto r_stack_it = std::find_if(stack.rbegin(), stack.rend(), [state](auto curr){return (curr.node == state.stack_point);});
            auto f_stack_it = (r_stack_it + 1).base();
            stack.erase(f_stack_it + 1, stack.end());

            e_info.results.erase(std::next(e_info.results.begin(), state.results_size), std::prev(e_info.results.end()));
        }

        void check_flag(const unsigned curr_node) {
            switch (e_info.fl) {
                case flag::AND:
                    if (!e_info.get_result())
                        root = nullptr;
                    else 
                        next_children(curr_node);
                    break;
                case flag::IF_FALSE: 
                    next_children(curr_node + 1);
                    break;
                case flag::IF_TRUE:
                    next_children(curr_node);
                    ++currentRootIndex;
                    break;
                case flag::WHILE_TRUE:
                    if (is_first_while_check(curr_node)) {
                        next_children(curr_node - 2);
                        e_info.results.pop_back();
                    } else {
                        next_children(curr_node);
                    }
                    break;
                case flag::FUNC_ENTRY:
                    root = e_info.root;
                    currentRootIndex = curr_node;
                    break;
                case flag::BLOCK_ENTRY:
                    states.save_state({stack.back().node, e_info.results.size()});
                    next_children(curr_node);
                    break;
                case flag::BLOCK_EXIT: { 
                    state_t state = states.remove_state();
                    return_state(state);

                    root = stack.back().node->children[2];
                    currentRootIndex = 2;
                    break;
                }
                default:
                    next_children(curr_node);
            };
        }

        node_t *root;
        unsigned currentRootIndex = 0;
        std::vector<numbered_node_t> stack;
        states_t states;
        eval_info e_info;

        public:

        void traversal(node_t *root_) {
             root = root_;
             while (root || stack.size() > 0) {
                if (root) {
                    add_node_to_stack();
                    continue;
                }

                numbered_node_t curr_node = eval_stack_top();

                while (stack.size() > 0 && is_last_child(curr_node.number) && !is_while_true() && !is_block_end()) {
                    e_info.clear_flag();
                    curr_node = eval_stack_top();
                }

                if (stack.size() > 0) {
                    check_flag(curr_node.number);
                    e_info.clear_flag();
                }
            }
            currentRootIndex = 0;
            stack.clear();
            states.clear();
            e_info.clear();
        }
    };

    node_t *root;
    std::vector<std::unique_ptr<node_t>> nodes;
    Scopes scopes;
    node_info n_info;
    std::unique_ptr<traversal_t> trv;

    public:

    tree_t(node_t *root_ = nullptr) : root{root_}, n_info{scopes}, trv{std::make_unique<traversal_t>()} {
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

    void traversal() const { trv->traversal(root); }
    void dump() const;
};

} // ast
