## Description
Javascript interpreter written in C++.

## Supported Syntax

## Context Free Grammar

```
program -> statement_list

statement_list -> statement
                | statement ; statement_list

statement -> declare_statement ;
           | function_declare ;
           | assign_statement ;
           | if_statement
           | while_statement
           | return_statement ;
           | call_expression ;

declare_statement -> var ID = expression;

function_declare -> function ID ( parameter_list ) { statement_list }

parameter_list -> ID
                | ID , parameter_list

assign_statement -> ID = expression ;

if_statement -> if ( expression ) { statement }
              | if ( expression ) { statement } else { statement }

while_statement -> while ( expression ) { statement }

return_statement -> return ; 
                  | return expression ;

call_expression -> ID ( argument_list )

argument_list -> expression
               | expression , argument_list

expression -> additive_expression relational_operator additive_expression
            | additive_expression

relational_operator -> <=
                     | >=
                     | ==
                     | <
                     | >
                     | !=

additive_expression -> term 
                     | term + additive_expression
                     | term - additive_expression

term -> factor
      | term * factor
      | term / factor

factor -> REAL
        | INT
        | CHAR
        | STRING
        | BOOL
        | ID
        | call_expression
        | ( expression )

```

## Reference
1. https://github.com/rspivak/lsbasi
2. https://github.com/Xiang1993/jack-compiler