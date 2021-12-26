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

void KeywordFilter::addKeyword(StringRef Keyword, tok::TokenKind Kind) {
  KeywordToKind.insert(std::make_pair(Keyword, Kind));
}

void KeywordFilter::addKeywords() {
#define KEYWORD(ID, SPELL) addKeyword(StringRef(SPELL), tok::kw_##ID);
#include "tiny-c/Basic/TokenKinds.def"
}

void Lexer::run() {
  Token Tok;
  do {
    nextToken(Tok);
    Tokens.push_back(Tok);
  } while (Tok.isNot(tok::eof));
}

Token &Lexer::next() {
  if ((Nth + 1) >= Tokens.size()) {
    // error
    exit(1);
  }
  return Tokens.at(++Nth);
}

Token &Lexer::peek(int n) {
  unsigned int idx = Nth + n;
  if (idx >= Tokens.size()) {
    // error
    exit(1);
  }
  return Tokens.at(idx);
}

void Lexer::nextToken(Token &Result) {
  while (*CurPtr && charinfo::isWhitespace(*CurPtr))
    CurPtr++;
  if (*CurPtr == '\x00') {
    Result.Kind = tok::eof;
    return;
  }
  if (charinfo::isLetter(*CurPtr)) {  // ident | "return"
    const char *end = CurPtr + 1;
    while (charinfo::isLetter(*end))
      end++;
    StringRef Name(CurPtr, end - CurPtr);
    formToken(Result, end, Keywords.getFilteredKind(Name, tok::ident));
    return;
  } else if (charinfo::isDigit(*CurPtr)) {  // number
    const char *end = CurPtr + 1;
    while (charinfo::isDigit(*end))
      end++;
    formToken(Result, end, tok::integer_literal);
    return;
  } else {
    switch (*CurPtr) {
#define CASE(ch, tok) \
case ch: formToken(Result, CurPtr + 1, tok); break
CASE('+', tok::plus);
CASE('-', tok::minus);
CASE('*', tok::star);
CASE('/', tok::slash);
CASE(',', tok::comma);
CASE(';', tok::semi);
CASE('=', tok::equal);
CASE('(', tok::l_paren);
CASE(')', tok::r_paren);
CASE('{', tok::l_curl);
CASE('}', tok::r_curl);
#undef CASE
      default: formToken(Result, CurPtr + 1, tok::unknown);
    }
  }
  return;
}

void Lexer::formToken(Token &Result, const char *TokEnd, tok::TokenKind Kind) {
  size_t TokLen = TokEnd - CurPtr;
  Result.Ptr = CurPtr;
  Result.Length = TokLen;
  Result.Kind = Kind;
  CurPtr = TokEnd;
}

//void Lexer::next(Token &Result) {
  //while (*BufferPtr && charinfo::isWhitespace(*BufferPtr)) {
    //BufferPtr++;
  //}
  //if (*BufferPtr == '\x00') {
    //Result.Kind = Token::eoi;
    //return;
  //}
  //if (charinfo::isLetter(*BufferPtr)) {
    //const char *end = BufferPtr + 1;
    //while (charinfo::isLetter(*end))
      //end++;
    //llvm::StringRef Name(BufferPtr, end - BufferPtr);
    //Token::TokenKind Kind = Name == "return" ? Token::KW_return : Token::ident;
    //formToken(Result, end, Kind);
    //return;
  //} else if (charinfo::isDigit(*BufferPtr)) {
    //const char *end = BufferPtr + 1;
    //while (charinfo::isDigit(*end))
      //end++;
    //formToken(Result, end, Token::number);
    //return;
  //} else {
    //switch (*BufferPtr) {
//#define CASE(ch, tok) \
//case ch: formToken(Result, BufferPtr + 1, tok); break
//CASE(';', Token::semicolon);
//CASE('=', Token::equal);
//CASE('+', Token::plus);
//CASE('-', Token::minus);
//CASE('*', Token::star);
//CASE('/', Token::slash);
//CASE('(', Token::l_paren);
//CASE(')', Token::r_paren);
//#undef CASE
      //default:
        //formToken(Result, BufferPtr + 1, Token::unknown);
    //}
    //return;
  //}
//}

//void Lexer::formToken(Token &Tok, const char *TokEnd, Token::TokenKind Kind) {
  //Tok.Kind = Kind;
  //Tok.Text = llvm::StringRef(BufferPtr, TokEnd - BufferPtr);
  //BufferPtr = TokEnd;
//}