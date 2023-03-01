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
#include <cstring>
#include <memory>
#include <cmath>

#include "ast.hpp"

namespace yy { class driver_t; }
}

%code {
#include "driver.hpp"
namespace yy { 
    parser::token_type yylex(parser::semantic_type* yylval, parser::location_type* loc, driver_t &driver); 
    bool is_error = 0;
}
}

%token
    ERR     "UNKNOWN OPERATION"

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

    PRINT   "print"
    ABS     "abs"
    GET     "?"

    END 0   "end of file"
;

%token <int> NUMBER
%token <ast::name_t*> NAME
%nterm <ast::node_t*> exp
%nterm <ast::node_t*> list
%nterm <ast::node_t*> stmt
%nterm <ast::node_t*> io_func
%nterm <ast::node_t*> line
%nterm <ast::node_t*> body
%nterm <ast::node_t*> block
%nterm arg_list
%nterm init_arg_list

%nonassoc RPAREN
%nonassoc ELSE

%left GRATER LESS EQUAL NEQUAL GEQUAL LEQUAL
%right ASSIGN PLUS_A MINUS_A STAR_A SLASH_A PERSENT_A L_SHIFT_A R_SHIFT_A AMPERSAND_A STICK_A CAP_A
%left D_STICK
%left D_AMPERSAND
%left STICK
%left CAP
%left AMPERSAND
%left L_SHIFT R_SHIFT
%left PLUS MINUS
%left STAR SLASH PERSENT
%left POW
%nonassoc UMINUS

%start program

%% 

stmt: IF LPAREN exp RPAREN body {
        $$ = drv.tree.ast_insert<ast::if_t>($5, nullptr, $3);
        drv.remove_scope();
    }
    | IF LPAREN exp RPAREN body ELSE body {
        $$ = drv.tree.ast_insert<ast::if_t>($5, $7, $3);
        drv.remove_scope();
    }
    | IF LPAREN exp RPAREN stmt ELSE body {
        $$ = drv.tree.ast_insert<ast::if_t>($5, $7, $3);
        drv.remove_scope();
    }
    | IF LPAREN exp RPAREN body ELSE stmt {
        $$ = drv.tree.ast_insert<ast::if_t>($5, $7, $3);
        drv.remove_scope();
    }
    | WHILE LPAREN exp RPAREN body { 
        $$ = drv.tree.ast_insert<ast::while_t>($5, nullptr, $3);
        drv.remove_scope();
    }
    | IF LPAREN exp RPAREN stmt {
        $$ = drv.tree.ast_insert<ast::if_t>($5, nullptr, $3);
        drv.remove_scope();
    }
    | IF LPAREN exp RPAREN stmt ELSE stmt { 
        $$ = drv.tree.ast_insert<ast::if_t>($5, $7, $3);
        drv.remove_scope();
    }
    | WHILE LPAREN exp RPAREN stmt { 
        $$ = drv.tree.ast_insert<ast::while_t>($5, nullptr, $3);
        drv.remove_scope();
    }
    ;

body: line { $$ = $1; }
    | block { $$ = $1; }
    | END  { $$ = nullptr; }
    ;

block: LUNICORN list RUNICORN {
         $$ = drv.tree.ast_insert<ast::block>($2);
     }
     ;

arg_list:
        | arg_list NAME {
            auto search = drv.find_variable($2->name);
            search->is_init = 1;
            drv.arg_list.push_back($2->name);
        }
        ;

init_arg_list:
             | init_arg_list NUMBER {
                 drv.arg_init_list.push_back($2);
             }
             ;

list: { $$ = nullptr; }
    | stmt list {
        if ($2 == nullptr)
            $$ = $1;
        else {
            $$ = drv.tree.ast_insert<ast::scolon_t>($1, $2);
        }
    }
    | line list {
        if ($2 == nullptr)
            $$ = $1;
        else {
            $$ = drv.tree.ast_insert<ast::scolon_t>($1, $2);
        }
    }
    | error SCOLON list { yyerrok; }
    ;

line: exp SCOLON     { $$ = $1; }
    | io_func SCOLON { $$ = $1; }
    | SCOLON         { $$ = nullptr; }
    ;

exp:  exp GRATER exp {
        $$ = drv.tree.ast_insert<ast::g_t>($1, $3);
    }
    | exp LESS exp { 
        $$ = drv.tree.ast_insert<ast::l_t>($1, $3);
    }
    | exp EQUAL exp { 
        $$ = drv.tree.ast_insert<ast::e_t>($1, $3);
    }
    | exp NEQUAL exp { 
        $$ = drv.tree.ast_insert<ast::ne_t>($1, $3);
    }
    | exp GEQUAL exp { 
        $$ = drv.tree.ast_insert<ast::ge_t>($1, $3);
    }
    | exp LEQUAL exp { 
        $$ = drv.tree.ast_insert<ast::le_t>($1, $3);
    }
    | exp PLUS exp {
        $$ = drv.tree.ast_insert<ast::add_t>($1, $3);
    }
    | exp MINUS exp {
        $$ = drv.tree.ast_insert<ast::sub_t>($1, $3);
    }
    | exp STAR exp { 
        $$ = drv.tree.ast_insert<ast::mul_t>($1, $3);
    }
    | exp SLASH exp { 
        $$ = drv.tree.ast_insert<ast::div_t>($1, $3);
    }
    | exp PERSENT exp {
        $$ = drv.tree.ast_insert<ast::remainder_t>($1, $3);
    }
    | exp POW exp { 
        $$ = drv.tree.ast_insert<ast::pow_t>($1, $3);
    }
    | NAME ASSIGN exp {
        auto search = drv.find_variable($1->name);
        search->is_init = 1;
        ast::node_t *p_node = drv.tree.ast_insert($1->name);
        $$ = drv.tree.ast_insert<ast::assign_t>(p_node, $3);
    }
    | NAME ASSIGN block {
        auto search = drv.find_variable($1->name);
        search->is_init = 1;
        ast::node_t *p_node = drv.tree.ast_insert($1->name);
        $$ = drv.tree.ast_insert<ast::assign_t>(p_node, $3);
    }
    | NAME ASSIGN FUNC LPAREN arg_list RPAREN block {
        drv.recover_scopes();
        auto search = drv.find_variable($1->name);
        search->is_init = 1;
        ast::node_t *p_node = drv.tree.ast_insert($1->name);
        $$ = drv.tree.ast_insert(p_node, ast::func_info{$7, drv.arg_list});
        drv.clear_arg_list();
    }
    | exp AMPERSAND exp {
        $$ = drv.tree.ast_insert<ast::b_and_t>($1, $3);
    }
    | exp STICK exp {
        $$ = drv.tree.ast_insert<ast::b_or_t>($1, $3);
    }
    | exp CAP exp {
        $$ = drv.tree.ast_insert<ast::xor_t>($1, $3);
    }
    | exp L_SHIFT exp {
        $$ = drv.tree.ast_insert<ast::l_shift_t>($1, $3);
    }
    | exp R_SHIFT exp {
        $$ = drv.tree.ast_insert<ast::r_shift_t>($1, $3);
    }
    | exp D_AMPERSAND exp {
        $$ = drv.tree.ast_insert<ast::l_and_t>($1, $3);
    }
    | exp D_STICK exp {
        $$ = drv.tree.ast_insert<ast::l_or_t>($1, $3);
    }
    | ABS LPAREN exp RPAREN {
        $$ = drv.tree.ast_insert<ast::abs_t>($3);
    }
    | GET {
        $$ = drv.tree.ast_insert<ast::get_t>();
    }
    | LPAREN exp RPAREN { $$ = $2; }
    | MINUS exp %prec UMINUS { 
        $$ = drv.tree.ast_insert<ast::minus_t>($2);
    }
    | PLUS exp %prec UMINUS {
        $$ = drv.tree.ast_insert<ast::plus_t>($2);
    }
    | NOT exp %prec UMINUS {
        $$ = drv.tree.ast_insert<ast::not_t>($2);
    }
    | TILDA exp %prec UMINUS {
        $$ = drv.tree.ast_insert<ast::b_not_t>($2);
    }
    | D_PLUS exp %prec UMINUS {
        $$ = drv.tree.ast_insert<ast::pr_increment_t>($2);
    }
    | D_MINUS exp %prec UMINUS {
        $$ = drv.tree.ast_insert<ast::pr_decrement_t>($2);
    }
    | NUMBER { 
        $$ = drv.tree.ast_insert($1);
    }
    | NAME { 
        if ($1->is_init == 0) {
            std::cout << yy::red << "Error:" << yy::norm << @1 << ": uninitialized variable" << std::endl;
            is_error = 1;
        }
        else {
            $$ = drv.tree.ast_insert($1->name);
        }
    }
    | NAME LPAREN init_arg_list RPAREN {
        if ($1->is_init == 0) {
            std::cout << yy::red << "Error:" << yy::norm << @1 << ": uninitialized function" << std::endl;
            is_error = 1;
        }
        else {
            $$ = drv.tree.ast_insert($1->name, drv.arg_init_list);
            drv.clear_arg_init_list();
        }
    }
    | ERR {
        std::cout << yy::red << "Error:" << yy::norm << @1 << ": unknown operation" << std::endl;
        is_error = 1;
    }
    ;

io_func: PRINT exp {
        $$ = drv.tree.ast_insert<ast::print_t>($2);
    }

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
        drv.tree.evaluate();
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