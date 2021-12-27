#ifndef TINY_C_LEXER_TOKENKINDS_H
#define TINY_C_LEXER_TOKENKINDS_H

#include "llvm/Support/Compiler.h"

namespace tok {
enum TokenKind : unsigned short {
#define TOK(ID) ID,
#include "TokenKinds.def"
  NUM_TOKENS
};
} // namespace tok

#endif