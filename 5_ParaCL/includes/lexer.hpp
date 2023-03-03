#pragma once

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include <fstream>

#include "location.hh"

namespace yy{

struct lexer_t : yyFlexLexer {
    yy::location location{};

    lexer_t(std::ifstream *in) : yyFlexLexer(in) {}
    int yylex() override;
};

} // yy
