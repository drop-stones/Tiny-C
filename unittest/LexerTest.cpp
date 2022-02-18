
#include "tiny-c/Lexer/Lexer.h"
#include "gtest/gtest.h"

namespace {

TEST(LexerTest, tokenizeTest) {
  Lexer Lex("1 + 2 * 3");
  Lex.run();
  Token tok;

  tok = Lex.next();
  EXPECT_EQ(tok::integer_literal, tok.getKind());
  EXPECT_EQ("1", tok.getLiteralData());

  tok = Lex.next();
  EXPECT_EQ(tok::plus, tok.getKind());

  tok = Lex.next();
  EXPECT_EQ(tok::integer_literal, tok.getKind());
  EXPECT_EQ("2", tok.getLiteralData());

  tok = Lex.next();
  EXPECT_EQ(tok::star, tok.getKind());

  tok = Lex.next();
  EXPECT_EQ(tok::integer_literal, tok.getKind());
  EXPECT_EQ("3", tok.getLiteralData());

  tok = Lex.next();
  EXPECT_EQ(tok::eof, tok.getKind());
}

TEST(LexerTest, localVariableTest) {
  Lexer Lex("     \
    int main() {  \
      int num;    \
      num = 100;  \
      return num; \
    }             \
  ");
  Lex.run();
  Token tok;

  tok = Lex.next();
  EXPECT_EQ("int", tok.getText());
  EXPECT_EQ(tok::kw_INTEGER, tok.getKind());

  tok = Lex.next();
  EXPECT_EQ("main", tok.getText());
  EXPECT_EQ(tok::ident, tok.getKind());

  tok = Lex.next();
  EXPECT_EQ("(", tok.getText());
  EXPECT_EQ(tok::l_paren, tok.getKind());

  tok = Lex.next();
  EXPECT_EQ(")", tok.getText());
  EXPECT_EQ(tok::r_paren, tok.getKind());

  tok = Lex.next();
  EXPECT_EQ("{", tok.getText());
  EXPECT_EQ(tok::l_curl, tok.getKind());

  tok = Lex.next();
  EXPECT_EQ("int", tok.getText());
  EXPECT_EQ(tok::kw_INTEGER, tok.getKind());

  tok = Lex.next();
  EXPECT_EQ("num", tok.getText());
  EXPECT_EQ(tok::ident, tok.getKind());

  tok = Lex.next();
  EXPECT_EQ(";", tok.getText());
  EXPECT_EQ(tok::semi, tok.getKind());

  tok = Lex.next();
  EXPECT_EQ("num", tok.getText());
  EXPECT_EQ(tok::ident, tok.getKind());

  tok = Lex.next();
  EXPECT_EQ("=", tok.getText());
  EXPECT_EQ(tok::equal, tok.getKind());

  tok = Lex.next();
  EXPECT_EQ("100", tok.getText());
  EXPECT_EQ(tok::integer_literal, tok.getKind());

  tok = Lex.next();
  EXPECT_EQ(";", tok.getText());
  EXPECT_EQ(tok::semi, tok.getKind());

  tok = Lex.next();
  EXPECT_EQ("return", tok.getText());
  EXPECT_EQ(tok::kw_RETURN, tok.getKind());

  tok = Lex.next();
  EXPECT_EQ("num", tok.getText());
  EXPECT_EQ(tok::ident, tok.getKind());

  tok = Lex.next();
  EXPECT_EQ(";", tok.getText());
  EXPECT_EQ(tok::semi, tok.getKind());

  tok = Lex.next();
  EXPECT_EQ("}", tok.getText());
  EXPECT_EQ(tok::r_curl, tok.getKind());

  tok = Lex.next();
  EXPECT_EQ(tok::eof, tok.getKind());
}

TEST(LexerTest, ifTest) {
  Lexer Lex("         \
    if (100 < 200) {  \
      return 100;     \
    } else {          \
      return 200;     \
    } \
  ");
  Lex.run();
  Token tok;

  tok = Lex.next();
  EXPECT_EQ("if", tok.getText());
  EXPECT_EQ(tok::kw_IF, tok.getKind());

  tok = Lex.next();
  EXPECT_EQ("(", tok.getText());
  EXPECT_EQ(tok::l_paren, tok.getKind());

  tok = Lex.next(); // 100

  tok = Lex.next();
  EXPECT_EQ("<", tok.getText());
  EXPECT_EQ(tok::less, tok.getKind());

  tok = Lex.next(); // 200
  tok = Lex.next(); // )
  tok = Lex.next(); // {
  tok = Lex.next(); // return
  tok = Lex.next(); // 100
  tok = Lex.next(); // ;
  tok = Lex.next(); // }

  tok = Lex.next(); // else
  EXPECT_EQ("else", tok.getText());
  EXPECT_EQ(tok::kw_ELSE, tok.getKind());
}

TEST(LexerTest, whileTest) {
  Lexer Lex("           \
    while (num != 0) {  \
      num = num - 1;    \
    }                   \
  ");
  Lex.run();
  Token tok;

  tok = Lex.next();
  EXPECT_EQ("while", tok.getText());
  EXPECT_EQ(tok::kw_WHILE, tok.getKind());

  tok = Lex.next(); // (
  tok = Lex.next(); // num

  tok = Lex.next();
  EXPECT_EQ("!=", tok.getText());
  EXPECT_EQ(tok::notequal, tok.getKind());
}

TEST(LexerTest, funcCallTest) {
  Lexer Lex("             \
    int sum;              \
    sum = add(100, 200);  \
    return sum;           \
  ");
  Lex.run();
  Token tok;

  tok = Lex.next();
  EXPECT_EQ("int", tok.getText());
  EXPECT_EQ(tok::kw_INTEGER, tok.getKind());

  tok = Lex.next(); // sum
  tok = Lex.next(); // ;
  tok = Lex.next(); // sum
  tok = Lex.next(); // =

  tok = Lex.next(); // add
  EXPECT_EQ("add", tok.getText());
  EXPECT_EQ(tok::ident, tok.getKind());

  tok = Lex.next(); // (
  EXPECT_EQ("(", tok.getText());
  EXPECT_EQ(tok::l_paren, tok.getKind());

  tok = Lex.next(); // 100
  EXPECT_EQ("100", tok.getText());
  EXPECT_EQ(tok::integer_literal, tok.getKind());

  tok = Lex.next(); // ,
  EXPECT_EQ(",", tok.getText());
  EXPECT_EQ(tok::comma, tok.getKind());

  tok = Lex.next(); // 200
  EXPECT_EQ("200", tok.getText());
  EXPECT_EQ(tok::integer_literal, tok.getKind());

  tok = Lex.next(); // )
  EXPECT_EQ(")", tok.getText());
  EXPECT_EQ(tok::r_paren, tok.getKind());

  tok = Lex.next(); // ;
}

} // namespace