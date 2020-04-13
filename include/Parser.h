#ifndef _PARSER_H
#define _PARSER_H

#include "Lexer.h"
#include <string>
#include <deque>
#include <vector>

class Parser {
public:
    enum NodeType {
        NONE,
        PROGRAM_NODE,
        VAR_NODE,
        UNARY_OPERATOR_NODE,
        BINARY_OPERATOR_NODE,
        FUNCTION_DECLARE_NODE,
        RETURN_NODE,
        FUNCTION_CALL_NODE,
        VAR_DECLARE_NODE,
        VAR_ASSIGN_NODE,
        COMPARE_NODE,
        IF_NODE,
        INT_NODE,
        REAL_NODE,
        STRING_NODE,
        CHAR_NODE,
        BOOL_NODE,
        WHILE_NODE
    };
    class ASTNode {
    public:
        Lexer::Token token;
        ASTNode *child[5];
        ASTNode *next;
        NodeType type;
        ASTNode() {
            type = NONE;
            child[0] = child[1] = child[2] = child[3] = child[4] = nullptr;
            next = nullptr;
        }
    };
private:
    ASTNode *root;
    Lexer lexer;
    Lexer::Token getToken();
    void restoreToken();
    std::deque<Lexer::Token> leftTokenBuffer;
    std::deque<Lexer::Token> rightTokenBuffer;
    void error(const std::string& message, Lexer::Token token);
    void log(const std::string& message, Lexer::Token token);
    void parseProgram();
    ASTNode *parseStatementList();
    ASTNode *parseStatement();
    ASTNode *parseDeclareStatement();
    ASTNode *parseParameterList();
    ASTNode *parseAssignStatement();
    ASTNode *parseIfStatement();
    ASTNode *parseWhileStatement();
    ASTNode *parseReturnStatement();
    ASTNode *parseFunction();
    ASTNode *parseCallExpression();
    ASTNode *parseArgumentList();
    ASTNode *parseExpression();
    ASTNode *parseAdditiveExpression();
    ASTNode *parseTerm();
    ASTNode *parseFactor();
    
public:
    explicit Parser(std::string &filename);
    ASTNode *getAST();
    void printAST();
};

#endif