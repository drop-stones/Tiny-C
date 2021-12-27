#include "tiny-c/Parser/Parser.h"

TranslationUnitDecl *Parser::parse() {
  TranslationUnitDecl *Res;
  parseTranslationUnit(Res);
  expect(tok::eof);
  return Res;
}

void Parser::parseTranslationUnit(TranslationUnitDecl *&T) {
  FuncList Funcs;
  while (Tok.isNot(tok::eof)) {
    parseFunc(Funcs);
  }
  T = new TranslationUnitDecl(SMLoc(), "TransUnit", Funcs);
}

void Parser::parseFunc(FuncList &Funcs) {
  ParmVarList Parms;
  StmtList Stmts;
  DeclList Decls;
  SMLoc Loc = Tok.getLocation();
  consume(tok::kw_INTEGER);
  StringRef FuncName = Tok.getText();
  consume(tok::ident);

  FuncDecl *Func = new FuncDecl(Loc, FuncName, IntegerType, Parms, Stmts, Decls);
  CurScope = Func;

  consume(tok::l_paren);
  // TODO: Parms
  consume(tok::r_paren);
  consume(tok::l_curl);

  while (Tok.isNot(tok::r_curl)) {
    parseStmt(Stmts, Decls);
    consume(tok::semi);
  }
  consume(tok::r_curl);

  Func->setParms(Parms);
  Func->setStmts(Stmts);
  Func->setDecls(Decls);

  Funcs.push_back(Func);
}

void Parser::parseStmt(StmtList &Stmts, DeclList &Decls) {
  if (Tok.is(tok::kw_INTEGER)) { // DeclStmt
    TypeDecl *Ty = new TypeDecl(Tok.getLocation(), Tok.getText());
    consume(tok::kw_INTEGER);
    VarDecl *Var = new VarDecl(Tok.getLocation(), Tok.getText(), Ty);
    consume(tok::ident);
    DeclStmt *Decl = new DeclStmt(Var);
    Stmts.push_back(Decl);
    Decls.push_back(Var);
    setVarDecl(Var);
    return;
  } else if (Tok.is(tok::ident) && Lex.peek(1).is(tok::equal)) {  // AssignStmt
    VarDecl *Var = getVarDecl(Tok.getText());
    consume(tok::ident);
    consume(tok::equal);
    Expr *E = nullptr;
    parseExpr(E);
    AssignStmt *Assign = new AssignStmt(Var, E);
    Stmts.push_back(Assign);
    return;
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
  Expr *Left = nullptr;
  parseTerm(Left);
  while (Tok.isOneOf(tok::plus, tok::minus)) {
    Operator Op(Tok.getLocation(), Tok.getKind());
    advance();
    Expr *Right = nullptr;
    parseTerm(Right);
    Left = new BinaryOp(Left, Right, Op, Left->getType());
  }
  E = Left;
  if (Tok.is(tok::integer_literal)) {
    SMLoc Loc = Tok.getLocation();
    llvm::APSInt Val{Tok.getText()};
    consume(tok::integer_literal);
    E = new IntegerLiteral(Loc, Val, IntegerType);
    return;
  }
}

void Parser::parseTerm(Expr *&E) {
  Expr *Left = nullptr;
  parseFactor(Left);
  while (Tok.isOneOf(tok::star, tok::slash)) {
    Operator Op(Tok.getLocation(), Tok.getKind());
    advance();
    Expr *Right = nullptr;
    parseFactor(Right);
    Left = new BinaryOp(Left, Right, Op, Left->getType());
  }
  E = Left;
}

void Parser::parseFactor(Expr *&E) {
  SMLoc Loc = Tok.getLocation();

  switch (Tok.getKind()) {
  case tok::ident: {
    VarDecl *Var = getVarDecl(Tok.getText());
    consume(tok::ident);
    E = new DeclRef(Var);
    return;
  }
  case tok::integer_literal: {
    llvm::APSInt Val{Tok.getText()};
    consume(tok::integer_literal);
    E = new IntegerLiteral(Loc, Val, IntegerType);
    return;
  }
  case tok::l_paren:
    consume(tok::l_paren);
    parseExpr(E);
    consume(tok::r_paren);
    return;
  default:
    error();
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