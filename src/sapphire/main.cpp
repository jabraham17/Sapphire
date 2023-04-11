#include "ast/ast.h"
#include "ast/visitors/ast-dump.h"
#include "codegen/codegen.h"
#include "parser/parser.h"
#include "resolution/scope-resolve.h"
#include "resolution/type-resolve.h"

#include <fstream>

int main(int argc, const char** argv, [[maybe_unused]] const char** envp) {

  FILE* fp;
  bool needToClose = false;
  if(argc > 1) {
    fp = fopen(argv[1], "r");
    if(fp == nullptr) {
      std::cerr << "no file found\n";
      return 1;
    }
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

  // resolve scopes
  {
    pass::resolution::ScopeResolve sr(ast);
    sr.resolve();
    if(sr.hasErrors()) {
      std::cerr << "Failed to scope resolve\n";
      for(auto e : sr.errors()) {
        std::cerr << e << std::endl;
      }
      return 1;
    }
  }

  // resolve types
  ast->accept(new pass::resolution::TypeResolve());

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
