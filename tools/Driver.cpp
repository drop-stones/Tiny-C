#include "tiny-c/Parser/Parser.h"
#include "tiny-c/CodeGen/CodeGen.h"

llvm::StringRef expr = "1 + 2 * 3";

int main() {
  Lexer Lex(expr);
  Parser Parser(Lex);
  AST *Tree = Parser.parse();
  CodeGen CodeGenerator;
  CodeGenerator.compile(Tree);
}