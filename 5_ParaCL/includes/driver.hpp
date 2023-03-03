#pragma once

#include "parser.tab.hh"
#include "lexer.hpp"
#include "ast.hpp"

namespace yy {
using arg_list_t = std::vector<std::string>;

const std::string red  = "\033[1;31m";
const std::string norm = "\033[0m";

class driver_t final {
    lexer_t *lexer = nullptr;
    std::vector<ast::scopes_t> scopes{};
    ast::scope_t func_scope{};
    bool is_colon = 0;

    public:
    ast::tree_t tree{};
    std::vector<arg_list_t> v_arg_list{};

    driver_t(std::ifstream *in) {
        lexer = new lexer_t{in};
        scopes.push_back({});
        add_scope();
    }

    parser::token_type yylex(parser::semantic_type *yylval, parser::location_type *location) {
        parser::token_type tt = static_cast<parser::token_type>(lexer->yylex());

        switch(tt) {
            case yy::parser::token_type::NUMBER:
                yylval->as<int>() = std::stoi(lexer->YYText());
                break;
            case yy::parser::token_type::NAME: {
                if (is_colon) {
                    auto new_func = add_func(lexer->YYText());
                    yylval->as<ast::name_t*>() = new_func;
                    is_colon = 0;
                } else {
                    if (auto search = find_variable(lexer->YYText()); search != nullptr) 
                        yylval->as<ast::name_t*>() = search;
                    else {
                        auto new_var = add_variable(lexer->YYText());
                        yylval->as<ast::name_t*>() = new_var;
                    }
                }
                break;
            }
            case yy::parser::token_type::RUNICORN:
                remove_scope();
                break;
            case yy::parser::token_type::LUNICORN:
                add_scope();
                break;
            case yy::parser::token_type::FUNC:
                hide_scopes();
                v_arg_list.push_back({});
                break;
            case yy::parser::token_type::COLON:
                is_colon = 1;
                break;
            default: {}
        }

        *location = lexer->location;
        return tt;
    }

    bool parse() {
        parser parser(*this);
        bool res = parser.parse();
        return res;
    }

    ast::name_t* find_variable(const std::string &name) {
        auto search = func_scope.find(name);
        if (search != func_scope.end()) return &search->second;

        auto scope = std::find_if(scopes.back().rbegin(), scopes.back().rend(), [&name](ast::scope_t &scope) {
            auto search = scope.find(name);
            return search != scope.end();
        });
        if (scope == scopes.back().rend()) return nullptr;
        search = scope->find(name);
        return &search->second;
    }

    ast::name_t* add_func(const std::string &name) {
        auto new_func = func_scope.insert({name, ast::name_t{name, arg_list_size(), 1}});
        return &new_func.first->second;
    }

    ast::name_t* add_variable(const std::string &name) {
        auto cur_scope = std::prev(scopes.back().end());
        auto new_elem = cur_scope->insert({name, ast::name_t{name, -1, 0}});
        return &new_elem.first->second;
    }

    void add_scope() { scopes.back().push_back({}); }
    void remove_scope() { scopes.back().pop_back(); }

    void hide_scopes() {
        scopes.push_back({});
        add_scope();
    }
    void recover_scopes() { scopes.pop_back(); }

    void add_arg(const std::string &name) { v_arg_list.back().push_back(name); }
    unsigned arg_list_size() { return v_arg_list.back().size(); }
    void clear_agr_list() { v_arg_list.pop_back(); }

    unsigned scopes_depth() { return scopes.size(); }

    ~driver_t() {
        delete lexer;
    }

};

} // yy
