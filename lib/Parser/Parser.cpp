#include "tiny-c/Parser/Parser.h"

StmtList *Parser::parse() {
  StmtList *Res = parseStatementSequence();
  expect(Token::eoi);
  return Res;
}

StmtList *Parser::parseStatementSequence() {
  StmtList *Stmts = new StmtList;
  do {
    Stmt *stmt = parseStatement();
    Stmts->push_back(stmt);
    consume(Token::semicolon);
  } while (!Tok.is(Token::eoi));
  return Stmts;
}

Stmt *Parser::parseStatement() {
  Stmt *Res = nullptr;
  switch(Tok.getKind()) {
  case Token::KW_return:
    Res = parseReturn();
    return Res;
  case Token::ident:
    Res = parseAssign();
    return Res;
  default:
    error();
    return nullptr;
  }
}

Stmt *Parser::parseReturn() {
  consume(Token::KW_return);
  Expr *E = parseExpr();
  Stmt *Res = new RetStmt(E);
  return Res;
}

Stmt *Parser::parseAssign() {
  //Factor *Var = new Factor(Factor::Ident, Tok.getText());
  Factor *Var = (Factor *)parseFactor();
  assert(Var->getKind() == Factor::Ident);
  consume(Token::equal);
  Expr *E = parseExpr();
  Stmt *Res = new AssignStmt(Var, E);
  return Res;
}


Expr *Parser::parseExpr() {
  Expr *Left = parseTerm();
  while (Tok.isOneOf(Token::plus, Token::minus)) {
    BinaryOp::Operator Op = Tok.is(Token::plus) ? BinaryOp::Plus : BinaryOp::Minus;
    advance();
    Expr *Right = parseTerm();
    Left = new BinaryOp(Op, Left, Right);
  }
  return Left;
}

Expr *Parser::parseTerm() {
  Expr *Left = parseFactor();
  while (Tok.isOneOf(Token::star, Token::slash)) {
    BinaryOp::Operator Op = Tok.is(Token::star) ? BinaryOp::Mul : BinaryOp::Div;
    advance();
    Expr *Right = parseFactor();
    Left = new BinaryOp(Op, Left, Right);
  }
  return Left;
}

Expr *Parser::parseFactor() {
  Expr *Res = nullptr;
  switch (Tok.getKind()) {
  case Token::ident:
    Res = new Factor(Factor::Ident, Tok.getText());
    advance();
    break;
  case Token::number:
    Res = new Factor(Factor::Number, Tok.getText());
    advance();
    break;
  case Token::l_paren:
    advance();
    Res = parseExpr();
    if (!consume(Token::r_paren))
      break;
  default:
    if (!Res) error();
    while (!Tok.isOneOf(Token::r_paren, Token::star,
                        Token::plus, Token::minus,
                        Token::slash, Token::eoi))
      advance();
  }
  return Res;
}