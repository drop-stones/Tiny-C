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

  TypeDecl *IntegerType;

  void error() {
    llvm::errs() << "Unexpected: " << Tok.getText() << "\n";
    HasError = true;
  }

  void advance() { Tok = Lex.next(); }

  /// Return false if the token is expected.
  bool expect(tok::TokenKind ExpectedKind) {
    if (Tok.getKind() != ExpectedKind) {
      error();
      return true;
    }
    return false;
  }

  bool consume(tok::TokenKind ExpectedKind) {
    if (expect(ExpectedKind))
      return true;
    advance();
    return false;
  }

  void parseTranslationUnit(TranslationUnitDecl *&T);
  void parseFunc(FuncList &Funcs);
  void parseParmVar(ParmVarList &Parms);
  void parseStmt(StmtList &Stmts);
  void parseExpr(Expr *&E);

  //StmtList *parseStatementSequence();
  //Stmt *parseStatement();
  //Stmt *parseAssign();
  //Stmt *parseReturn();
  //Expr *parseExpr();
  //Expr *parseTerm();
  //Expr *parseFactor();

public:
  Parser(Lexer &Lex) : Lex(Lex), HasError(false) {
    advance();

    IntegerType = new TypeDecl(SMLoc(), "int");
  }

  bool hasError() { return HasError; }
  TranslationUnitDecl *parse();
};

#endif