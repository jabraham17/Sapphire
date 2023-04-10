#include "ast/ast.h"
#include "ast/visitors/dump-ast.h"
#include "codegen/codegen.h"
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
  auto ast = parser::parse(fp);
  if(needToClose) fclose(fp);

  if(ast == nullptr) return 1;

  // ast->accept(new ast::visitor::DumpAST(std::cout));
  // std::cout << std::endl;

  auto ir = codegen::doCodegen(ast);
  std::cout << ir << std::endl;
}
