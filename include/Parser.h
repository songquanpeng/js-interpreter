#ifndef _PARSR_H
#define _PARSER_H

#include "Lexer.h"
#include <string>
#include <deque>
#include <vector>

class Parser {
public:
    enum NodeType {
        NONE,
        CLASS,
        VAR
    };
    class ASTNode {
    public:
        Lexer::Token token;
        ASTNode* childred[5];
        ASTNode* next;
        NodeType type;
        ASTNode() {
            type = NONE;
            childred[0] = childred[1] = childred[2] = childred[3] = childred[4] = nullptr;
            next = nullptr;
        }
    };
private:
    ASTNode* root;
    Lexer lexer;
    Lexer::Token getToken();
    Lexer::Token restoreToken();
    std::deque<Lexer::Token> leftTokenBuffer;
    std::deque<Lexer::Token> rightTokenBuffer;
    void parseProgrem();
    ASTNode* parseSTH();
    
public:
    Parser(std::string &filename);
    ASTNode* getAST();
    void printAST();
};

#endif