# LLVM Frontend for Tiny-C

## Usage

### Compilation
```
$ mkdir build
$ cd build
$ cmake -G Ninja .
$ ninja
```

### Testing

Run Lexer and Parser tests.
```
$ ninja test
```

## Supported Feature

- Binary Expressions
  - `+ - * / == !=`
- if-, while-, return- statements
- Local variables
  - Declaration: `int x;`
  - Assignment statement: `x = 100;`
- `int` type
- Function
  - Declaration: `int add(int a, int b) {...}`
  - Function Call: `x = add(100, 200);`

### Grammar
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

## Unsupported Feature

- Scope Analysis
- Semantic Analysis
- Types other than `int`
- Pointer Type
- Array Type