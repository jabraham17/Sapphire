#include "codegen.h"

#include <array>
#include <filesystem>
#include <iostream>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/Process.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>

namespace codegen {
void LLVMCodegen::buildObjectFile(const std::string& Filename) {
  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();

  auto TargetTriple = llvm::sys::getDefaultTargetTriple();
  Module->setTargetTriple(TargetTriple);

  std::string Error;
  auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);

  // Print an error and exit if we couldn't find the requested target.
  // This generally occurs if we've forgotten to initialise the
  // TargetRegistry or we have a bogus target triple.
  if(!Target) {
    std::cerr << Error;
    exit(1);
  }

  auto CPU = "generic";
  auto Features = "";

  llvm::TargetOptions opt;
  auto RM = llvm::Optional<llvm::Reloc::Model>();
  auto TargetMachine =
      Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

  Module->setDataLayout(TargetMachine->createDataLayout());

  std::error_code EC;
  llvm::raw_fd_ostream dest(Filename, EC, llvm::sys::fs::OF_None);

  if(EC) {
    std::cerr << "Could not open file: " << EC.message();
    exit(1);
  }

  llvm::legacy::PassManager pass_manager;
  llvm::PassManagerBuilder pass_manager_builder;
  pass_manager_builder.OptLevel = 0;
  pass_manager_builder.populateModulePassManager(pass_manager);
  auto FileType = llvm::CGFT_ObjectFile;
  if(TargetMachine
         ->addPassesToEmitFile(pass_manager, dest, nullptr, FileType)) {
    std::cerr << "TargetMachine can't emit a file of this type";
    exit(1);
  }

  pass_manager.run(*Module);
  dest.flush();
}
void LLVMCodegen::buildExecutable(const std::string& filename) {

  // object file
  std::string tempFile = std::string(std::tmpnam(nullptr)) + ".o";
  buildObjectFile(tempFile);

  std::string runtimeDir = RUNTIME_LIBRARY_DIR;
  std::string runtimeLib = RUNTIME_LIBRARY_NAME;
  std::string linker = RUNTIME_LINKER_PATH;
  std::string linkerArgs = RUNTIME_LINKER_ARGS;

  std::array args = {
      linkerArgs,
      tempFile,
      std::string("-L") + runtimeDir,
      std::string("-l") + runtimeLib,
      std::string("-static"),
      std::string("-o"),
      filename};
#if defined(DEBUG) && DEBUG == 1
  std::cerr << "linker cmd: '" << linker;
  for(auto a : args) {
    std::cerr << " " << a;
  }
  std::cerr << "'\n";
#endif
  auto args_ref = llvm::SmallVector<llvm::StringRef>(args.begin(), args.end());
  auto ret = llvm::sys::ExecuteAndWait(linker, args_ref);
  if(ret) {
    std::cerr << "failed to link\n";
    exit(1);
  }

  std::filesystem::remove(tempFile);
}
} // namespace codegen
