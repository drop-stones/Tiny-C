#ifndef TINY_C_CODE_GEN_H
#define TINY_C_CODE_GEN_H

#include "tiny-c/AST/AST.h"

class CodeGen {
public:
  void compile(AST *Tree);
};

#endif