#include "driver.hpp"

int main() {
  FlexLexer *lexer = new yyFlexLexer;
  yy::driver_t driver(lexer);
  driver.parse();
  delete lexer;
}