#include "tiny-c/Parser/Parser.h"

TranslationUnitDecl *Parser::parse() {
  TranslationUnitDecl *Res;
  parseTranslationUnit(Res);
  expect(tok::eof);
  return Res;
}

void Parser::parseTranslationUnit(TranslationUnitDecl *&T) {
  llvm::outs() << "parseTranslationUnit\n";
  FuncList Funcs;
  while (Tok.isNot(tok::eof)) {
    parseFunc(Funcs);
  }
  //T->setFuncs(Funcs);
  T = new TranslationUnitDecl(SMLoc(), "TransUnit", Funcs);
}

void Parser::parseFunc(FuncList &Funcs) {
  //TypeDecl *Ty = new TypeDecl(Tok.getLocation(), Tok.getText());
  SMLoc Loc = Tok.getLocation();
  consume(tok::kw_INTEGER);
  StringRef FuncName = Tok.getText();
  consume(tok::ident);
  consume(tok::l_paren);

  // TODO: ParmVarList
  ParmVarList Parms;

  consume(tok::r_paren);
  consume(tok::l_curl);

  StmtList Stmts;
  while (Tok.isNot(tok::r_curl)) {
    parseStmt(Stmts);
    consume(tok::semi);
  }
  consume(tok::r_curl);

  FuncDecl *Func = new FuncDecl(Loc, FuncName, IntegerType, Parms, Stmts);
  Funcs.push_back(Func);
}

void Parser::parseStmt(StmtList &Stmts) {
  if (Tok.is(tok::kw_INTEGER)) { // DeclStmt
    TypeDecl *Ty = new TypeDecl(Tok.getLocation(), Tok.getText());
    consume(tok::kw_INTEGER);
    VarDecl *Var = new VarDecl(Tok.getLocation(), Tok.getText(), Ty);
    consume(tok::ident);
    DeclStmt *Decl = new DeclStmt(Var);
    Stmts.push_back(Decl);
    return;
  } else if (Tok.is(tok::ident) && Lex.peek(1).is(tok::equal)) {  // AssignStmt
    // TODO
  } else if (Tok.is(tok::kw_RETURN)) {  // ReturnStmt
    Expr *E;
    SMLoc Loc = Tok.getLocation();
    consume(tok::kw_RETURN);
    parseExpr(E);
    ReturnStmt *Ret = new ReturnStmt(E);
    Stmts.push_back(Ret);
    return;
  }
}

void Parser::parseExpr(Expr *&E) {
  if (Tok.is(tok::integer_literal)) {
    SMLoc Loc = Tok.getLocation();
    llvm::APSInt Val{Tok.getText()};
    consume(tok::integer_literal);
    E = new IntegerLiteral(Loc, Val, IntegerType);
    return;
  }
}

/*
StmtList *Parser::parse() {
  StmtList *Res = parseStatementSequence();
  expect(tok::eof);
  return Res;
}

StmtList *Parser::parseStatementSequence() {
  StmtList *Stmts = new StmtList;
  do {
    Stmt *stmt = parseStatement();
    Stmts->push_back(stmt);
    consume(tok::semi);
  } while (!Tok.is(tok::eof));
  return Stmts;
}

Stmt *Parser::parseStatement() {
  Stmt *Res = nullptr;
  switch(Tok.getKind()) {
  case tok::kw_RETURN:
    Res = parseReturn();
    return Res;
  case tok::ident:
    Res = parseAssign();
    return Res;
  default:
    error();
    return nullptr;
  }
}

Stmt *Parser::parseReturn() {
  consume(tok::kw_RETURN);
  Expr *E = parseExpr();
  Stmt *Res = new RetStmt(E);
  return Res;
}

Stmt *Parser::parseAssign() {
  //Factor *Var = new Factor(Factor::Ident, Tok.getText());
  Factor *Var = (Factor *)parseFactor();
  assert(Var->getKind() == Factor::Ident);
  consume(tok::equal);
  Expr *E = parseExpr();
  Stmt *Res = new AssignStmt(Var, E);
  return Res;
}


Expr *Parser::parseExpr() {
  Expr *Left = parseTerm();
  while (Tok.isOneOf(tok::plus, tok::minus)) {
    BinaryOp::Operator Op = Tok.is(tok::plus) ? BinaryOp::Plus : BinaryOp::Minus;
    advance();
    Expr *Right = parseTerm();
    Left = new BinaryOp(Op, Left, Right);
  }
  return Left;
}

Expr *Parser::parseTerm() {
  Expr *Left = parseFactor();
  while (Tok.isOneOf(tok::star, tok::slash)) {
    BinaryOp::Operator Op = Tok.is(tok::star) ? BinaryOp::Mul : BinaryOp::Div;
    advance();
    Expr *Right = parseFactor();
    Left = new BinaryOp(Op, Left, Right);
  }
  return Left;
}

Expr *Parser::parseFactor() {
  Expr *Res = nullptr;
  switch (Tok.getKind()) {
  case tok::ident:
    Res = new Factor(Factor::Ident, Tok.getText());
    advance();
    break;
  case tok::integer_literal:
    Res = new Factor(Factor::Number, Tok.getText());
    advance();
    break;
  case tok::l_paren:
    advance();
    Res = parseExpr();
    if (!consume(tok::r_paren))
      break;
  default:
    if (!Res) error();
    while (!Tok.isOneOf(tok::r_paren, tok::star,
                        tok::plus, tok::minus,
                        tok::slash, tok::eof))
      advance();
  }
  return Res;
}
*/