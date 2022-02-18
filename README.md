# LLVM Frontend for Tiny-C

## Grammer

```
translationUnit : (functionDefinition)*;
functionDefinition : type ident "(" (formalParameterList)? ")" block;
formalParameterList : formalParameter ( "," formalParameter )*;
formalParameter : type ident;

type : "int";
block : "{" ( statement ";" )* "}";
statement : declStmt | assignStmt | returnStmt | ifStmt | whileStmt;
declaration : type ident;
assignStmt : ident ( "=" expr )?;
returnStmt : "return" ( expr )?;
ifStmt : "if" "(" expr ")" block ( "else" block )?;
whileStmt : "while" "(" expr ")" block;

expr : relationalExpr (( "==" | "!=" ) relationExpr )*;
relationalExpr : additiveExpr (( "<" | "<=" | ">" | ">=" ) additiveExpr)*;
additiveExpr : term (( "+" | "-" ) term)*;
term : factor (( "*" | "/" ) factor)*;
factor : number | ident ( "(" ( expr ( "," expr )* )? ")" )? | "(" expr ")";
ident : ([a-zA-Z])+;
number : ([0-9])+;
```