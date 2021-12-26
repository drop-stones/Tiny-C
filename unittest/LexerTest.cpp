
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

TEST(LexerTest, localVariableTest) {
  Lexer Lex("   \
    num = 100;  \
    return num; \
  ");
  Token tok;

  Lex.next(tok);
  EXPECT_EQ("num", tok.getText());
  EXPECT_EQ(Token::ident, tok.getKind());

  Lex.next(tok);
  EXPECT_EQ("=", tok.getText());
  EXPECT_EQ(Token::equal, tok.getKind());

  Lex.next(tok);
  EXPECT_EQ("100", tok.getText());
  EXPECT_EQ(Token::number, tok.getKind());

  Lex.next(tok);
  EXPECT_EQ(";", tok.getText());
  EXPECT_EQ(Token::semicolon, tok.getKind());

  Lex.next(tok);
  EXPECT_EQ("return", tok.getText());
  EXPECT_EQ(Token::KW_return, tok.getKind());

  Lex.next(tok);
  EXPECT_EQ("num", tok.getText());
  EXPECT_EQ(Token::ident, tok.getKind());

  Lex.next(tok);
  EXPECT_EQ(";", tok.getText());
  EXPECT_EQ(Token::semicolon, tok.getKind());

  Lex.next(tok);
  EXPECT_EQ(Token::eoi, tok.getKind());
}

} // namespace