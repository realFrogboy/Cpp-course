#pragma once

#include <unordered_map>
#include <cstring>

#include "lexer.hpp"
#include "ast.hpp"

namespace yy {

const std::string red  = "\033[1;31m";
const std::string norm = "\033[0m";

class driver_t final {
    lexer_t *lexer = nullptr;
    std::vector<std::unordered_map<std::string, ast::name_t>> scopes{};

    public:

    ast::tree_t tree{};

    driver_t(std::ifstream *in) {
        lexer = new lexer_t{in};
        std::unordered_map<std::string, ast::name_t> global;
        scopes.push_back(global);
    }

    parser::token_type yylex(parser::semantic_type *yylval, parser::location_type *location) {
        parser::token_type tt = static_cast<parser::token_type>(lexer->yylex());

        if (tt == yy::parser::token_type::NUMBER)
            yylval->as<int>() = std::stoi(lexer->YYText());
        else if (tt == yy::parser::token_type::NAME) {
            if (auto search = find_variable(lexer->YYText()); search != nullptr) 
                yylval->as<ast::name_t*>() = search;
            else {
                auto new_var = add_variable(lexer->YYText());
                yylval->as<ast::name_t*>() = new_var;
            }
            //--------------------------
            if (auto search = tree.find_variable(lexer->YYText()); search != tree.variables_end()) {}
                //yylval->as<ast::name_t*>() = &(search->second);
            else {
                auto new_var = tree.add_variable(lexer->YYText());
                //yylval->as<ast::name_t*>() = &(new_var.first->second);
            }
        }
        else if (tt == yy::parser::token_type::LUNICORN) {
            add_scope();
        }
        else if (tt == yy::parser::token_type::RUNICORN) {
            remove_scope();
        }
        else if (tt == yy::parser::token_type::IF) {
            add_scope();
        }
        else if (tt == yy::parser::token_type::WHILE) {
            add_scope();
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
        size_t n = scopes.size();
        for (int idx = n - 1; idx >= 0; --idx) {
            if (auto search = scopes[idx].find(name); search != scopes[idx].end())
                return &search->second;
        }
        return nullptr;
    }

    ast::name_t* add_variable(const std::string &name) {
        auto cur_scope = std::prev(scopes.end());
        auto new_elem = cur_scope->insert({name, ast::name_t{name, 0, 0}});
        return &new_elem.first->second;
    }

    void add_scope() {
        std::unordered_map<std::string, ast::name_t> scope;
        scopes.push_back(scope);
    }

    void remove_scope() {
        scopes.pop_back();
    }

    ~driver_t() {
        delete lexer;
    }

};

} // driver_t