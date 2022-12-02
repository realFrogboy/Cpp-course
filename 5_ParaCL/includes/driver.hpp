#pragma once

#include <unordered_map>
#include <cstring>

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "parser.tab.hh"
#include "ast.hpp"

namespace yy {

class driver_t {
    FlexLexer *lexer;
    std::unordered_map<std::string, ast::name_t> variables{};

    public:

    driver_t(FlexLexer *lexer_) : lexer(lexer_) {}

    parser::token_type yylex(parser::semantic_type *yylval) {
        parser::token_type tt = static_cast<parser::token_type>(lexer->yylex());

        if (tt == yy::parser::token_type::NUMBER)
            yylval->as<int>() = std::stoi(lexer->YYText());
        
        else if (tt == yy::parser::token_type::NAME) {
            if (!strcmp(lexer->YYText(), "print")) {
                yylval->as<int>() = 1;
                return yy::parser::token_type::FUNC;
            } else if (!strcmp(lexer->YYText(), "abs")) {
                yylval->as<int>() = 2;
                return yy::parser::token_type::FUNC;
            }

            if (auto search = variables.find(lexer->YYText()); search != variables.end())
                yylval->as<ast::name_t*>() = &(search->second);
            else {
                auto new_var = variables.insert({lexer->YYText(), ast::name_t{lexer->YYText(), 0}});
                yylval->as<ast::name_t*>() = &(new_var.first->second);
            }
        }

        return tt;
    }

    bool parse() {
        parser parser(*this);
        bool res = parser.parse();
        return res;
    }

};

} // driver_t