#pragma once

#include "parser.tab.hh"
#include "lexer.hpp"
#include "ast.hpp"

#include <stdexcept>

namespace yy {
using arg_list_t = std::vector<std::string>;

const std::string red  = "\033[1;31m";
const std::string norm = "\033[0m";

struct parse_error : std::runtime_error {
    explicit parse_error(const std::string &msg_) : std::runtime_error{msg_} {}
};

class arg_list_mgr final {
    std::vector<arg_list_t> list;
    public:
    unsigned size() const { return list.back().size(); }
    arg_list_t operator()() const { return list.back(); }
    void add_arg(const std::string &name) { list.back().push_back(name); }

    void create() { list.push_back({}); }
    void clear()  { list.pop_back(); }
};

class driver_t final {
    lexer_t *lexer = nullptr;
    bool is_colon = 0;

    public:
    ast::tree_t tree;
    ast::Scopes scopes;
    arg_list_mgr arg_list;

    driver_t(std::ifstream *in) {
        lexer = new lexer_t{in};
        scopes.hide_scopes();
    }

    parser::token_type yylex(parser::semantic_type *yylval, parser::location_type *location) {
        parser::token_type tt = static_cast<parser::token_type>(lexer->yylex());

        switch(tt) {
            case yy::parser::token_type::NUMBER:
                yylval->as<int>() = std::stoi(lexer->YYText());
                break;
            case yy::parser::token_type::NAME: {
                if (is_colon) {
                    auto new_func = scopes.add_global(lexer->YYText(), arg_list.size());
                    new_func->is_init = 1;
                    yylval->as<ast::name_t*>() = new_func;
                    is_colon = 0;
                } else {
                    auto search = scopes.find_all_scopes(lexer->YYText());
                    if (search == nullptr) { 
                        ast::name_t *new_var;
                        if (scopes.is_global()) 
                            new_var = scopes.add_global(lexer->YYText(), -1);
                        else 
                            new_var = scopes.add_variable(lexer->YYText(), -1);
                            
                        yylval->as<ast::name_t*>() = new_var;
                    } else 
                        yylval->as<ast::name_t*>() = search;
                }
                break;
            }
            case yy::parser::token_type::RUNICORN:
                scopes.close_scope();
                break;
            case yy::parser::token_type::LUNICORN:
                scopes.open_scope();
                break;
            case yy::parser::token_type::FUNC:
                scopes.hide_scopes();
                arg_list.create();
                break;
            case yy::parser::token_type::COLON:
                is_colon = 1;
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

    int traversal_AST() {
        #ifdef DUMP
            tree.dump();
            int err = system("dot -Tpng tree_dump.dot -o image.png");
            if (err < 0) {
                std::cout << yy::red << "Error:" << yy::norm << ": can't execute \"dot -Tpng tree_dump.dot -o image.png\"" << std::endl;
                return 0;
            }
        #endif

        #if (CODEGEN == 1)
            tree.IR_generate();
            int err = system("gcc output.o -o prog");
                if (err < 0) {
                    std::cout << yy::red << "Error:" << yy::norm << ": can't execute \"gcc output.o -o prog\"" << std::endl;
                    return 0;
                }
        #else
            tree.traversal();
        #endif
        return 0;
    }

    ~driver_t() {
        delete lexer;
    }
};

} // yy
