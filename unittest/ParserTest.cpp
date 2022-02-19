
#include "tiny-c/Parser/Parser.h"
#include "gtest/gtest.h"

namespace {

TEST(ParserTest, funcTest) {
  Lexer Lex("     \
    int main() {  \
      return 100; \
    }             \
  ");
  Lex.run();
  Parser Parser(Lex);
  TranslationUnitDecl *TransUnit = Parser.parse();

  EXPECT_EQ(Decl::DK_TranslationUnit, TransUnit->getKind());

  FuncDecl *Func = TransUnit->getFuncs().front();
  EXPECT_EQ(Decl::DK_Func, Func->getKind());
  EXPECT_EQ("int", Func->getRetTy()->getName());
  EXPECT_EQ(0, Func->getParms().size());
  EXPECT_EQ(1, Func->getStmts().size());

  ReturnStmt *Ret = (ReturnStmt *)Func->getStmts().front();
  EXPECT_EQ(Stmt::SK_Return, Ret->getKind());

  IntegerLiteral *RetVal = (IntegerLiteral *)Ret->getRetVal();
  EXPECT_EQ(Expr::EK_Int, RetVal->getKind());
  EXPECT_EQ(100, RetVal->getVal());
}

TEST(ParserTest, exprTest) {
  Lexer Lex("             \
    int main() {          \
      return (1 + 2) * 3; \
    }                     \
  ");
  Lex.run();
  Parser Parser(Lex);
  TranslationUnitDecl *TransUnit = Parser.parse();

  EXPECT_EQ(Decl::DK_TranslationUnit, TransUnit->getKind());

  FuncDecl *Func = TransUnit->getFuncs().front();
  EXPECT_EQ(Decl::DK_Func, Func->getKind());
  EXPECT_EQ("int", Func->getRetTy()->getName());
  EXPECT_EQ(0, Func->getParms().size());
  EXPECT_EQ(1, Func->getStmts().size());

  ReturnStmt *Ret = (ReturnStmt *)Func->getStmts().front();
  EXPECT_EQ(Stmt::SK_Return, Ret->getKind());

  BinaryOp *RetVal = (BinaryOp *)Ret->getRetVal();
  EXPECT_EQ(Expr::EK_BinaryOp, RetVal->getKind());
  EXPECT_EQ(tok::star, RetVal->getOperator().getKind());

  BinaryOp *Left = (BinaryOp *)RetVal->getLeft();
  EXPECT_EQ(Expr::EK_BinaryOp, Left->getKind());
  EXPECT_EQ(tok::plus, Left->getOperator().getKind());

  IntegerLiteral *One = (IntegerLiteral *)Left->getLeft();
  EXPECT_EQ(Expr::EK_Int, One->getKind());
  EXPECT_EQ(1, One->getVal());

  IntegerLiteral *Two = (IntegerLiteral *)Left->getRight();
  EXPECT_EQ(Expr::EK_Int, Two->getKind());
  EXPECT_EQ(2, Two->getVal());

  IntegerLiteral *Three = (IntegerLiteral *)RetVal->getRight();
  EXPECT_EQ(Expr::EK_Int, Three->getKind());
  EXPECT_EQ(3, Three->getVal());
}

TEST(ParserTest, varTest) {
  Lexer Lex("     \
    int main() {  \
      int num;    \
      num = 100;  \
      return num; \
    }             \
  ");
  Lex.run();
  Parser Parser(Lex);
  TranslationUnitDecl *TransUnit = Parser.parse();

  EXPECT_EQ(Decl::DK_TranslationUnit, TransUnit->getKind());

  FuncDecl *Func = TransUnit->getFuncs().front();
  EXPECT_EQ(Decl::DK_Func, Func->getKind());
  EXPECT_EQ("int", Func->getRetTy()->getName());
  EXPECT_EQ(0, Func->getParms().size());
  EXPECT_EQ(3, Func->getStmts().size());

  DeclStmt *Decl = (DeclStmt *)Func->getStmts().at(0);
  EXPECT_EQ(Stmt::SK_Decl, Decl->getKind());
  VarDecl *num = (VarDecl *)Decl->getVar();
  EXPECT_EQ("int", num->getTy()->getName());
  EXPECT_EQ("num", num->getName());

  AssignStmt *Assign = (AssignStmt *)Func->getStmts().at(1);
  EXPECT_EQ(Stmt::SK_Assign, Assign->getKind());
  VarDecl *num2 = (VarDecl *)Assign->getVar();
  EXPECT_EQ("int", num2->getTy()->getName());
  EXPECT_EQ("num", num2->getName());
  IntegerLiteral *E = (IntegerLiteral *)Assign->getExpr();
  EXPECT_EQ(Expr::EK_Int, E->getKind());
  EXPECT_EQ(100, E->getVal());

  ReturnStmt *Ret = (ReturnStmt *)Func->getStmts().at(2);
  EXPECT_EQ(Stmt::SK_Return, Ret->getKind());
  DeclRef *RetVal = (DeclRef *)Ret->getRetVal();
  EXPECT_EQ(Expr::EK_DeclRef, RetVal->getKind());
  VarDecl *num3 = (VarDecl *)RetVal->getDecl();
  EXPECT_EQ("int", num3->getTy()->getName());
  EXPECT_EQ("num", num3->getName());
}

TEST(ParserTest, ifTest) {
  Lexer Lex("     \
    int main() {  \
      if (1 < 2) {    \
        return 1; \
      } else {    \
        return 0; \
      }           \
    }             \
  ");
  Lex.run();
  Parser Parser(Lex);
  TranslationUnitDecl *TransUnit = Parser.parse();
  FuncDecl *Func = TransUnit->getFuncs().front();

  IfStmt *If = (IfStmt *)Func->getStmts().front();
  EXPECT_EQ(Stmt::SK_If, If->getKind());

  BinaryOp *Cond = (BinaryOp *)If->getCond();
  EXPECT_EQ(Expr::EK_BinaryOp, Cond->getKind());
  EXPECT_EQ(tok::less, Cond->getOperator().getKind());
  IntegerLiteral *One = (IntegerLiteral *)Cond->getLeft();
  EXPECT_EQ(1, One->getVal());
  IntegerLiteral *Two = (IntegerLiteral *)Cond->getRight();
  EXPECT_EQ(2, Two->getVal());

  StmtList ThenBlock = If->getThenBlock();
  ReturnStmt *Ret = (ReturnStmt *)ThenBlock.front();
  EXPECT_EQ(Stmt::SK_Return, Ret->getKind());
  IntegerLiteral *RetVal = (IntegerLiteral *)Ret->getRetVal();
  EXPECT_EQ(1, RetVal->getVal());

  StmtList ElseBlock = If->getElseBlock();
  Ret = (ReturnStmt *)ElseBlock.front();
  RetVal = (IntegerLiteral *)Ret->getRetVal();
  EXPECT_EQ(0, RetVal->getVal());
}

TEST(ParserTest, whileTest) {
  Lexer Lex("         \
    int main() {      \
      int num;        \
      num = 0;        \
      while (1 <= 2) {\
        num = num + 1;\
      }               \
    }                 \
  ");
  Lex.run();
  Parser Parser(Lex);
  TranslationUnitDecl *TransUnit = Parser.parse();
  FuncDecl *Func = TransUnit->getFuncs().front();

  WhileStmt *While = (WhileStmt *)Func->getStmts().at(2);
  EXPECT_EQ(Stmt::SK_While, While->getKind());

  BinaryOp *Cond = (BinaryOp *)(While->getCond());
  IntegerLiteral *One = (IntegerLiteral *)Cond->getLeft();
  EXPECT_EQ(1, One->getVal());
  IntegerLiteral *Two = (IntegerLiteral *)Cond->getRight();
  EXPECT_EQ(2, Two->getVal());

  AssignStmt *Assign = (AssignStmt *)While->getBlock().front();
  EXPECT_EQ(Stmt::SK_Assign, Assign->getKind());
  VarDecl *num = Assign->getVar();
  EXPECT_EQ("num", num->getName());
  BinaryOp *Add = (BinaryOp *)Assign->getExpr();
  EXPECT_EQ(tok::plus, Add->getOperator().getKind());
  DeclRef *Num = (DeclRef *)Add->getLeft();
  EXPECT_EQ("num", Num->getDecl()->getName());
  One = (IntegerLiteral *)Add->getRight();
  EXPECT_EQ(1, One->getVal());
}

TEST(ParserTest, FormalArgumentTest) {
  Lexer Lex("               \
    int add(int a, int b) { \
      return a + b;         \
    }                       \
  ");
  Lex.run();
  Parser Parser(Lex);
  TranslationUnitDecl *TransUnit = Parser.parse();
  FuncDecl *Func = TransUnit->getFuncs().front();

  TypeDecl *RetTy = Func->getRetTy();
  EXPECT_EQ("int", RetTy->getName());

  EXPECT_EQ("add", Func->getName());

  ParmVarDecl *a = Func->getParms()[0];
  EXPECT_EQ("int", a->getTy()->getName());
  EXPECT_EQ("a", a->getName());

  ParmVarDecl *b = Func->getParms()[1];
  EXPECT_EQ("int", b->getTy()->getName());
  EXPECT_EQ("b", b->getName());

  ReturnStmt *ret = (ReturnStmt *)Func->getStmts()[0];
  EXPECT_EQ(Stmt::SK_Return, ret->getKind());

  Expr *add = ret->getRetVal();
  EXPECT_EQ(Expr::EK_BinaryOp, add->getKind());
}

TEST(ParserTest, FuncCallTest) {
  Lexer Lex("               \
    int add(int a, int b) { \
      return a + b;         \
    }                       \
    int main() {            \
      return add(100, 200); \
    }                       \
  ");
  Lex.run();
  Parser Parser(Lex);
  TranslationUnitDecl *TransUnit = Parser.parse();
  FuncDecl *add = TransUnit->getFuncs()[0];

  ReturnStmt *ret = (ReturnStmt *)add->getStmts()[0];
  EXPECT_EQ(Stmt::SK_Return, ret->getKind());

  FuncDecl *main = TransUnit->getFuncs()[1];

  ret = (ReturnStmt *)main->getStmts()[0];
  EXPECT_EQ(Stmt::SK_Return, ret->getKind());

  FuncCall *call = (FuncCall *)ret->getRetVal();
  EXPECT_EQ(Expr::EK_FuncCall, call->getKind());
  FuncDecl *callee = call->getFuncDecl();
  EXPECT_EQ("add", callee->getName());
  EXPECT_EQ("int", callee->getRetTy()->getName());
  ExprList Actuals = call->getActuals();
  EXPECT_EQ(100, ((IntegerLiteral *)Actuals[0])->getVal());
  EXPECT_EQ(200, ((IntegerLiteral *)Actuals[1])->getVal());
}

} // namespace