
#include "tiny-c/Parser/Parser.h"
#include "gtest/gtest.h"

namespace {

TEST(ParserTest, parseTest) {
  Lexer Lex("1 + 2 * 3");
  Parser Parser(Lex);
  AST *Tree = Parser.parse();

  BinaryOp *Plus = (BinaryOp *)Tree;
  EXPECT_EQ(BinaryOp::Plus, Plus->getOperator());

  Factor *One = (Factor *)Plus->getLeft();
  EXPECT_EQ("1", One->getVal());

  BinaryOp *Mul = (BinaryOp *)Plus->getRight();
  EXPECT_EQ(BinaryOp::Mul, Mul->getOperator());

  Factor *Two = (Factor *)Mul->getLeft();
  EXPECT_EQ("2", Two->getVal());

  Factor *Three = (Factor *)Mul->getRight();
  EXPECT_EQ("3", Three->getVal());
}

} // namespace