
#include "tiny-c/Parser/Parser.h"
#include "gtest/gtest.h"

namespace {

TEST(ParserTest, returnTest) {
  Lexer Lex("return 1 + 2 * 3;");
  Parser Parser(Lex);
  StmtList *Stmts = Parser.parse();

  Stmt *stmt = Stmts->front();
  RetStmt *Return = (RetStmt *)stmt;

  BinaryOp *Plus = (BinaryOp *)Return->getExpr();
  EXPECT_EQ(BinaryOp::Plus, Plus->getOperator());

  Factor *One = (Factor *)Plus->getLeft();
  EXPECT_EQ(Factor::Number, One->getKind());
  EXPECT_EQ("1", One->getVal());

  BinaryOp *Mul = (BinaryOp *)Plus->getRight();
  EXPECT_EQ(BinaryOp::Mul, Mul->getOperator());

  Factor *Two = (Factor *)Mul->getLeft();
  EXPECT_EQ(Factor::Number, Two->getKind());
  EXPECT_EQ("2", Two->getVal());

  Factor *Three = (Factor *)Mul->getRight();
  EXPECT_EQ(Factor::Number, Three->getKind());
  EXPECT_EQ("3", Three->getVal());
}

TEST(ParserTest, localVariableTest) {
  Lexer Lex("  \
    num = 100; \
    return num + 200; \
  ");
  Parser Parser(Lex);
  StmtList *Stmts = Parser.parse();

  AssignStmt *Assign = (AssignStmt *)Stmts->front();
  EXPECT_EQ(Stmt::SK_Assign, Assign->getKind());

  Factor *num = Assign->getVar();
  EXPECT_EQ(Factor::Ident, num->getKind());
  EXPECT_EQ("num", num->getVal());

  Factor *Const100 = (Factor *)Assign->getExpr();
  EXPECT_EQ(Factor::Number, Const100->getKind());
  EXPECT_EQ("100", Const100->getVal());

  RetStmt *Return = (RetStmt *)Stmts->at(1);
  EXPECT_EQ(Stmt::SK_Return, Return->getKind());

  BinaryOp *Plus = (BinaryOp *)Return->getExpr();
  EXPECT_EQ(BinaryOp::Plus, Plus->getOperator());

  num = (Factor *)Plus->getLeft();
  EXPECT_EQ(Factor::Ident, num->getKind());
  EXPECT_EQ("num", num->getVal());

  Factor *Const200 = (Factor *)Plus->getRight();
  EXPECT_EQ(Factor::Number, Const200->getKind());
  EXPECT_EQ("200", Const200->getVal());
}

} // namespace