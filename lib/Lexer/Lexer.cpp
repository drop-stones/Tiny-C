#include "tiny-c/Lexer/Lexer.h"

namespace charinfo {

LLVM_READNONE inline bool isWhitespace(char c) {
  return c == ' '  || c == '\t' || c == '\f' ||
         c == '\v' || c == '\r' || c == '\n';
}

LLVM_READNONE inline bool isDigit(char c) {
  return '0' <= c && c <= '9';
}

LLVM_READNONE inline bool isLetter(char c) {
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

} // namespace charinfo

void Lexer::next(Token &Result) {
  while (*BufferPtr && charinfo::isWhitespace(*BufferPtr)) {
    BufferPtr++;
  }
  if (*BufferPtr == '\x00') {
    Result.Kind = Token::eoi;
    return;
  }
  if (charinfo::isLetter(*BufferPtr)) {
    const char *end = BufferPtr + 1;
    while (charinfo::isLetter(*end))
      end++;
    llvm::StringRef Name(BufferPtr, end - BufferPtr);
    Token::TokenKind Kind = Name == "return" ? Token::KW_return : Token::ident;
    formToken(Result, end, Kind);
    return;
  } else if (charinfo::isDigit(*BufferPtr)) {
    const char *end = BufferPtr + 1;
    while (charinfo::isDigit(*end))
      end++;
    formToken(Result, end, Token::number);
    return;
  } else {
    switch (*BufferPtr) {
#define CASE(ch, tok) \
case ch: formToken(Result, BufferPtr + 1, tok); break
CASE(';', Token::semicolon);
CASE('=', Token::equal);
CASE('+', Token::plus);
CASE('-', Token::minus);
CASE('*', Token::star);
CASE('/', Token::slash);
CASE('(', Token::l_paren);
CASE(')', Token::r_paren);
#undef CASE
      default:
        formToken(Result, BufferPtr + 1, Token::unknown);
    }
    return;
  }
}

void Lexer::formToken(Token &Tok, const char *TokEnd, Token::TokenKind Kind) {
  Tok.Kind = Kind;
  Tok.Text = llvm::StringRef(BufferPtr, TokEnd - BufferPtr);
  BufferPtr = TokEnd;
}