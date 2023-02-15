%option c++
%option noyywrap nodefault yylineno
%option yyclass="yy::lexer_t"

%{
#include "parser.tab.hh"
#include "lexer.hpp"

#define YY_USER_ACTION location.step(); location.columns(yyleng);
%}

%%
%{
location.step();
%}

"+"  return yy::parser::token_type::PLUS;
"-"  return yy::parser::token_type::MINUS;
"*"  return yy::parser::token_type::STAR;
"/"  return yy::parser::token_type::SLASH;
"%"  return yy::parser::token_type::PERSENT;
"**" return yy::parser::token_type::POW;

";"  return yy::parser::token_type::SCOLON;
"("  return yy::parser::token_type::LPAREN;
")"  return yy::parser::token_type::RPAREN;
"{"  return yy::parser::token_type::LUNICORN;
"}"  return yy::parser::token_type::RUNICORN;

"&"  return yy::parser::token_type::AMPERSAND;
"|"  return yy::parser::token_type::STICK;
"^"  return yy::parser::token_type::CAP;
"~"  return yy::parser::token_type::TILDA;
"<<" return yy::parser::token_type::L_SHIFT;
">>" return yy::parser::token_type::R_SHIFT;

"&&" return yy::parser::token_type::D_AMPERSAND;
"||" return yy::parser::token_type::D_STICK;

"++" return yy::parser::token_type::D_PLUS;
"--" return yy::parser::token_type::D_MINUS;

"="  return yy::parser::token_type::ASSIGN;

">"  return yy::parser::token_type::GRATER;
"<"  return yy::parser::token_type::LESS;
"!=" return yy::parser::token_type::NEQUAL;
"==" return yy::parser::token_type::EQUAL;
">=" return yy::parser::token_type::GEQUAL;
"<=" return yy::parser::token_type::LEQUAL;

"!"  return yy::parser::token_type::NOT;

"if"    return yy::parser::token_type::IF;
"else"  return yy::parser::token_type::ELSE;
"while" return yy::parser::token_type::WHILE;

"print" return yy::parser::token_type::PRINT;
"abs"   return yy::parser::token_type::ABS;
"?"     return yy::parser::token_type::GET;

[a-zA-Z][a-zA-Z0-9]* return yy::parser::token_type::NAME;
[0-9]+ return yy::parser::token_type::NUMBER;
"//".*
[ \t\v\r] { location.step(); }
\n+  { location.lines(yyleng); location.step(); }
.  return yy::parser::token_type::ERR;
<<EOF>> return yy::parser::token_type::END;

%%