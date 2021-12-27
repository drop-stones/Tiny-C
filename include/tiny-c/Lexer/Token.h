#ifndef TINY_C_LEXER_TOKEN_H
#define TINY_C_LEXER_TOKEN_H

#include "tiny-c/Basic/LLVM.h"
#include "tiny-c/Basic/TokenKinds.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/SMLoc.h"

class Lexer;

class Token {
  friend class Lexer;

  /// The location of the token.
  const char *Ptr;

  /// The length of the token.
  size_t Length;

  /// The Kind of the token.
  tok::TokenKind Kind;

public:
  tok::TokenKind getKind() const { return Kind; }
  void setKind(tok::TokenKind K) { Kind = K; }

  /// is/isNot - Predicates to check if this token is a specific kind or not.
  bool is(tok::TokenKind K) const { return Kind == K; }
  bool isNot(tok::TokenKind K) const { return Kind != K; }
  bool isOneOf(tok::TokenKind K1, tok::TokenKind K2) const {
    return is(K1) || is(K2);
  }
  template <typename... Ts>
  bool isOneOf(tok::TokenKind K1, tok::TokenKind K2, Ts... Ks) const {
    return is(K1) || isOneOf(K2, Ks...);
  }

  SMLoc getLocation() const {
    return SMLoc::getFromPointer(Ptr);
  }
  size_t getLength() const { return Length; }

  StringRef getText() {
    return StringRef(Ptr, Length);
  }

  StringRef getIdentifier() {
    assert(is(tok::ident));
    return StringRef(Ptr, Length);
  }

  StringRef getLiteralData() {
    assert(isOneOf(tok::integer_literal, tok::string_literal));
    return StringRef(Ptr, Length);
  }
};

#endif