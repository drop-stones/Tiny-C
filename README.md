# LLVM Frontend for Tiny-C

## Grammer for functions

```
compilationUnit : (functionDefinition)*;
functionDefinition : type ident "(" (formalParameterList)? ")" block;
formalParameterList : formalParameter ( "," formalParameter )*;
formalParameter : type ident;

type : "int";
block : "{" ( (declaration | statement) ";" )* "}";
declaration : type ident;
statement : assignStmt | returnStmt;
assignStmt : ident ( "=" expr )?;
returnStmt : "return" ( expr )?;

expr : term (( "+" | "-" ) term)*;
term : factor (( "*" | "/" ) factor)*;
factor : number | ident | "(" expr ")";
ident : ([a-zA-Z])+;
number : ([0-9])+;
```