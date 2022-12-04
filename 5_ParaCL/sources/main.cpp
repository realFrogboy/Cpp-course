#include "driver.hpp"

int main() {
  yy::lexer_t *lexer = new yy::lexer_t;
  yy::driver_t driver(lexer);
  driver.parse();
  system("dot -Tpng tree_dump.dot -o image.png");
  delete lexer;
}