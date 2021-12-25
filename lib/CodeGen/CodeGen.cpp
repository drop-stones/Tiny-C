#include "tiny-c/CodeGen/CodeGen.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
class ToIRVisitor : public ASTVisitor {
  Module *M;
  IRBuilder<> Builder;
  Type *VoidTy;
  Type *Int32Ty;
  Value *V;

public:
  ToIRVisitor(Module *M) : M(M), Builder(M->getContext()) {
    VoidTy = Type::getVoidTy(M->getContext());
    Int32Ty = Type::getInt32Ty(M->getContext());
  }

  void run(AST *Tree) {
    FunctionType *MainFty = FunctionType::get(Int32Ty, {VoidTy}, false);
    Function *MainFn = Function::Create(MainFty, GlobalValue::ExternalLinkage, "main", M);
    BasicBlock *BB = BasicBlock::Create(M->getContext(), "entry", MainFn);

    Builder.SetInsertPoint(BB);

    Tree->accept(*this);

    Builder.CreateRet(V);
  }

  virtual void visit(Factor &Node) override {
    int intval;
    Node.getVal().getAsInteger(10, intval);
    V = ConstantInt::get(Int32Ty, intval, true);
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
};

} // namespace

std::unique_ptr<llvm::Module> CodeGen::compile(AST *Tree) {
  std::unique_ptr<llvm::Module> M = std::make_unique<llvm::Module>("tiny-c.expr", Ctx);
  ToIRVisitor ToIR(M.get());
  ToIR.run(Tree);
  return M;
}