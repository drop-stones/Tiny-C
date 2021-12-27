# LLVM Frontend for Tiny-C

## Grammer for functions

```
translationUnit : (functionDefinition)*;
functionDefinition : type ident "(" (formalParameterList)? ")" block;
formalParameterList : formalParameter ( "," formalParameter )*;
formalParameter : type ident;

type : "int";
block : "{" ( statement ";" )* "}";
statement : declStmt | assignStmt | returnStmt;
declaration : type ident;
assignStmt : ident ( "=" expr )?;
returnStmt : "return" ( expr )?;

expr : term (( "+" | "-" ) term)*;
term : factor (( "*" | "/" ) factor)*;
factor : number | ident | "(" expr ")";
ident : ([a-zA-Z])+;
number : ([0-9])+;
```