#include "ast/ast.h"
#include "codegen/codegen.h"
#include "parser/parser.h"
#include "resolution/resolve.h"

#include <fstream>
#include <iostream>

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
  pass::resolution::Resolve resolver(ast);
  if(!resolver.resolve()) {
    std::cerr << "Failed to resolve:\n";
    for(auto e : resolver.errors()) {
      std::cerr << "  " << e << std::endl;
    }
  }

  // codegen

  codegen::LLVMCodegen codegenObject;

  auto ir = codegenObject.doCodegen(ast);
  std::cout << ir << std::endl;

  // write ir to file
  std::ofstream test("test.ll");
  test << ir << std::endl;
}
