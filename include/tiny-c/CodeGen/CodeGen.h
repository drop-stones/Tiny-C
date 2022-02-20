#ifndef TINY_C_CODE_GEN_H
#define TINY_C_CODE_GEN_H

#include "tiny-c/AST/AST.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include <unordered_map>

class CodeGen {
  llvm::Module *Mod;
  llvm::LLVMContext &Ctx;
  llvm::IRBuilder<> Builder;
  llvm::Value *V;

  llvm::StringMap<llvm::Function *> FuncMap;
  std::unordered_map<llvm::Function *, llvm::StringMap<llvm::Value *>> AllocaMap;
  llvm::Function *CurScope;

  llvm::Type *VoidTy;
  llvm::Type *Int32Ty;

  llvm::Function *getFunc(llvm::StringRef Name) {
    return FuncMap[Name];
  }
  void setFunc(llvm::StringRef Name, llvm::Function *Func) {
    FuncMap[Name] = Func;
  }
  llvm::Value *getAlloca(llvm::StringRef Name) {
    return AllocaMap[CurScope][Name];
  }
  void setAlloca(llvm::StringRef Name, llvm::Value *Alloca) {
    AllocaMap[CurScope][Name] = Alloca;
  }
  llvm::Type *getLLVMType(TypeDecl *Ty);

  void compileTranslationUnit(TranslationUnitDecl *TransUnit);
  void compileFunc(FuncDecl *Func);
  void compileFormalParm(ParmVarDecl *Parm, llvm::Argument *Arg);
  void compileVarDecl(VarDecl *Var);
  void compileStmt(Stmt *Stmt);
  void compileExpr(Expr *Expr);

public:
  CodeGen(llvm::LLVMContext &Ctx) : Ctx(Ctx), Builder(Ctx) {
    VoidTy = llvm::Type::getVoidTy(Ctx);
    Int32Ty = llvm::Type::getInt32Ty(Ctx);
  }

  std::unique_ptr<llvm::Module> compile(TranslationUnitDecl *TransUnit, std::string FileName);
};

#endif