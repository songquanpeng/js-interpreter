#ifndef _INTERPRETER_H
#define _INTERPRETER_H

#include "Parser.h"
#include <map>
#include <string>
#include <vector>

class Interpreter {
public:
    class Variable {
    public:
        Lexer::TokenType type;
        std::string value;
    };
    void interpretFile(std::string& filename);

private:
    Parser parser;
    std::map<std::string, Variable> variableTable;
    Parser::ASTNode *root;
    void visitAST(Parser::ASTNode *node);
    void visitDeclareNode(Parser::ASTNode *node);
    std::string visitExpressionNode(Parser::ASTNode *node);
    std::string visitBinaryOptNode(Parser::ASTNode *node);
    void error(const std::string& message, Parser::ASTNode *node);
    void log(const std::string& message, Parser::ASTNode *node);
};


#endif
