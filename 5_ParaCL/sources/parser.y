%skeleton "lalr1.cc"
%require "3.5.1"
%language "c++"

%define api.value.type variant

%param { yy::driver_t &drv }

%code requires {
#include <iostream>
#include <string>
#include <cmath>

#include "paracl.hpp"

namespace yy { class driver_t; }
}

%code {
#include "driver.hpp"
namespace yy { parser::token_type yylex(parser::semantic_type* yylval, driver_t &driver); }
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
%token <name_t*> NAME
%token <int> FUNC
%nterm <node_t*> exp
%nterm <node_t*> list
%nterm <node_t*> stmt

%left GRATER LESS EQUAL NEQUAL GEQUAL LEQUAL
%right ASSIGN
%left PLUS MINUS
%left STAR SLASH
%left POW
%nonassoc UMINUS

%start program

%% 

stmt: IF LPAREN exp RPAREN LUNICORN list RUNICORN { $$ = new flow_t (flow_type::IF, $3, $6, nullptr); }
    | IF LPAREN exp RPAREN LUNICORN list RUNICORN ELSE LUNICORN list RUNICORN { $$ = new flow_t (flow_type::IF, $3, $6, $10); }
    | WHILE LPAREN exp RPAREN LUNICORN list RUNICORN { $$ = new flow_t (flow_type::WHILE, $3, $6, nullptr); }
    ;

list: { $$ = nullptr; }
    | stmt list {
        if ($2 == nullptr)
            $$ = $1;
        else
            $$ = new binop_t{binop_type::SCOLON, $1, $2};
    }
    | exp SCOLON list {
        if ($3 == nullptr)
            $$ = $1;
        else
            $$ = new binop_t{binop_type::SCOLON, $1, $3};
    }
    ;

exp:  exp GRATER exp { $$ = new binop_t{binop_type::G, $1, $3}; }
    | exp LESS exp { $$ = new binop_t{binop_type::L, $1, $3}; }
    | exp EQUAL exp { $$ = new binop_t{binop_type::E, $1, $3}; }
    | exp NEQUAL exp { $$ = new binop_t{binop_type::NE, $1, $3}; }
    | exp GEQUAL exp { $$ = new binop_t{binop_type::GE, $1, $3}; }
    | exp LEQUAL exp { $$ = new binop_t{binop_type::LE, $1, $3}; }
    | exp PLUS exp { $$ = new binop_t{binop_type::ADD, $1, $3}; }
    | exp MINUS exp { $$ = new binop_t{binop_type::SUB, $1, $3};}
    | exp STAR exp { $$ = new binop_t{binop_type::MUL, $1, $3}; }
    | exp SLASH exp { $$ = new binop_t{binop_type::DIV, $1, $3}; }
    | exp POW exp { $$ = new binop_t{binop_type::POW, $1, $3}; }
    | LPAREN exp RPAREN { $$ = $2; }
    | MINUS exp %prec UMINUS { $$ = new unop_t{unop_type::MINUS, $2}; }
    | NUMBER { $$ = new num_t{$1}; }
    | NAME { $$ = new variable_t(*$1); }
    | exp ASSIGN exp { $$ = new binop_t{binop_type::ASSIGN, $1, $3}; }
    | FUNC LPAREN exp RPAREN { $$ = new func_t{$1, $3}; }
    | ERR { std::cout << "BAD INPUT" << std::endl; }
    ;

program: list END {
        tree_t ast{$1};
        ast.dump();
    }
    ;

%%

namespace yy {

parser::token_type yylex(parser::semantic_type *yylval, driver_t &drv) {
    return drv.yylex(yylval);
}

void parser::error(const std::string&){}

}