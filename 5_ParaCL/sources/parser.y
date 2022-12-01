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
    MODULE  "|"
;


%token <int> NUMBER
%token <name_t*> NAME
%token <int> FUNC
%nterm <int> exp

%left GRATER LESS EQUAL NEQUAL GEQUAL LEQUAL
%right ASSIGN
%left PLUS MINUS
%left STAR SLASH
%left POW
%nonassoc MODULE UMINUS

%start program

%% 

program: list
    ;

list: list exp SCOLON { }
    | %empty
    ;

exp:  exp GRATER exp { $$ = ($1 > $3); }
    | exp LESS exp { $$ = ($1 < $3); }
    | exp EQUAL exp { $$ = ($1 == $3); }
    | exp NEQUAL exp { $$ = ($1 != $3); }
    | exp GEQUAL exp { $$ = ($1 >= $3); }
    | exp LEQUAL exp { $$ = ($1 <= $3); }
    | exp PLUS exp { $$ = $1 + $3; }
    | exp MINUS exp { $$ = $1 - $3;}
    | exp STAR exp { $$ = $1 * $3; }
    | exp SLASH exp { $$ = $1 / $3; }
    | exp POW exp { 
        int res = 1;
        for (int cnt = 0; cnt < $3; ++cnt)
            res *= $1;
        $$ = res;
    }
    | MODULE exp { $$ = std::abs($2); }
    | LPAREN exp RPAREN { $$ = $2; }
    | MINUS exp %prec UMINUS { $$ = -$2; }
    | NUMBER { $$ = $1; }
    | NAME { $$ = $1->value; }
    | NAME ASSIGN exp { $1->value = $3; }
    | FUNC LPAREN exp RPAREN { 
        switch($1) {
            case 1: std::cout << $3 << std::endl;
        }
    }
    | ERR { std::cout << "BAD INPUT" << std::endl; }
    ;

%%

namespace yy {

parser::token_type yylex(parser::semantic_type *yylval, driver_t &drv) {
    return drv.yylex(yylval);
}

void parser::error(const std::string&){}

}