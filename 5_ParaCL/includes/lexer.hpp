#pragma once

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "parser.tab.hh"
#include "location.hh"

namespace yy{

class lexer_t : public yyFlexLexer{
public:
    yy::location location{};
   
   lexer_t() : yyFlexLexer() {}
  
   //get rid of override virtual function warning
   using FlexLexer::yylex;

   int yylex() override;
   // YY_DECL defined in mc_lexer.l
   // Method body created by flex in mc_lexer.yy.cc

};

} // yy