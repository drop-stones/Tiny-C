#include "tiny-c/Parser/Parser.h"

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