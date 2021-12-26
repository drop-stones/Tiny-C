#ifndef TINY_C_CODE_GEN_H
#define TINY_C_CODE_GEN_H

#include "tiny-c/AST/AST.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"

class CodeGen {
  llvm::LLVMContext Ctx;

public:
  std::unique_ptr<llvm::Module> compile(StmtList *Stmts);
};

#endif