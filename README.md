## Description
Javascript interpreter written in C++.

## TODO List
- [x] Support var declare statement: `var b = 2 * a - 4;`
- [x] Support assign statement: `c = a * b / c;`
- [x] Support negative number. 
- [x] Interactive mode.
- [x] Debug mode.
- [x] Support if statement.
- [ ] Support while loop statement.
- [ ] Support for loop statement.
- [ ] Support function definition.
- [ ] Support function call.
- [ ] Support class.
- [ ] Support multi-level scope.
- [ ] Implement necessary built-in functions.
    - [ ] output(value)
    - [ ] input(prompt)
- [ ] When error occurred in interactive mode, do not exit but try to recover.

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
                     | &&
                     | ||

additive_expression -> term 
                     | term + additive_expression
                     | term - additive_expression

term -> factor
      | term * factor
      | term / factor

factor -> positive_factor
        | - positive_factor

positive_factor -> REAL
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