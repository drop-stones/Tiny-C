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
  DeclList Decls;

public:
  FuncDecl(SMLoc Loc, StringRef Name, TypeDecl *RetTy, ParmVarList &Parms, StmtList &Stmts, DeclList &Decls)
    : Decl(DK_Func, Loc, Name), RetTy(RetTy), Parms(Parms), Stmts(Stmts), Decls(Decls) {}

  TypeDecl *getRetTy() { return RetTy; }
  void setRetTy(TypeDecl *T) { RetTy = T; }
  ParmVarList &getParms() { return Parms; }
  void setParms(ParmVarList &P) { Parms = P; }
  StmtList &getStmts() { return Stmts; }
  void setStmts(StmtList &S) { Stmts = S; }
  DeclList &getDecls() { return Decls; }
  void setDecls(DeclList &D) { Decls = D; }
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
    SK_If,
    SK_While,
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

  static bool classof(const Stmt *S) {
    return S->getKind() == SK_Decl;
  }
};

class AssignStmt : public Stmt {
  VarDecl *Var;
  Expr *E;

public:
  AssignStmt(VarDecl *Var, Expr *E) : Stmt(SK_Assign), Var(Var), E(E) {}

  VarDecl *getVar() { return Var; }
  Expr *getExpr() { return E; }

  static bool classof(const Stmt *S) {
    return S->getKind() == SK_Assign;
  }
};

class ReturnStmt : public Stmt {
  Expr *RetVal;

public:
  ReturnStmt(Expr *RetVal) : Stmt(SK_Return), RetVal(RetVal) {}

  Expr *getRetVal() { return RetVal; }

  static bool classof(const Stmt *S) {
    return S->getKind() == SK_Return;
  }
};

class IfStmt : public Stmt {
  Expr *Cond;
  StmtList ThenBlock;
  StmtList ElseBlock;

public:
  IfStmt(Expr *Cond, StmtList ThenBlock, StmtList ElseBlock)
    : Stmt(SK_If), Cond(Cond), ThenBlock(ThenBlock), ElseBlock(ElseBlock) {}
  IfStmt(Expr *Cond, StmtList ThenBlock)
    : IfStmt(Cond, ThenBlock, {}) {}
  
  Expr *getCond() { return Cond; }
  StmtList &getThenBlock() { return ThenBlock; }
  StmtList &getElseBlock() { return ElseBlock; }

  static bool classof(const Stmt *S) {
    return S->getKind() == SK_If;
  }
};

class WhileStmt : public Stmt {
  Expr *Cond;
  StmtList Block;

public:
  WhileStmt(Expr *Cond, StmtList Block) : Stmt(SK_While), Cond(Cond), Block(Block) {}

  Expr *getCond() { return Cond; }
  StmtList &getBlock() { return Block; }

  static bool classof(const Stmt *S) {
    return S->getKind() == SK_While;
  }
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
    //EK_Equality,
    //EK_Relational,
    //EK_Additive,
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

  static bool classof(const Expr *E) {
    return E->getKind() == EK_BinaryOp;
  }
};

class IntegerLiteral : public Expr {
  SMLoc Loc;
  llvm::APSInt Val;

public:
  IntegerLiteral(SMLoc Loc, const llvm::APSInt &Val, TypeDecl *Ty)
    : Expr(EK_Int, Ty), Loc(Loc), Val(Val) {}
  
  llvm::APSInt &getVal() { return Val; }

  static bool classof(const Expr *E) {
    return E->getKind() == EK_Int;
  }
};

class DeclRef : public Expr {
  Decl *Var;

public:
  DeclRef(VarDecl *Var) : Expr(EK_DeclRef, Var->getTy()), Var(Var) {}

  Decl *getDecl() { return Var; }

  static bool classof(const Expr *E) {
    return E->getKind() == EK_DeclRef;
  }
};

#endif