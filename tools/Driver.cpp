#include "tiny-c/Parser/Parser.h"
#include "tiny-c/CodeGen/CodeGen.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FileSystem.h"

static llvm::cl::opt<std::string> InputFile(
  llvm::cl::Positional, // not named, not specified with a hyphen
  llvm::cl::desc("<input file>"),
  llvm::cl::Required
);

bool emit(llvm::StringRef argv0, llvm::Module *M, llvm::StringRef InputFilename) {
  std::string OutputFilename;
  if (InputFilename.endswith(".c")) {
    OutputFilename = InputFilename.drop_back(4).str();
    OutputFilename.append(".ll");
  }

  // Open output file.
  std::error_code EC;
  llvm::sys::fs::OpenFlags OpenFlags = llvm::sys::fs::OF_Text;
  llvm::raw_fd_ostream Out(OutputFilename, EC, OpenFlags);
  if (EC) {
    llvm::errs() << argv0 << ": " << EC.message() << "\n";
    return false;
  }
  M->print(Out, nullptr);
  
  return true;
}

int main(int argc, const char **argv) {
  llvm::InitLLVM X(argc, argv);
  llvm::cl::ParseCommandLineOptions(argc, argv, "Tiny-C - LLVM Frontend for Tiny-C");

  llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> FileOrErr = llvm::MemoryBuffer::getFile(InputFile);
  if (std::error_code BufferError = FileOrErr.getError()) {
    llvm::errs() << "Error reading" << InputFile << ": " << BufferError.message() << "\n";
  }
  llvm::StringRef Source = FileOrErr.get()->getBuffer();

  Lexer Lex(Source);
  Parser Parser(Lex);
  //TranslationUnitDecl *TransUnit = Parser.parse();
  //CodeGen CodeGenerator;
  //std::unique_ptr<llvm::Module> M = CodeGenerator.compile(Stmts);
  //emit(argv[0], M.get(), InputFile);
}