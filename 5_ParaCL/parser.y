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
%nterm <ast::node_t*> if_body

%left GRATER LESS EQUAL NEQUAL GEQUAL LEQUAL
%right ASSIGN
%left PLUS MINUS
%left STAR SLASH
%left POW
%nonassoc UMINUS

%start program

%% 

stmt: if_stmt
    | IF LPAREN exp RPAREN if_body ELSE if_stmt { 
        ast::if_t node(std::make_unique<ast::flow_dump>("if"), $3, $5, $7);
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | WHILE LPAREN exp RPAREN LUNICORN list RUNICORN { 
        ast::while_t node(std::make_unique<ast::flow_dump>("while"), $3, $6, nullptr);
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | WHILE LPAREN exp RPAREN line { 
        ast::while_t node(std::make_unique<ast::flow_dump>("while"), $3, $5, nullptr);
        $$ = drv.tree.ast_insert(std::move(node));
    }
    ;

if_stmt: IF LPAREN exp RPAREN if_body {
           ast::if_t node(std::make_unique<ast::flow_dump>("if"), $3, $5, nullptr);
           $$ = drv.tree.ast_insert(std::move(node)); 
       }
       | IF LPAREN exp RPAREN if_body ELSE if_body { 
           ast::if_t node(std::make_unique<ast::flow_dump>("if"), $3, $5, $7);
           $$ = drv.tree.ast_insert(std::move(node));
       }

if_body: line { $$ = $1; }
       | LUNICORN list RUNICORN { $$ = $2; }

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
    | exp ASSIGN exp {
        if ($1->get_type() != ast::node_type::VARIABLE)
            std::cout << yy::red << "Error:" << yy::norm << @2 << ": assign to nonvariable type" << std::endl;
        ast::assign_t node{std::make_unique<ast::binop_dump>("="), $1, $3};
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
    | NUMBER { 
        ast::num_t node{$1, std::make_unique<ast::num_dump>(),};
        $$ = drv.tree.ast_insert<ast::num_t>(std::move(node));
    }
    | NAME { 
        ast::variable_t node(*$1, std::make_unique<ast::variable_dump>());
        $$ = drv.tree.ast_insert<ast::variable_t>(std::move(node));
    }
    ;

io_func: PRINT exp {
        ast::print_t node{std::make_unique<ast::func_dump>("print"), $2};
        $$ = drv.tree.ast_insert(std::move(node));
    }
    | SCAN exp {
        if ($2->get_type() != ast::node_type::VARIABLE)
            std::cout << yy::red << "Error:" << yy::norm << @2 << ": scan with nonvariable type" << std::endl;
        ast::scan_t node{std::make_unique<ast::func_dump>("scan"), $2};
        $$ = drv.tree.ast_insert(std::move(node));
    }

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