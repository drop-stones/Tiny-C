#ifndef TINY_C_AST_AST_H
#define TINY_C_AST_AST_H

#include "tiny-c/Basic/LLVM.h"
#include "tiny-c/Basic/TokenKinds.h"
#include "llvm/ADT/APSInt.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Support/SMLoc.h"
#include <vector>

class Decl;
class FuncDecl;
class ParmVarDecl;
class Expr;
class Stmt;
class Ident;

using DeclList = std::vector<Decl *>;
using FuncList = std::vector<FuncDecl *>;
using ParmVarList = std::vector<ParmVarDecl *>;
using ExprList = std::vector<Expr *>;
using StmtList = std::vector<Stmt *>;
using IdentList = std::vector<Ident *>;

class Ident {
  SMLoc Loc;
  StringRef Name;

public:
  Ident(SMLoc Loc, const StringRef &Name) : Loc(Loc), Name(Name) {}
  SMLoc getLocation() { return Loc; }
  const StringRef &getName() { return Name; }
};

class Decl {
public:
  enum DeclKind {
    DK_TranslationUnit,
    DK_Type,
    DK_Func,
    DK_Var,
    DK_Parm,
  };

private:
  const DeclKind Kind;

protected:
  // Decl *EnclosingDecl; /// for scope
  SMLoc Loc;
  StringRef Name;

public:
  Decl(DeclKind Kind, SMLoc Loc, StringRef Name)
    : Kind(Kind), Loc(Loc), Name(Name) {}
  
  DeclKind getKind() const { return Kind; }
  SMLoc getLocation() { return Loc; }
  StringRef getName() { return Name; }
};

class TranslationUnitDecl : public Decl {
  FuncList Funcs;

public:
  TranslationUnitDecl(SMLoc Loc, StringRef Name, FuncList &Funcs)
    : Decl(DK_TranslationUnit, Loc, Name), Funcs(Funcs) {}
  
  const FuncList &getFuncs() { return Funcs; }
  void setFuncs(FuncList &F) { Funcs = F; }
};

class TypeDecl : public Decl {
public:
  TypeDecl(SMLoc Loc, StringRef Name) : Decl(DK_Type, Loc, Name) {}
};

class FuncDecl : public Decl {
  TypeDecl *RetTy;
  ParmVarList Parms;
  StmtList Stmts;

public:
  FuncDecl(SMLoc Loc, StringRef Name, TypeDecl *RetTy, ParmVarList &Parms, StmtList &Stmts)
    : Decl(DK_Func, Loc, Name), RetTy(RetTy), Parms(Parms), Stmts(Stmts) {}

  TypeDecl *getRetTy() { return RetTy; }
  void setRetTy(TypeDecl *T) { RetTy = T; }
  ParmVarList &getParms() { return Parms; }
  void setParms(ParmVarList &P) { Parms = P; }
  StmtList &getStmts() { return Stmts; }
  void setStmts(StmtList &S) { Stmts = S; }
};

class VarDecl : public Decl {
  TypeDecl *Ty;

public:
  VarDecl(SMLoc Loc, StringRef Name, TypeDecl *Ty)
    : Decl(DK_Var, Loc, Name), Ty(Ty) {}
  
  TypeDecl *getTy() { return Ty; }
};

class ParmVarDecl : public Decl {
  TypeDecl *Ty;

public:
  ParmVarDecl(SMLoc Loc, StringRef Name, TypeDecl *Ty)
    : Decl(DK_Parm, Loc, Name), Ty(Ty) {}
  
  TypeDecl *getTy() { return Ty; }
};


class Stmt {
public:
  enum StmtKind {
    SK_Decl,
    SK_Assign,
    SK_Return,
  };

private:
  const StmtKind Kind;

protected:
  Stmt(StmtKind Kind) : Kind(Kind) {}

public:
  StmtKind getKind() const { return Kind; }
};

class DeclStmt : public Stmt {
  VarDecl *Var;

public:
  DeclStmt(VarDecl *Var) : Stmt(SK_Decl), Var(Var) {}

  VarDecl *getVar() { return Var; }
};

class AssignStmt : public Stmt {
  VarDecl *Var;
  Expr *E;

public:
  AssignStmt(VarDecl *Var, Expr *E) : Stmt(SK_Assign), Var(Var), E(E) {}

  VarDecl *getVar() { return Var; }
  Expr *getExpr() { return E; }
};

class ReturnStmt : public Stmt {
  Expr *RetVal;

public:
  ReturnStmt(Expr *RetVal) : Stmt(SK_Return), RetVal(RetVal) {}

  Expr *getRetVal() { return RetVal; }
};

class Operator {
  SMLoc Loc;
  tok::TokenKind Kind;

public:
  Operator(SMLoc Loc, tok::TokenKind Kind) : Loc(Loc), Kind(Kind) {}

  SMLoc getLocation() const { return Loc; }
  tok::TokenKind getKind() const { return Kind; }
};

class Expr {
public:
  enum ExprKind {
    EK_BinaryOp,
    EK_Int,
    EK_DeclRef,
  };

private:
  const ExprKind Kind;
  TypeDecl *Ty;

protected:
  Expr(ExprKind Kind, TypeDecl *Ty) : Kind(Kind), Ty(Ty) {}

public:
  ExprKind getKind() const { return Kind; }
  TypeDecl *getType() { return Ty; }
  void setType(TypeDecl *T) { Ty = T; }
};

class BinaryOp : public Expr {
  Expr *Left;
  Expr *Right;
  const Operator Op;

public:
  BinaryOp(Expr *Left, Expr *Right, Operator Op, TypeDecl *Ty)
    : Expr(EK_BinaryOp, Ty), Left(Left), Right(Right), Op(Op) {}
  
  Expr *getLeft() { return Left; }
  Expr *getRight() { return Right; }
  const Operator &getOperator() { return Op; }
};

class IntegerLiteral : public Expr {
  SMLoc Loc;
  llvm::APSInt Val;

public:
  IntegerLiteral(SMLoc Loc, const llvm::APSInt &Val, TypeDecl *Ty)
    : Expr(EK_Int, Ty), Loc(Loc), Val(Val) {}
  
  llvm::APSInt &getVal() { return Val; }
};

class DeclRef : public Expr {
  Decl *Var;

public:
  DeclRef(VarDecl *Var) : Expr(EK_DeclRef, Var->getTy()), Var(Var) {}

  Decl *getDecl() { return Var; }
};

/*
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
*/

#endif