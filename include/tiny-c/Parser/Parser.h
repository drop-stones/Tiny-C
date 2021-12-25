#ifndef TINY_C_PARSER_H
#define TINY_C_PARSER_H

#include "tiny-c/AST/AST.h"
#include "tiny-c/Lexer/Lexer.h"
#include "llvm/Support/raw_ostream.h"

class Parser {
private:
  Lexer &Lex;
  Token Tok;
  bool HasError;

  void error() {
    llvm::errs() << "Unexpected: " << Tok.getText() << "\n";
    HasError = true;
  }

  void advance() { Lex.next(Tok); }

  bool expect(Token::TokenKind Kind) {
    if (Tok.getKind() != Kind) {
      error();
      return true;
    }
    return false;
  }

  bool consume(Token::TokenKind Kind) {
    if (expect(Kind))
      return true;
    advance();
    return false;
  }

  StmtList *parseStatementSequence();
  Stmt *parseStatement();
  Stmt *parseAssign();
  Stmt *parseReturn();
  Expr *parseExpr();
  Expr *parseTerm();
  Expr *parseFactor();

public:
  Parser(Lexer &Lex) : Lex(Lex), HasError(false) {
    advance();
  }

  bool hasError() { return HasError; }
  StmtList *parse();
};

#endif