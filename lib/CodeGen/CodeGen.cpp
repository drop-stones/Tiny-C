#include "tiny-c/AST/AST.h"
#include "tiny-c/CodeGen/CodeGen.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

llvm::Type *CodeGen::getLLVMType(TypeDecl *Ty) {
  if (Ty->getName() == "int")
    return Int32Ty;
  else
    return VoidTy;
}

std::unique_ptr<llvm::Module> CodeGen::compile(TranslationUnitDecl *TransUnit, std::string FileName) {
  std::unique_ptr<llvm::Module> M = std::make_unique<llvm::Module>(FileName, Ctx);
  Mod = M.get();
  compileTranslationUnit(TransUnit);
  return M;
};

void CodeGen::compileTranslationUnit(TranslationUnitDecl *TransUnit) {
  for (auto *Func : TransUnit->getFuncs()) {
    compileFunc(Func);
  }
}

void CodeGen::compileFunc(FuncDecl *Func) {
  llvm::Type *RetTy = getLLVMType(Func->getRetTy());
  FunctionType *FnTy = FunctionType::get(RetTy, {}, false);
  Function *Fn = Function::Create(FnTy, GlobalValue::ExternalLinkage, Func->getName(), Mod);
  CurScope = Fn;
  BasicBlock *BB = BasicBlock::Create(Ctx, "entry", Fn);

  Builder.SetInsertPoint(BB);

  for (auto *Var : Func->getDecls()) {
    compileVarDecl((VarDecl *)Var);
  }

  for (auto *Stmt : Func->getStmts()) {
    compileStmt(Stmt);
  }
}

void CodeGen::compileVarDecl(VarDecl *Var) {
  llvm::Value *Alloca = Builder.CreateAlloca(getLLVMType(Var->getTy()));
  setAlloca(Var->getName(), Alloca);
}

void CodeGen::compileStmt(Stmt *Stmt) {
  if (DeclStmt *Decl = dyn_cast<DeclStmt>(Stmt)) {
    // do nothing, already allocated.
    return;
  } else if (AssignStmt *Assign = dyn_cast<AssignStmt>(Stmt)) {
    Value *Alloca = getAlloca(Assign->getVar()->getName());
    compileExpr(Assign->getExpr());
    Value *Expr = V;
    Builder.CreateStore(Expr, Alloca);
  } else if (ReturnStmt *Return = dyn_cast<ReturnStmt>(Stmt)) {
    compileExpr(Return->getRetVal());
    Value *RetVal = V;
    Builder.CreateRet(RetVal);
  }
}

void CodeGen::compileExpr(Expr *Expr) {
  if (BinaryOp *Bin = dyn_cast<BinaryOp>(Expr)) {
    compileExpr(Bin->getLeft());
    Value *Left = V;
    compileExpr(Bin->getRight());
    Value *Right = V;
    switch (Bin->getOperator().getKind()) {
    case tok::plus:
      V = Builder.CreateNSWAdd(Left, Right); break;
    case tok::minus:
      V = Builder.CreateNSWSub(Left, Right); break;
    case tok::star:
      V = Builder.CreateNSWMul(Left, Right); break;
    case tok::slash:
      V = Builder.CreateSDiv(Left, Right); break;
    }
  } else if (IntegerLiteral *Int = dyn_cast<IntegerLiteral>(Expr)) {
    V = ConstantInt::get(Int32Ty, Int->getVal().getExtValue(), true);
  } else if (DeclRef *Decl = dyn_cast<DeclRef>(Expr)) {
    Value *Alloca = getAlloca(Decl->getDecl()->getName());
    Type *Ty = getLLVMType(Decl->getType());
    V = Builder.CreateLoad(Ty, Alloca);
  }
}

/*
namespace {
class ToIRVisitor : public ASTVisitor {
  Module *M;
  IRBuilder<> Builder;
  Type *VoidTy;
  Type *Int32Ty;
  Value *V;
  llvm::DenseMap<llvm::StringRef, llvm::Value *> Allocs;
  bool isRValue;

public:
  ToIRVisitor(Module *M) : M(M), Builder(M->getContext()), isRValue(false) {
    VoidTy = Type::getVoidTy(M->getContext());
    Int32Ty = Type::getInt32Ty(M->getContext());
  }

  void run(StmtList *Stmts) {
    FunctionType *MainFty = FunctionType::get(Int32Ty, {}, false);
    Function *MainFn = Function::Create(MainFty, GlobalValue::ExternalLinkage, "main", M);
    BasicBlock *BB = BasicBlock::Create(M->getContext(), "entry", MainFn);

    Builder.SetInsertPoint(BB);

    //Tree->accept(*this);
    for (Stmt *stmt : *Stmts) {
      stmt->accept(*this);
    }
  }

  virtual void visit(Factor &Node) override {
    if (Node.getKind() == Factor::Ident) {
      if (Allocs[Node.getVal()] == nullptr) {
        llvm::Value *Val = Builder.CreateAlloca(Int32Ty);
        Allocs[Node.getVal()] = Val;
        V = Val;
      } else {
        V = Allocs[Node.getVal()];
      }
      if (isRValue) {
        V = Builder.CreateLoad(Int32Ty, V);
      }
    } else if (Node.getKind() == Factor::Number) {
      int intval;
      Node.getVal().getAsInteger(10, intval);
      V = ConstantInt::get(Int32Ty, intval, true);
    }
  }

  virtual void visit(BinaryOp &Node) override {
    Node.getLeft()->accept(*this);
    Value *Left = V;
    Node.getRight()->accept(*this);
    Value *Right = V;
    switch (Node.getOperator()) {
    case BinaryOp::Plus:
      V = Builder.CreateNSWAdd(Left, Right); break; // NSW: "No Signed Wrap", the result is undefined if signed overflow occurs.
    case BinaryOp::Minus:
      V = Builder.CreateNSWSub(Left, Right); break;
    case BinaryOp::Mul:
      V = Builder.CreateNSWMul(Left, Right); break;
    case BinaryOp::Div:
      V = Builder.CreateSDiv(Left, Right);   break;
    }
  }

  virtual void visit(RetStmt &Node) override {
    isRValue = true;
    Node.getExpr()->accept(*this);
    Value *Expr = V;
    isRValue = false;
    Builder.CreateRet(V);
  }

  virtual void visit(AssignStmt &Node) override {
    Node.getVar()->accept(*this);
    Value *Var = V;
    isRValue = true;
    Node.getExpr()->accept(*this);
    Value *Expr = V;
    isRValue = false;
    V = Builder.CreateStore(Expr, Var);
  }
};

} // namespace

std::unique_ptr<llvm::Module> CodeGen::compile(StmtList *Stmts) {
  std::unique_ptr<llvm::Module> M = std::make_unique<llvm::Module>("tiny-c.expr", Ctx);
  ToIRVisitor ToIR(M.get());
  ToIR.run(Stmts);
  return M;
}
*/