%skeleton "lalr1.cc"
%require "3.5.1"
%language "c++"

%define api.value.type variant
%define parse.error verbose
%define parse.lac full

%param { yy::driver_t &drv }

%locations

%code requires {
#include <iostream>
#include <variant>
#include <string>
#include <memory>
#include <cmath>

#include "ast.hpp"

namespace yy { class driver_t; }
}

%code {
#include "driver.hpp"
namespace yy { 
    parser::token_type yylex(parser::semantic_type* yylval, parser::location_type* loc, driver_t &driver); 
    unsigned init_arg_size = 0;
    bool is_error = 0;
}
}

%token
    ERR     "UNKNOWN OPERATION"
    COMMA   ","

    POW     "**"
    MINUS   "-"
    PLUS    "+"
    STAR    "*"
    SLASH   "/"
    PERSENT "%"

    LPAREN   "("
    RPAREN   ")"
    LUNICORN "{"
    RUNICORN "}"
    SCOLON   ";"
    COLON    ":"

    GRATER  ">"
    LESS    "<"
    EQUAL   "=="
    NEQUAL  "!="
    GEQUAL  ">="
    LEQUAL  "<="

    AMPERSAND "&"
    STICK     "|"
    TILDA     "~"
    CAP       "^"
    L_SHIFT   "<<"
    R_SHIFT   ">>"

    D_AMPERSAND "&&"
    D_STICK     "||"
    NOT     "!"

    D_PLUS  "++"
    D_MINUS "--"

    ASSIGN  "="

    IF      "if"
    ELSE    "else"
    WHILE   "while"
    FUNC    "func"
    RETURN  "return"

    PRINT   "print"
    ABS     "abs"
    GET     "?"

    END 0    "end of file"
;

%token <int> NUMBER
%token <ast::name_t*> NAME
%nterm <ast::node_t*> exp
%nterm <ast::node_t*> scolon_exp
%nterm <ast::node_t*> list
%nterm <ast::node_t*> stmt
%nterm <ast::node_t*> io_func
%nterm <ast::node_t*> line
%nterm <ast::node_t*> body
%nterm <ast::node_t*> block
%nterm arg_list
%nterm <ast::node_t*> init_arg_list

%nonassoc RPAREN
%nonassoc ELSE

%right ASSIGN
%left D_STICK
%left D_AMPERSAND
%left STICK
%left CAP
%left AMPERSAND
%left EQUAL NEQUAL
%left GRATER LESS GEQUAL LEQUAL
%left L_SHIFT R_SHIFT
%left PLUS MINUS
%left STAR SLASH PERSENT
%left POW
%nonassoc UMINUS

%start program

%% 

stmt: IF LPAREN exp RPAREN body {
        auto if_cond = drv.tree.ast_insert<ast::if_t>(std::vector<ast::node_t*>{$3});
        auto IR_if_cont = drv.tree.ast_insert<ast::IR_if_cont_without_else>();

        $$ = drv.tree.ast_insert<ast::block>(std::vector<ast::node_t*>{if_cond, $5, IR_if_cont, nullptr});
    }
    | IF LPAREN exp RPAREN body ELSE body {
        auto if_cond = drv.tree.ast_insert<ast::if_t>(std::vector<ast::node_t*>{$3});
        auto IR_else = drv.tree.ast_insert<ast::IR_else_t>();
        auto IR_if_cont = drv.tree.ast_insert<ast::IR_if_cont_with_else>();

        $$ = drv.tree.ast_insert<ast::block>(std::vector<ast::node_t*>{if_cond, $5, IR_else, $7, IR_if_cont});
    }
    | IF LPAREN exp RPAREN stmt ELSE body {
        auto if_cond = drv.tree.ast_insert<ast::if_t>(std::vector<ast::node_t*>{$3});
        auto IR_else = drv.tree.ast_insert<ast::IR_else_t>();
        auto IR_if_cont = drv.tree.ast_insert<ast::IR_if_cont_with_else>();

        $$ = drv.tree.ast_insert<ast::block>(std::vector<ast::node_t*>{if_cond, $5, IR_else, $7, IR_if_cont});
    }
    | IF LPAREN exp RPAREN body ELSE stmt {
        auto if_cond = drv.tree.ast_insert<ast::if_t>(std::vector<ast::node_t*>{$3});
        auto IR_else = drv.tree.ast_insert<ast::IR_else_t>();
        auto IR_if_cont = drv.tree.ast_insert<ast::IR_if_cont_with_else>();

        $$ = drv.tree.ast_insert<ast::block>(std::vector<ast::node_t*>{if_cond, $5, IR_else, $7, IR_if_cont});
    }
    | WHILE LPAREN exp RPAREN body {
        auto while_cond = drv.tree.ast_insert<ast::while_t>(std::vector<ast::node_t*>{$3});
        auto end_cond = drv.tree.ast_insert<ast::IR_end_cond_t>(std::vector<ast::node_t*>{$3});

        $$ = drv.tree.ast_insert<ast::block>(std::vector<ast::node_t*>{while_cond, $5, end_cond});
    }
    | IF LPAREN exp RPAREN stmt {
        auto if_cond = drv.tree.ast_insert<ast::if_t>(std::vector<ast::node_t*>{$3});
        auto IR_if_cont = drv.tree.ast_insert<ast::IR_if_cont_without_else>();

        $$ = drv.tree.ast_insert<ast::block>(std::vector<ast::node_t*>{if_cond, $5, IR_if_cont, nullptr});
    }
    | IF LPAREN exp RPAREN stmt ELSE stmt {
        auto if_cond = drv.tree.ast_insert<ast::if_t>(std::vector<ast::node_t*>{$3});
        auto IR_else = drv.tree.ast_insert<ast::IR_else_t>();
        auto IR_if_cont = drv.tree.ast_insert<ast::IR_if_cont_with_else>();

        $$ = drv.tree.ast_insert<ast::block>(std::vector<ast::node_t*>{if_cond, $5, IR_else, $7, IR_if_cont});
    }
    | WHILE LPAREN exp RPAREN stmt { 
        auto while_cond = drv.tree.ast_insert<ast::while_t>(std::vector<ast::node_t*>{$3});
        auto end_cond = drv.tree.ast_insert<ast::IR_end_cond_t>(std::vector<ast::node_t*>{$3});

        $$ = drv.tree.ast_insert<ast::block>(std::vector<ast::node_t*>{while_cond, $5, end_cond});
    }
    ;

body: line
    | END  { $$ = nullptr; }
    ;

arg_list:
        | NAME {
            $1->is_init = 1;
            drv.arg_list.add_arg($1->name);
        }
        | arg_list COMMA NAME {
            $3->is_init = 1;
            drv.arg_list.add_arg($3->name);
        }
        ;

init_arg_list: { $$ = nullptr; }
             | exp {
                  $$ = drv.tree.ast_insert<ast::block>(std::vector<ast::node_t*>{$1});
                  ++init_arg_size;
             }
             | init_arg_list COMMA exp  {
                $$ = drv.tree.ast_insert<ast::block>(std::vector<ast::node_t*>{$3, $1});
                ++init_arg_size;
             }
             ;

list: { $$ = nullptr; }
    | stmt list {
        if ($2 == nullptr)
            $$ = $1;
        else {
            $$ = drv.tree.ast_insert<ast::block>(std::vector<ast::node_t*>{$1, $2});
        }
    }
    | line list {
        if ($2 == nullptr)
            $$ = $1;
        else {
            if ($1 == nullptr) {
                auto empty_lhs = drv.tree.ast_insert<ast::block>();
                $$ = drv.tree.ast_insert<ast::block>(std::vector<ast::node_t*>{empty_lhs, $2});
            } else 
                $$ = drv.tree.ast_insert<ast::block>(std::vector<ast::node_t*>{$1, $2});
        }
    }
    | error SCOLON list { yyerrok; }
    ;

block: LUNICORN list RUNICORN {
        auto block_init = drv.tree.ast_insert<ast::block_init>();
        auto block_exec = drv.tree.ast_insert<ast::block_exec>(std::vector<ast::node_t*>{$2});
        $$ = drv.tree.ast_insert<ast::block>(std::vector<ast::node_t*>{block_init, block_exec, nullptr});
     }
     ;

line: exp
    | scolon_exp SCOLON { $$ = $1; }
    | io_func SCOLON    { $$ = $1; }
    | NAME ASSIGN FUNC LPAREN arg_list RPAREN block {
        drv.scopes.recover_scopes();

        $1->is_init = 1;
        $1->value = static_cast<int>(drv.arg_list.size());

        ast::node_t *p_node = drv.tree.ast_insert<ast::scalar_variable>($1->name);
        $$ = drv.tree.ast_insert(ast::func_info{$7, drv.arg_list()}, "", std::vector<ast::node_t*>{p_node});
        drv.arg_list.clear();
    }
    | NAME ASSIGN FUNC LPAREN arg_list RPAREN COLON NAME block {
        drv.scopes.recover_scopes();

        $1->is_init = 1;
        $1->value = static_cast<int>(drv.arg_list.size());

        ast::node_t *p_node = drv.tree.ast_insert<ast::scalar_variable>($1->name);
        $$ = drv.tree.ast_insert(ast::func_info{$9, drv.arg_list()}, $8->name, std::vector<ast::node_t*>{p_node});
        drv.arg_list.clear();
    }
    | SCOLON { $$ = nullptr; }
    ;

exp : block
    | scolon_exp
    ;

scolon_exp: exp GRATER exp {
        $$ = drv.tree.ast_insert<ast::g_t>(std::vector<ast::node_t*>{$1, $3});
    }
    | exp LESS exp { 
        $$ = drv.tree.ast_insert<ast::l_t>(std::vector<ast::node_t*>{$1, $3});
    }
    | exp EQUAL exp { 
        $$ = drv.tree.ast_insert<ast::e_t>(std::vector<ast::node_t*>{$1, $3});
    }
    | exp NEQUAL exp { 
        $$ = drv.tree.ast_insert<ast::ne_t>(std::vector<ast::node_t*>{$1, $3});
    }
    | exp GEQUAL exp { 
        $$ = drv.tree.ast_insert<ast::ge_t>(std::vector<ast::node_t*>{$1, $3});
    }
    | exp LEQUAL exp { 
        $$ = drv.tree.ast_insert<ast::le_t>(std::vector<ast::node_t*>{$1, $3});
    }
    | exp PLUS exp {
        $$ = drv.tree.ast_insert<ast::add_t>(std::vector<ast::node_t*>{$1, $3});
    }
    | exp MINUS exp {
        $$ = drv.tree.ast_insert<ast::sub_t>(std::vector<ast::node_t*>{$1, $3});
    }
    | exp STAR exp { 
        $$ = drv.tree.ast_insert<ast::mul_t>(std::vector<ast::node_t*>{$1, $3});
    }
    | exp SLASH exp { 
        $$ = drv.tree.ast_insert<ast::div_t>(std::vector<ast::node_t*>{$1, $3});
    }
    | exp PERSENT exp {
        $$ = drv.tree.ast_insert<ast::remainder_t>(std::vector<ast::node_t*>{$1, $3});
    }
    | exp POW exp { 
        $$ = drv.tree.ast_insert<ast::pow_t>(std::vector<ast::node_t*>{$1, $3});
    }
    | NAME ASSIGN exp {
        $1->is_init = 1;

        ast::node_t *p_node = drv.tree.ast_insert<ast::scalar_variable>($1->name);
        $$ = drv.tree.ast_insert<ast::var_assign_t>(std::vector<ast::node_t*>{p_node, $3});
    }
    | exp AMPERSAND exp {
        $$ = drv.tree.ast_insert<ast::b_and_t>(std::vector<ast::node_t*>{$1, $3});
    }
    | exp STICK exp {
        $$ = drv.tree.ast_insert<ast::b_or_t>(std::vector<ast::node_t*>{$1, $3});
    }
    | exp CAP exp {
        $$ = drv.tree.ast_insert<ast::xor_t>(std::vector<ast::node_t*>{$1, $3});
    }
    | exp L_SHIFT exp {
        $$ = drv.tree.ast_insert<ast::l_shift_t>(std::vector<ast::node_t*>{$1, $3});
    }
    | exp R_SHIFT exp {
        $$ = drv.tree.ast_insert<ast::r_shift_t>(std::vector<ast::node_t*>{$1, $3});
    }
    | exp D_AMPERSAND exp {
        $$ = drv.tree.ast_insert<ast::l_and_t>(std::vector<ast::node_t*>{$1, $3});
    }
    | exp D_STICK exp {
        $$ = drv.tree.ast_insert<ast::l_or_t>(std::vector<ast::node_t*>{$1, $3});
    }
    | ABS LPAREN exp RPAREN {
        $$ = drv.tree.ast_insert<ast::abs_t>(std::vector<ast::node_t*>{$3});
    }
    | GET {
        $$ = drv.tree.ast_insert<ast::get_t>();
    }
    | LPAREN exp RPAREN { $$ = $2; }
    | MINUS exp %prec UMINUS { 
        $$ = drv.tree.ast_insert<ast::minus_t>(std::vector<ast::node_t*>{$2});
    }
    | PLUS exp %prec UMINUS {
        $$ = drv.tree.ast_insert<ast::plus_t>(std::vector<ast::node_t*>{$2});
    }
    | NOT exp %prec UMINUS {
        $$ = drv.tree.ast_insert<ast::not_t>(std::vector<ast::node_t*>{$2});
    }
    | TILDA exp %prec UMINUS {
        $$ = drv.tree.ast_insert<ast::b_not_t>(std::vector<ast::node_t*>{$2});
    }
    | D_PLUS exp %prec UMINUS {
        $$ = drv.tree.ast_insert<ast::pr_increment_t>(std::vector<ast::node_t*>{$2});
    }
    | D_MINUS exp %prec UMINUS {
        $$ = drv.tree.ast_insert<ast::pr_decrement_t>(std::vector<ast::node_t*>{$2});
    }
    | NUMBER { 
        $$ = drv.tree.ast_insert($1);
    }
    | NAME { 
        if ($1->is_init == 0) {
            std::cout << yy::red << "Error:" << yy::norm << @1 << ": uninitialized variable" << std::endl;
            is_error = 1;
        }
        if (std::get<int>($1->value) >= 0) {
            std::cout << yy::red << "Error:" << yy::norm << @1 << ": " << $1->name << " incorrect call" << std::endl;
            is_error = 1;
        }
        else {
            $$ = drv.tree.ast_insert<ast::scalar_variable>($1->name);
        }
    }
    | NAME LPAREN init_arg_list RPAREN {
        if ($1->is_init == 0) {
            std::cout << yy::red << "Error:" << yy::norm << @1 << ": uninitialized function" << std::endl;
            is_error = 1;
        }
        if (std::get<int>($1->value) < 0) {
            std::cout << yy::red << "Error:" << yy::norm << @1 << ": scalar type call" << std::endl;
            is_error = 1;
        }
        if (static_cast<unsigned>(std::get<int>($1->value)) != init_arg_size) {
            std::cout << yy::red << "Error:" << yy::norm << @1 << ": mismatch with function signature" << std::endl;
            is_error = 1;
        }
        auto func_init = drv.tree.ast_insert<ast::func_init>($1->name);
        auto recover_scopes = drv.tree.ast_insert<ast::recover_scopes>();

        if (init_arg_size)
            $$ = drv.tree.ast_insert<ast::block>(std::vector<ast::node_t*>{$3, func_init, recover_scopes});
        else 
            $$ = drv.tree.ast_insert<ast::block>(std::vector<ast::node_t*>{func_init, recover_scopes});
        init_arg_size = 0;
    }
    | ERR {
        std::cout << yy::red << "Error:" << yy::norm << @1 << ": unknown operation" << std::endl;
        is_error = 1;
    }
    ;

io_func: PRINT exp  { $$ = drv.tree.ast_insert<ast::print_t>(std::vector<ast::node_t*>{$2}); }
       | RETURN exp { 
           if (drv.scopes.is_global()) {
                std::cout << yy::red << "Error:" << yy::norm << @1 << ": can't return from global scope" << std::endl;
                is_error = 1;
           }
           $$ = drv.tree.ast_insert<ast::return_t>(std::vector<ast::node_t*>{$2}); 
       }
       ;

program: list END {
        if (!drv.tree.is_valid()) {
            std::cout << yy::red << "Error:" << yy::norm << @2 << ": empty AST" << std::endl;
            return 0;
        }
        if (is_error) return 0;
        #ifdef DUMP
            drv.tree.dump();
            int err = system("dot -Tpng tree_dump.dot -o image.png");
            if (err < 0) {
                std::cout << yy::red << "Error:" << yy::norm << @2 << ": can't execute \"dot -Tpng tree_dump.dot -o image.png\"" << std::endl;
                return 0;
            }
        #endif
        drv.tree.IR_generate();
        int err = system("gcc output.o -o prog");
            if (err < 0) {
                std::cout << yy::red << "Error:" << yy::norm << @2 << ": can't execute \"dot -Tpng tree_dump.dot -o image.png\"" << std::endl;
                return 0;
            }
        }
    ;

%%

namespace yy {

parser::token_type yylex(parser::semantic_type *yylval, parser::location_type* location, driver_t &drv) {
    return drv.yylex(yylval, location);
}

void parser::error(const parser::location_type& location, const std::string& what) {
    std::cout << yy::red << "Error:" << yy::norm << location << ' ' << what << std::endl;
    is_error = 1;
}

} // yy
