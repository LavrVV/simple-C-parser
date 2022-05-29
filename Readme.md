# My simple parser for C like language

This parser created without any parser generator, just vanilla C++.
For now it does not support arrays, compare operators.
No checks for grammar errors for now so it works? only if grammar is correct
TODO: unit test to check if it really works)

## Grammar:
Something like that:
```
parser:
  programm ::= statement* ;
  statement ::= if | for | while | assign | func_call | func_def | var_decl | return | ';' ;
  block ::= '{' statement* '}' ;
  if ::= 'if' '(' expr ')'  block ('else' block)? ;
  for ::= 'for' '(' statement ';' expr ';' statement ')' block ;
  while ::= 'while' '(' expr ')' block
  assign ::= T_NAME '=' expr ;
  func_call ::= // parse in expr
  func_def ::= T_TYPE T_NAME '(' T_TYPE T_NAME (',' T_TYPE T_NAME)* ')' block ;
  var_decl ::= T_TYPE (T_NAME '=' expr | T_NAME ';') ;
  return ::= 'return' expr ;
  expr ::= expr T_OPERATOR expr | '(' expr ')' | func_call
lexer:
  T_TYPE ::= 'int' | 'float' | 'string' | 'char' | 'bool' | 'void' ;
  T_NAME ::= [a-zA-z] [a-zA-z0-9_]*
  T_OPERATOR ::= '*' | '/' | '+' | '-' | '!' | '||' | '&&' ;
```