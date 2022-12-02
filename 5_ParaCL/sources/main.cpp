#include "driver.hpp"

int main() {
  FlexLexer *lexer = new yyFlexLexer;
  yy::driver_t driver(lexer);
  driver.parse();
  system("dot -Tpng tree_dump.dot -o image.png");
  delete lexer;
}