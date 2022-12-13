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
namespace yy { parser::token_type yylex(parser::semantic_type* yylval, parser::location_type* loc, driver_t &driver); }
}

%token
    ERR
    POW     "**"
    ASSIGN  "="
    MINUS   "-"
    PLUS    "+"
    STAR    "*"
    SLASH   "/"
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

    IF      "if"
    ELSE    "else"
    WHILE   "while"

    PRINT   "print"
    SCAN    "scan"
    ABS     "abs"

    END 0   "end of file"
;


%token <int> NUMBER
%token <ast::name_t*> NAME
%token <int> FUNC
%nterm <ast::node_t*> exp
%nterm <ast::node_t*> list
%nterm <ast::node_t*> stmt

%left GRATER LESS EQUAL NEQUAL GEQUAL LEQUAL
%right ASSIGN
%left PLUS MINUS
%left STAR SLASH
%left POW
%nonassoc UMINUS

%start program

%% 

stmt: IF LPAREN exp RPAREN LUNICORN list RUNICORN {
        ast::if_t node(std::make_unique<ast::flow_dump>("if"), $3, $6, nullptr);
        $$ = drv.tree.ast_insert(std::move(node)); 
    }
    | IF LPAREN exp RPAREN LUNICORN list RUNICORN ELSE LUNICORN list RUNICORN { 
        ast::if_t node(std::make_unique<ast::flow_dump>("if"), $3, $6, $10);
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | WHILE LPAREN exp RPAREN LUNICORN list RUNICORN { 
        ast::while_t node(std::make_unique<ast::flow_dump>("while"), $3, $6, nullptr);
        $$ = drv.tree.ast_insert(std::move(node));
    }
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
    | exp SCOLON list {
        if ($3 == nullptr)
            $$ = $1;
        else {
            ast::scolon_t node{std::make_unique<ast::binop_dump>(";"), $1, $3};
            $$ = drv.tree.ast_insert(std::move(node));
        }
    }
    | error SCOLON list { yyerrok; }
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
    | exp POW exp { 
        ast::pow_t node{std::make_unique<ast::binop_dump>("**"), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | LPAREN exp RPAREN { $$ = $2; }
    | MINUS exp %prec UMINUS { 
        ast::minus_t node{std::make_unique<ast::unop_dump>("-"), $2};
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
    | exp ASSIGN exp {
        if ($1->get_type() != ast::node_type::VARIABLE)
            std::cout << yy::red << "Error:" << yy::norm << @2 << ": assign to nonvariable type" << std::endl;
        ast::assign_t node{std::make_unique<ast::binop_dump>("="), $1, $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | PRINT LPAREN exp RPAREN {
        ast::print_t node{std::make_unique<ast::func_dump>("print"), $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | SCAN LPAREN exp RPAREN {
        if ($3->get_type() != ast::node_type::VARIABLE)
            std::cout << yy::red << "Error:" << yy::norm << @2 << ": scan with nonvariable type" << std::endl;
        ast::scan_t node{std::make_unique<ast::func_dump>("scan"), $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | ABS LPAREN exp RPAREN {
        ast::abs_t node{std::make_unique<ast::func_dump>("abs"), $3};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | ERR { std::cout << yy::red << "Error:" << yy::norm << @1 << ": incorrect input" << std::endl; }
    ;

program: list END {
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
}

}