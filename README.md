## Description
Javascript interpreter written in C++.

## TODO List
- [x] Support variable declare statement.
- [x] Support assign statement.
- [x] Support negative number. 
- [x] Interactive mode.
- [x] Debug mode.
- [x] Support if statement.
- [x] Support while loop statement.
- [x] Support for loop statement.
- [ ] Support continue & break;
- [x] Support function definition.
- [x] Support function call.
- [ ] Support class.
- [x] Support multi-level scope.
- [x] Implement necessary built-in functions.
    - [x] output(str)
    - [x] input()
- [ ] When error occurred in interactive mode, do not exit but try to recover.
- [ ] ~~Fix the operator's priority problem.~~ (Always use parentheses can avoid this problem)
- [ ] Support more operators:
    - [ ] %
    - [ ] ===
    - [ ] !==
- [ ] Support array.
- [x] Make the `;` optional.

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
           | for_statement
           | return_statement ;

declare_statement -> var ID = expression;

function_declare -> function ID ( parameter_list ) { statement_list }

parameter_list -> ID
                | ID , parameter_list

assign_statement -> ID = expression ;

if_statement -> if ( expression ) { statement }
              | if ( expression ) { statement } else { statement }

while_statement -> while ( expression ) { statement }

for_statement -> for ( declare_statement ; expression ; assign_statement) { statement }

return_statement -> return ; 
                  | return expression ;

call_expression -> ID ( argument_list )

array_access_expression -> ID [expression]

argument_list -> expression
               | expression , argument_list

expression -> additive_expression relational_operator additive_expression
            | additive_expression
            | array_declare_expression

array_declare_expression -> [factor_list]

factor_list -> factor
             | factor_list

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
        | array_access_expression
        | ( expression )

```

## Reference
1. https://github.com/rspivak/lsbasi
2. https://github.com/Xiang1993/jack-compiler