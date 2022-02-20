#ifndef TINY_C_PARSER_H
#define TINY_C_PARSER_H

#include "tiny-c/AST/AST.h"
#include "tiny-c/Lexer/Lexer.h"
#include "llvm/Support/raw_ostream.h"
#include <unordered_map>

class Parser {
private:
  Lexer &Lex;
  Token Tok;
  bool HasError;

  TypeDecl *IntegerType;

  llvm::StringMap<FuncDecl *> FuncMap;
  std::unordered_map<Decl *, llvm::StringMap<VarDecl *>> DeclMap;
  Decl *CurScope;

  void setFuncDecl(FuncDecl *F) {
    FuncMap[F->getName()] = F;
  }

  FuncDecl *getFuncDecl(llvm::StringRef Name) {
    return FuncMap[Name];
  }

  void setVarDecl(VarDecl *D) {
    DeclMap[CurScope][D->getName()] = D;
  }

  VarDecl *getVarDecl(llvm::StringRef Name) {
    return DeclMap[CurScope][Name];
  }

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
  void parseBlock(StmtList &Stmts, DeclList &Decls);
  void parseParmVar(DeclList &Parms);
  void parseStmt(StmtList &Stmts, DeclList &Decls);
  void parseExpr(Expr *&E);
  void parseRelationalExpr(Expr *&E);
  void parseAdditiveExpr(Expr *&E);
  void parseTerm(Expr *&E);
  void parseFactor(Expr *&E);

public:
  Parser(Lexer &Lex) : Lex(Lex), HasError(false) {
    advance();

    IntegerType = new TypeDecl(SMLoc(), "int");
  }

  bool hasError() { return HasError; }
  TranslationUnitDecl *parse();
};

#endif