#ifndef TINY_C_AST_H
#define TINY_C_AST_H

#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"

class AST;
class Expr;
class Factor;
class BinaryOp;
class Stmt;
class AssignStmt;
class RetStmt;

using StmtList = std::vector<Stmt *>;

class ASTVisitor {
public:
  virtual void visit(AST &){};
  virtual void visit(Expr &){};
  virtual void visit(Factor &){};
  virtual void visit(BinaryOp &){};
  virtual void visit(AssignStmt &){};
  virtual void visit(RetStmt &){};
};

class Procedure {
  StmtList Stmts;

public:
  Procedure() {}
  StmtList &getStmts() { return Stmts; }
  void setStmts(StmtList &L) { Stmts = L; }
};

class AST {
public:
  virtual ~AST() {}
  virtual void accept(ASTVisitor &V) = 0;
};

class Expr : public AST {
public:
  Expr() {}
};

// hold number or identifier.
class Factor : public Expr {
public:
  enum ValueKind { Ident, Number };

private:
  ValueKind Kind;
  llvm::StringRef Val;

public:
  Factor(ValueKind K, llvm::StringRef Val) : Kind(K), Val(Val) {}
  ValueKind getKind() const { return Kind; }
  llvm::StringRef getVal() const { return Val; }
  virtual void accept(ASTVisitor &V) override {
    V.visit(*this);
  }
};

class BinaryOp : public Expr {
public:
  enum Operator { Plus, Minus, Mul, Div };

private:
  Expr *Left;
  Expr *Right;
  Operator Op;

public:
  BinaryOp(Operator Op, Expr *L, Expr *R) : Op(Op), Left(L), Right(R) {}
  Expr *getLeft() { return Left; }
  Expr *getRight() { return Right; }
  Operator getOperator() { return Op; }
  virtual void accept(ASTVisitor &V) override {
    V.visit(*this);
  }
};

class Stmt : public AST {
public:
  enum StmtKind {
    SK_Assign,
    SK_Return
  };

private:
  const StmtKind Kind;

protected:
  Stmt(StmtKind K) : Kind(K) {}

public:
  StmtKind getKind() const { return Kind; }
};

class AssignStmt : public Stmt {
  Factor *Var;
  Expr *E;

public:
  AssignStmt(Factor *V, Expr *E) : Stmt(SK_Assign), Var(V), E(E) {}
  Factor *getVar() { return Var; }
  Expr *getExpr() { return E; }
  virtual void accept(ASTVisitor &V) override {
    V.visit(*this);
  }
};

class RetStmt : public Stmt {
  Expr *E;

public:
  RetStmt(Expr *E) : Stmt(SK_Return), E(E) {}
  Expr *getExpr() { return E; }
  virtual void accept(ASTVisitor &V) override {
    V.visit(*this);
  }
};

#endif