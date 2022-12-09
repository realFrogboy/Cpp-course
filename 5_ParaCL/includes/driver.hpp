#pragma once

#include <unordered_map>
#include <cstring>

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "lexer.hpp"
#include "ast.hpp"

namespace yy {

const std::string red  = "\033[1;31m";
const std::string norm = "\033[0m";

class driver_t final {
    lexer_t *lexer;

    public:

    ast::tree_t tree{};

    driver_t(lexer_t *lexer_) : lexer(lexer_) {}

    parser::token_type yylex(parser::semantic_type *yylval, parser::location_type *location) {
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

            if (auto search = tree.find_variable(lexer->YYText()); search != tree.variables_end())
                yylval->as<ast::name_t*>() = &(search->second);
            else {
                auto new_var = tree.add_variable(lexer->YYText());
                yylval->as<ast::name_t*>() = &(new_var.first->second);
            }
        }

        *location = lexer->location;

        return tt;
    }

    bool parse() {
        parser parser(*this);
        bool res = parser.parse();
        return res;
    }

};

} // driver_t