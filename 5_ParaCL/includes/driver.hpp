#pragma once

#include "parser.tab.hh"
#include "lexer.hpp"
#include "ast.hpp"

namespace yy {

const std::string red  = "\033[1;31m";
const std::string norm = "\033[0m";

class driver_t final {
    lexer_t *lexer = nullptr;
    ast::scopes_t scopes{};

    public:

    ast::tree_t tree{};

    driver_t(std::ifstream *in) {
        lexer = new lexer_t{in};
        scopes.push_back({});
    }

    parser::token_type yylex(parser::semantic_type *yylval, parser::location_type *location) {
        parser::token_type tt = static_cast<parser::token_type>(lexer->yylex());

        switch(tt) {
            case yy::parser::token_type::NUMBER:
                yylval->as<int>() = std::stoi(lexer->YYText());
                break;
            case yy::parser::token_type::NAME: {
                if (auto search = find_variable(lexer->YYText()); search != nullptr) 
                    yylval->as<ast::name_t*>() = search;
                else {
                    auto new_var = add_variable(lexer->YYText());
                    yylval->as<ast::name_t*>() = new_var;
                }
                break;
            }
            case yy::parser::token_type::RUNICORN:
                remove_scope();
                break;
            case yy::parser::token_type::LUNICORN:
            case yy::parser::token_type::IF:
            case yy::parser::token_type::WHILE:
            case yy::parser::token_type::FUNC:
                add_scope();
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
        auto scope = std::find_if(scopes.rbegin(), scopes.rend(), [&name](ast::scope_t &scope) {
            auto search = scope.find(name);
            return search != scope.end();
        });
        if (scope == scopes.rend()) return nullptr;
        auto search = scope->find(name);
        return &search->second;
    }

    ast::name_t* add_variable(const std::string &name) {
        auto cur_scope = std::prev(scopes.end());
        auto new_elem = cur_scope->insert({name, ast::name_t{name, 0, 0}});
        return &new_elem.first->second;
    }

    void add_scope() {
        ast::scope_t scope;
        scopes.push_back(scope);
    }

    void remove_scope() {
        scopes.pop_back();
    }

    ~driver_t() {
        delete lexer;
    }

};

} // yy