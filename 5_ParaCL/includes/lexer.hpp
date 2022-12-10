#pragma once

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include <fstream>

#include "parser.tab.hh"
#include "location.hh"

namespace yy{

class lexer_t : public yyFlexLexer{
public:
    yy::location location{};
   
    lexer_t(std::ifstream *in) : yyFlexLexer(in) {}
  
    int yylex() override;
};

} // yy