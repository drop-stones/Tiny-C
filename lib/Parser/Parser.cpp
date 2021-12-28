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

  parseBlock(Stmts, Decls);
  //consume(tok::l_curl);
  //while (Tok.isNot(tok::r_curl)) {
  //  parseStmt(Stmts, Decls);
  //  consume(tok::semi);
  //}
  //consume(tok::r_curl);

  Func->setParms(Parms);
  Func->setStmts(Stmts);
  Func->setDecls(Decls);

  Funcs.push_back(Func);
}

void Parser::parseBlock(StmtList &Stmts, DeclList &Decls) {
  consume(tok::l_curl);
  while (Tok.isNot(tok::r_curl)) {
    parseStmt(Stmts, Decls);
  }
  consume(tok::r_curl);
}

void Parser::parseStmt(StmtList &Stmts, DeclList &Decls) {
  if (Tok.is(tok::kw_INTEGER)) { // DeclStmt
    TypeDecl *Ty = new TypeDecl(Tok.getLocation(), Tok.getText());
    consume(tok::kw_INTEGER);
    VarDecl *Var = new VarDecl(Tok.getLocation(), Tok.getText(), Ty);
    consume(tok::ident);
    consume(tok::semi);
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
    consume(tok::semi);
    AssignStmt *Assign = new AssignStmt(Var, E);
    Stmts.push_back(Assign);
    return;
  } else if (Tok.is(tok::kw_RETURN)) {  // ReturnStmt
    Expr *E;
    SMLoc Loc = Tok.getLocation();
    consume(tok::kw_RETURN);
    parseExpr(E);
    consume(tok::semi);
    ReturnStmt *Ret = new ReturnStmt(E);
    Stmts.push_back(Ret);
    return;
  } else if (Tok.is(tok::kw_IF)) {  // IfStmt
    Expr *Cond;
    StmtList ThenBlock, ElseBlock;
    SMLoc Loc = Tok.getLocation();
    consume(tok::kw_IF);
    consume(tok::l_paren);
    parseExpr(Cond);
    consume(tok::r_paren);
    parseBlock(ThenBlock, Decls);
    if (Tok.is(tok::kw_ELSE)) {
      consume(tok::kw_ELSE);
      parseBlock(ElseBlock, Decls);
    }
    IfStmt *If = new IfStmt(Cond, ThenBlock, ElseBlock);
    Stmts.push_back(If);
    return;
  } else if (Tok.is(tok::kw_WHILE)) { // WhileStmt
    Expr *Cond;
    StmtList Block;
    SMLoc Loc = Tok.getLocation();
    consume(tok::kw_WHILE);
    consume(tok::l_paren);
    parseExpr(Cond);
    consume(tok::r_paren);
    parseBlock(Block, Decls);
    WhileStmt *While = new WhileStmt(Cond, Block);
    Stmts.push_back(While);
    return;
  }
}

void Parser::parseExpr(Expr *&E) {
  Expr *Left = nullptr;
  parseRelationalExpr(Left);
  while (Tok.isOneOf(tok::equalequal, tok::notequal)) {
    Operator Op(Tok.getLocation(), Tok.getKind());
    advance();
    Expr *Right = nullptr;
    parseRelationalExpr(Right);
    Left = new BinaryOp(Left, Right, Op, Left->getType());
  }
  E = Left;
}

void Parser::parseRelationalExpr(Expr *&E) {
  Expr *Left = nullptr;
  parseAdditiveExpr(Left);
  while (Tok.isOneOf(tok::less, tok::lessequal, tok::greater, tok::greaterequal)) {
    Operator Op(Tok.getLocation(), Tok.getKind());
    advance();
    Expr *Right = nullptr;
    parseAdditiveExpr(Right);
    Left = new BinaryOp(Left, Right, Op, Left->getType());
  }
  E = Left;
}

void Parser::parseAdditiveExpr(Expr *&E) {
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
