
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

} // namespace