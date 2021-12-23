
#include "tiny-c/Lexer/Lexer.h"
#include "gtest/gtest.h"

namespace {

TEST(LexerTest, tokenizeTest) {
  Lexer Lex("1 + 2 * 3");
  Token tok;

  Lex.next(tok);
  EXPECT_EQ("1", tok.getText());
  EXPECT_EQ(Token::number, tok.getKind());

  Lex.next(tok);
  EXPECT_EQ("+", tok.getText());
  EXPECT_EQ(Token::plus, tok.getKind());

  Lex.next(tok);
  EXPECT_EQ("2", tok.getText());
  EXPECT_EQ(Token::number, tok.getKind());

  Lex.next(tok);
  EXPECT_EQ("*", tok.getText());
  EXPECT_EQ(Token::star, tok.getKind());

  Lex.next(tok);
  EXPECT_EQ("3", tok.getText());
  EXPECT_EQ(Token::number, tok.getKind());

  Lex.next(tok);
  EXPECT_EQ(Token::eoi, tok.getKind());
}

} // namespace