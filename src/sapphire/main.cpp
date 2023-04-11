#include "ast/ast.h"
#include "ast/visitors/ast-dump.h"
#include "codegen/codegen.h"
#include "parser/parser.h"
#include "resolution/type-resolution.h"

#include <fstream>

int main(int argc, const char** argv, [[maybe_unused]] const char** envp) {

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

  // unresolved code
  ast->accept(new ast::visitor::ASTDump(std::cout));
  std::cout << std::endl;
  std::cout << std::string(80, '=') << std::endl;

  // resolve types
  ast->accept(new pass::resolution::TypeResolutionPass());

  // resolved code
  ast->accept(new ast::visitor::ASTDump(std::cout));
  std::cout << std::endl;
  std::cout << std::string(80, '=') << std::endl;

  // codegen

  codegen::LLVMCodegen codegenObject;

  auto ir = codegenObject.doCodegen(ast);
  std::cout << ir << std::endl;

  // write ir to file
  std::ofstream test("test.ll");
  test << ir << std::endl;
}
