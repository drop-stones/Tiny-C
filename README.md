# LLVM Frontend for Tiny-C

## Grammer for local variables

```
statementSequence : statement ( ";" statement )*;
statement : assign | "return" ( expr )?;
assign : ident ( "=" expr )?;
expr : term (( "+" | "-" ) term)*;
term : factor (( "*" | "/" ) factor)*;
factor : number | ident | "(" expr ")";
ident : ([a-zA-Z])+;
number : ([0-9])+;
```