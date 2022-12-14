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

    LPAREN  "("
    RPAREN  ")"
    LUNICORN "{"
    RUNICORN "}"
    SCOLON  ";"

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

    ASSIGN      "="
    PLUS_A      "+="
    MINUS_A     "-="
    STAR_A      "*="
    SLASH_A     "/="
    PERSENT_A   "%="
    L_SHIFT_A   "<<="
    R_SHIFT_A   ">>="
    AMPERSAND_A "&="
    STICK_A     "|="
    CAP_A       "^="

    IF      "if"
    ELSE    "else"
    WHILE   "while"

    PRINT   "print"
    SCAN    "scan"
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
%nterm <ast::node_t*> if_stmt
%nterm <ast::node_t*> while_stmt
%nterm <ast::node_t*> body

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

stmt: if_stmt
    | while_stmt
    | IF LPAREN exp RPAREN body ELSE stmt { 
        ast::if_t node(std::make_unique<ast::flow_dump>("if"), $3, $5, $7);
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | IF LPAREN exp RPAREN while_stmt ELSE stmt { 
           ast::if_t node(std::make_unique<ast::flow_dump>("if"), $3, $5, $7);
           $$ = drv.tree.ast_insert(std::move(node));
       }
    | WHILE LPAREN exp RPAREN stmt { 
        ast::while_t node(std::make_unique<ast::flow_dump>("while"), $3, $5, nullptr);
        $$ = drv.tree.ast_insert(std::move(node));
    }
    ;

if_stmt: IF LPAREN exp RPAREN body {
           ast::if_t node(std::make_unique<ast::flow_dump>("if"), $3, $5, nullptr);
           $$ = drv.tree.ast_insert(std::move(node)); 
       }
       | IF LPAREN exp RPAREN body ELSE body { 
           ast::if_t node(std::make_unique<ast::flow_dump>("if"), $3, $5, $7);
           $$ = drv.tree.ast_insert(std::move(node));
       }
       | IF LPAREN exp RPAREN while_stmt { 
           ast::if_t node(std::make_unique<ast::flow_dump>("if"), $3, $5, nullptr);
           $$ = drv.tree.ast_insert(std::move(node));
       }
       | IF LPAREN exp RPAREN while_stmt ELSE body { 
           ast::if_t node(std::make_unique<ast::flow_dump>("if"), $3, $5, $7);
           $$ = drv.tree.ast_insert(std::move(node));
       }
       ;

while_stmt: WHILE LPAREN exp RPAREN body { 
               ast::while_t node(std::make_unique<ast::flow_dump>("while"), $3, $5, nullptr);
               $$ = drv.tree.ast_insert(std::move(node));
           }
           ;

body: line { $$ = $1; }
    | LUNICORN list RUNICORN { $$ = $2; }
    ;

list: { $$ = nullptr; }
    | stmt list {
        if ($2 == nullptr)
            $$ = $1;
        else {
            ast::scolon_t node{std::make_unique<ast::binop_dump>(";"), $1, $2};
            $$ = drv.tree.ast_insert(std::move(node));
        }
    }
    | line list {
        if ($2 == nullptr)
            $$ = $1;
        else {
            ast::scolon_t node{std::make_unique<ast::binop_dump>(";"), $1, $2};
            $$ = drv.tree.ast_insert(std::move(node));
        }
    }
    | error SCOLON list { yyerrok; }
    ;

line: exp SCOLON     { $$ = $1; }
    | io_func SCOLON { $$ = $1; }
    ;

exp:  exp GRATER exp {
        ast::g_t node{std::make_unique<ast::binop_dump>(">"), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp LESS exp { 
        ast::l_t node{std::make_unique<ast::binop_dump>("<"), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp EQUAL exp { 
        ast::e_t node{std::make_unique<ast::binop_dump>("=="), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp NEQUAL exp { 
        ast::ne_t node{std::make_unique<ast::binop_dump>("!="), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp GEQUAL exp { 
        ast::ge_t node{std::make_unique<ast::binop_dump>(">="), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp LEQUAL exp { 
        ast::le_t node{std::make_unique<ast::binop_dump>("<="), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp PLUS exp { 
        ast::add_t node{std::make_unique<ast::binop_dump>("+"), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp MINUS exp { 
        ast::sub_t node{std::make_unique<ast::binop_dump>("-"), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp STAR exp { 
        ast::mul_t node{std::make_unique<ast::binop_dump>("*"), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp SLASH exp { 
        ast::div_t node{std::make_unique<ast::binop_dump>("/"), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp PERSENT exp { 
        ast::remainder_t node{std::make_unique<ast::binop_dump>("%"), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp POW exp { 
        ast::pow_t node{std::make_unique<ast::binop_dump>("**"), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp ASSIGN exp {
        if ($1->get_type() != ast::node_type::VARIABLE) {
            std::cout << yy::red << "Error:" << yy::norm << @2 << ": assign to nonvariable type" << std::endl;
            is_error = 1;
        }
        ast::assign_t node{std::make_unique<ast::binop_dump>("="), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp PLUS_A exp {
        if ($1->get_type() != ast::node_type::VARIABLE) {
            std::cout << yy::red << "Error:" << yy::norm << @2 << ": assign to nonvariable type" << std::endl;
            is_error = 1;
        }
        ast::add_a_t node{std::make_unique<ast::binop_dump>("+="), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp MINUS_A exp {
        if ($1->get_type() != ast::node_type::VARIABLE) {
            std::cout << yy::red << "Error:" << yy::norm << @2 << ": assign to nonvariable type" << std::endl;
            is_error = 1;
        }
        ast::sub_a_t node{std::make_unique<ast::binop_dump>("-="), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp STAR_A exp {
        if ($1->get_type() != ast::node_type::VARIABLE) {
            std::cout << yy::red << "Error:" << yy::norm << @2 << ": assign to nonvariable type" << std::endl;
            is_error = 1;
        }
        ast::mult_a_t node{std::make_unique<ast::binop_dump>("*="), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp SLASH_A exp {
        if ($1->get_type() != ast::node_type::VARIABLE) {
            std::cout << yy::red << "Error:" << yy::norm << @2 << ": assign to nonvariable type" << std::endl;
            is_error = 1;
        }
        ast::div_a_t node{std::make_unique<ast::binop_dump>("/="), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp PERSENT_A exp {
        if ($1->get_type() != ast::node_type::VARIABLE) {
            std::cout << yy::red << "Error:" << yy::norm << @2 << ": assign to nonvariable type" << std::endl;
            is_error = 1;
        }
        ast::remainder_a_t node{std::make_unique<ast::binop_dump>("%="), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp L_SHIFT_A exp {
        if ($1->get_type() != ast::node_type::VARIABLE) {
            std::cout << yy::red << "Error:" << yy::norm << @2 << ": assign to nonvariable type" << std::endl;
            is_error = 1;
        }
        ast::l_shift_a_t node{std::make_unique<ast::binop_dump>("<<="), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp R_SHIFT_A exp {
        if ($1->get_type() != ast::node_type::VARIABLE) {
            std::cout << yy::red << "Error:" << yy::norm << @2 << ": assign to nonvariable type" << std::endl;
            is_error = 1;
        }
        ast::r_shift_a_t node{std::make_unique<ast::binop_dump>(">>="), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp AMPERSAND_A exp {
        if ($1->get_type() != ast::node_type::VARIABLE) {
            std::cout << yy::red << "Error:" << yy::norm << @2 << ": assign to nonvariable type" << std::endl;
            is_error = 1;
        }
        ast::b_and_a_t node{std::make_unique<ast::binop_dump>("<<="), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp STICK_A exp {
        if ($1->get_type() != ast::node_type::VARIABLE) {
            std::cout << yy::red << "Error:" << yy::norm << @2 << ": assign to nonvariable type" << std::endl;
            is_error = 1;
        }
        ast::b_or_a_t node{std::make_unique<ast::binop_dump>("<<="), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp CAP_A exp {
        if ($1->get_type() != ast::node_type::VARIABLE) {
            std::cout << yy::red << "Error:" << yy::norm << @2 << ": assign to nonvariable type" << std::endl;
            is_error = 1;
        }
        ast::xor_a_t node{std::make_unique<ast::binop_dump>("<<="), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp AMPERSAND exp {
        ast::b_and_t node{std::make_unique<ast::binop_dump>("&"), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp STICK exp {
        ast::b_or_t node{std::make_unique<ast::binop_dump>("|"), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp CAP exp {
        ast::xor_t node{std::make_unique<ast::binop_dump>("^"), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp L_SHIFT exp {
        ast::l_shift_t node{std::make_unique<ast::binop_dump>("<<"), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp R_SHIFT exp {
        ast::r_shift_t node{std::make_unique<ast::binop_dump>(">>"), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp D_AMPERSAND exp {
        ast::l_and_t node{std::make_unique<ast::binop_dump>("&&"), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | exp D_STICK exp {
        ast::l_or_t node{std::make_unique<ast::binop_dump>("||"), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | ABS LPAREN exp RPAREN {
        ast::abs_t node{std::make_unique<ast::func_dump>("abs"), $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | GET {
        ast::get_t node{std::make_unique<ast::func_dump>("?")};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | LPAREN exp RPAREN { $$ = $2; }
    | MINUS exp %prec UMINUS { 
        ast::minus_t node{std::make_unique<ast::unop_dump>("-"), $2};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | PLUS exp %prec UMINUS {
        ast::plus_t node{std::make_unique<ast::unop_dump>("+"), $2};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | NOT exp %prec UMINUS {
        ast::not_t node{std::make_unique<ast::unop_dump>("!"), $2};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | TILDA exp %prec UMINUS {
        ast::b_not_t node{std::make_unique<ast::unop_dump>("~"), $2};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | D_PLUS exp %prec UMINUS {
        ast::pr_increment_t node{std::make_unique<ast::unop_dump>("++"), $2};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | D_MINUS exp %prec UMINUS {
        ast::pr_decrement_t node{std::make_unique<ast::unop_dump>("--"), $2};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | NUMBER { 
        ast::num_t node{$1, std::make_unique<ast::num_dump>(),};
        $$ = drv.tree.ast_insert<ast::num_t>(std::move(node));
    }
    | NAME { 
        ast::variable_t node(*$1, std::make_unique<ast::variable_dump>());
        $$ = drv.tree.ast_insert<ast::variable_t>(std::move(node));
    }
    | ERR {
        std::cout << yy::red << "Error:" << yy::norm << @1 << ": unknown operation" << std::endl;
        is_error = 1;
    }
    ;

io_func: PRINT exp {
        ast::print_t node{std::make_unique<ast::func_dump>("print"), $2};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | SCAN exp {
        if ($2->get_type() != ast::node_type::VARIABLE) {
            std::cout << yy::red << "Error:" << yy::norm << @2 << ": scan with nonvariable type" << std::endl;
            is_error = 1;
        }
        ast::scan_t node{std::make_unique<ast::func_dump>("scan"), $2};
        $$ = drv.tree.ast_insert(std::move(node));
    }

program: list END {
        if (is_error) return 0;
        #ifdef DUMP
            drv.tree.dump();
            system("dot -Tpng tree_dump.dot -o image.png");
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

}