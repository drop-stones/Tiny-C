#ifndef TINY_C_LEXER_H
#define TINY_C_LEXER_H

#include "tiny-c/Basic/LLVM.h"
#include "tiny-c/Lexer/Token.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/MemoryBuffer.h"

using TokenList = std::vector<Token>;

class KeywordFilter {
  llvm::StringMap<tok::TokenKind> KeywordToKind;

  void addKeyword(StringRef Keyword, tok::TokenKind TokenCode);

public:
  void addKeywords();

  tok::TokenKind
  getFilteredKind(StringRef Name, tok::TokenKind DefaultTokenKind = tok::unknown) {
    auto Result = KeywordToKind.find(Name);
    if (Result != KeywordToKind.end())
      return Result->second;
    return DefaultTokenKind;
  }
};

class Lexer {
  const char *BufferStart;
  const char *CurPtr;

  /// All Token list
  TokenList Tokens;

  /// Current index of TokenList.
  int Nth;

  KeywordFilter Keywords;

public:
  Lexer(const llvm::StringRef &Buffer) {
    BufferStart = Buffer.begin();
    CurPtr = BufferStart;
    Nth = -1;
    Keywords.addKeywords();
  }

  /// Fill TokenList by all tokens in Buffer.
  void run();

  /// Go to next and returns the token.
  Token &next();

  /// Returns the n-th token after the current token.
  Token &peek(int n);

private:
  void nextToken(Token &Result);
  void formToken(Token &Result, const char *TokEnd, tok::TokenKind Kind);
};

#endif