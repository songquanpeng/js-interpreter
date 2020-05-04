#ifndef _INTERPRETER_H
#define _INTERPRETER_H

#include "Parser.h"
#include <map>
#include <string>
#include <vector>

using std::string;

class Interpreter {
public:
    class Variable {
    public:
        Lexer::TokenType type;
        std::string value;
    };
    void interpretFile(std::string& filename);
    void shell();
    void setDebugMode(bool enable);

private:
    Parser parser;
    std::map<std::string, Variable> variableTable;
    string getVariableValue(const std::string& name);
    Parser::ASTNode *root;
    bool debug = false;
    static void error(const std::string& message, const std::string& extra="");
    void log(const std::string& message, const std::string& extra="");
    void printVariableTable();
    bool shellExecute(const string& input);
    string visitNode(Parser::ASTNode *node);
    string visitDeclareNode(Parser::ASTNode *node);
    string visitAssignNode(Parser::ASTNode *node);
    string visitExpressionNode(Parser::ASTNode *node);
    string visitIfNode(Parser::ASTNode *node);
    string visitNegativeNode(Parser::ASTNode *node);
    string visitBinaryOperatorNode(Parser::ASTNode *node);
};


#endif
