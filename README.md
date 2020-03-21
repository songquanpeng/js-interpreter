## Description
Javascript interpreter written in C++.

## Supported Syntax

## Context Free Grammer

```javascript
program -> statement_list

statement_list -> statement
                | statement ; statement_list

statement -> decalre_statement ;
           | assign_statement ;
           | if_statement ;
           | while_statement ;
           | return_statement ;
           | call_statement ;

decalre_statement -> var ID = expression;
                   | function ID ( parameter_list ) { statement_list }

parameter_list -> ID
                | ID , parameter_list

assignment_statement -> ID = expression ;

if_statement -> if ( expression ) { statement }
              | if ( expression ) { statement } else { statement }

while_statement -> while ( expression ) { statement }

return_statement -> return ; 
                  | return expression ;

call_statement -> ID ( argument_list )

argument_list -> expression
               | expression , argument_list

expression_list -> expression
                 | expression , expression_list

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
        | ID
        | call_statement
        | ( expression )

```

## Reference
1. https://github.com/rspivak/lsbasi
2. https://github.com/Xiang1993/jack-compiler