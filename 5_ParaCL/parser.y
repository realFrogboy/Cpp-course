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
#include <string>
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
        ast::flow_t node(ast::flow_type::IF, $3, $6, nullptr);
        $$ = drv.tree.ast_insert<ast::flow_t>(std::move(node)); 
    }
    | IF LPAREN exp RPAREN LUNICORN list RUNICORN ELSE LUNICORN list RUNICORN { 
        ast::flow_t node(ast::flow_type::IF, $3, $6, $10);
        $$ = drv.tree.ast_insert<ast::flow_t>(std::move(node));
    }
    | WHILE LPAREN exp RPAREN LUNICORN list RUNICORN { 
        ast::flow_t node(ast::flow_type::WHILE, $3, $6, nullptr);
        $$ = drv.tree.ast_insert<ast::flow_t>(std::move(node));
    }
    ;

list: { $$ = nullptr; }
    | stmt list {
        if ($2 == nullptr)
            $$ = $1;
        else {
            ast::binop_t node{ast::binop_type::SCOLON, $1, $2};
            $$ = drv.tree.ast_insert<ast::binop_t>(std::move(node));
        }
    }
    | exp SCOLON list {
        if ($3 == nullptr)
            $$ = $1;
        else {
            ast::binop_t node{ast::binop_type::SCOLON, $1, $3};
            $$ = drv.tree.ast_insert<ast::binop_t>(std::move(node));
        }
    }
    | error SCOLON list { yyerrok; }
    ;

exp:  exp GRATER exp {
        ast::binop_t node{ast::binop_type::G, $1, $3};
        $$ = drv.tree.ast_insert<ast::binop_t>(std::move(node));
    }
    | exp LESS exp { 
        ast::binop_t node{ast::binop_type::L, $1, $3};
        $$ = drv.tree.ast_insert<ast::binop_t>(std::move(node));
    }
    | exp EQUAL exp { 
        ast::binop_t node{ast::binop_type::E, $1, $3};
        $$ = drv.tree.ast_insert<ast::binop_t>(std::move(node));
    }
    | exp NEQUAL exp { 
        ast::binop_t node{ast::binop_type::NE, $1, $3};
        $$ = drv.tree.ast_insert<ast::binop_t>(std::move(node));
    }
    | exp GEQUAL exp { 
        ast::binop_t node{ast::binop_type::GE, $1, $3};
        $$ = drv.tree.ast_insert<ast::binop_t>(std::move(node));
    }
    | exp LEQUAL exp { 
        ast::binop_t node{ast::binop_type::LE, $1, $3};
        $$ = drv.tree.ast_insert<ast::binop_t>(std::move(node));
    }
    | exp PLUS exp { 
        ast::binop_t node{ast::binop_type::ADD, $1, $3};
        $$ = drv.tree.ast_insert<ast::binop_t>(std::move(node));
    }
    | exp MINUS exp { 
        ast::binop_t node{ast::binop_type::SUB, $1, $3};
        $$ = drv.tree.ast_insert<ast::binop_t>(std::move(node));
    }
    | exp STAR exp { 
        ast::binop_t node{ast::binop_type::MUL, $1, $3};
        $$ = drv.tree.ast_insert<ast::binop_t>(std::move(node));
    }
    | exp SLASH exp { 
        ast::binop_t node{ast::binop_type::DIV, $1, $3};
        $$ = drv.tree.ast_insert<ast::binop_t>(std::move(node));
    }
    | exp POW exp { 
        ast::binop_t node{ast::binop_type::POW, $1, $3};
        $$ = drv.tree.ast_insert<ast::binop_t>(std::move(node));
    }
    | LPAREN exp RPAREN { $$ = $2; }
    | MINUS exp %prec UMINUS { 
        ast::unop_t node{ast::unop_type::MINUS, $2};
        $$ = drv.tree.ast_insert<ast::unop_t>(std::move(node));
    }
    | NUMBER { 
        ast::num_t node{$1};
        $$ = drv.tree.ast_insert<ast::num_t>(std::move(node));
    }
    | NAME { 
        ast::variable_t node(*$1);
        $$ = drv.tree.ast_insert<ast::variable_t>(std::move(node));
    }
    | exp ASSIGN exp {
        if ($1->get_type() != ast::node_type::VARIABLE)
            std::cout << yy::red << "Error:" << yy::norm << @2 << ": assign to nonvariable type" << std::endl;
        ast::binop_t node{ast::binop_type::ASSIGN, $1, $3};
        $$ = drv.tree.ast_insert<ast::binop_t>(std::move(node));
    }
    | FUNC LPAREN exp RPAREN {
        if ($1 == ast::func_type::SCAN && $3->get_type() != ast::node_type::VARIABLE)
            std::cout << yy::red << "Error:" << yy::norm << @2 << ": scan with nonvariable type" << std::endl;
        ast::func_t node{$1, $3};
        $$ = drv.tree.ast_insert<ast::func_t>(std::move(node));
    }
    | ERR { std::cout << "BAD INPUT" << std::endl; }
    ;

program: list END {
        drv.tree.dump();
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