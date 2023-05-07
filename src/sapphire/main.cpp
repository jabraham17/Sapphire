#include "ast/ast.h"
#include "codegen/codegen.h"
#include "common/argparse.hpp"
#include "common/debug.h"
#include "parser/parser.h"
#include "passes/resolve.h"
#include "passes/simple-error-checking.h"

#include <fstream>
#include <iostream>
#include <string>

argparse::ArgumentParser get_args(int argc, const char** argv) {
  argparse::ArgumentParser program_args(
      {},
      {},
      argparse::default_arguments::help);
  program_args.add_argument("file").help("Sapphire source file");

  program_args.add_argument("--output", "-o")
      .default_value(std::string("a.out"))
      .help("output file");

#if defined(DEBUG) && DEBUG == 1
  // if the debug arg is unknown, this WILL fail silently
  // this is currently the desired behavior
  program_args.add_argument("--debug")
      .append()
      .action([](const std::string& value) {
        return common::debug::DebugType(value);
      })
      .help("enable debug printing for the selected module(s)");
#endif

  try {
    program_args.parse_args(argc, argv);
  } catch(const std::runtime_error& err) {
    std::cerr << "Bad arguments: " + std::string(err.what()) + "\n" +
                     program_args.help().str()
              << "\n";
    exit(1);
  }
  return program_args;
}

int main(int argc, const char** argv, [[maybe_unused]] const char** envp) {

  auto args = get_args(argc, argv);
  auto filename = args.get<std::string>("file");
  auto outfile = args.get<std::string>("--output");
  // auto outfile = "a.out";

  FILE* fp = fopen(filename.c_str(), "r");
  if(fp == nullptr) {
    std::cerr << "no file found\n";
    return 1;
  }

  auto ast = parser::parse(fp);
  fclose(fp);

  if(ast == nullptr) return 1;

  pass::resolution::Resolve resolver(ast);
  if(resolver.run()) {
    std::cerr << "Failed to resolve:\n";
    for(auto e : resolver.errors()) {
      std::cerr << "  " << e << std::endl;
    }
    return 1;
  }

  pass::error_checking::SimpleErrorChecking simpleErrors(ast);
  if(simpleErrors.run()) {
    std::cerr << "Errors:\n";
    for(auto e : simpleErrors.errors()) {
      std::cerr << "  " << e << std::endl;
    }
    return 1;
  }

  // codegen

  codegen::LLVMCodegen codegenObject(ast);
  if(codegenObject.run()) {
    std::cerr << "Codegen errors:\n";
    for(auto e : codegenObject.errors()) {
      std::cerr << "  " << e << std::endl;
    }
    return 1;
  }

#if defined(DEBUG) && DEBUG == 1

  auto ir = codegenObject.getIR();
  std::cout << ir << std::endl;

  // write ir to file
  std::ofstream test(outfile + ".ll");
  test << ir << std::endl;
#endif

  codegenObject.buildExecutable(outfile);
}
