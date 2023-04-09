#include "ast/ast.h"
#include "ast/visitors/dump-ast.h"
#include "parser/parser.h"

int main(int argc, const char** argv, const char** envp) {

  FILE* fp;
  bool needToClose = false;
  if(argc > 1) {
    fp = fopen(argv[1], "r");
    needToClose = true;
  } else {
    fp = stdin;
  }
  auto a = parser::parse(fp);
  if(needToClose) fclose(fp);

  if(a == nullptr) return 1;

  a->accept(new ast::visitor::DumpAST(std::cout));
  std::cout << std::endl;
}
