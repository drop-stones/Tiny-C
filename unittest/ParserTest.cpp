
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

} // namespace