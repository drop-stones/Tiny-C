# LLVM Frontend for Tiny-C

## Grammer for calculation

```
expr : term (( "+" | "-" ) term)*;
term : factor (( "*" | "/" ) factor)*;
factor : number | "(" expr ")";
number : ([0-9])+;
```