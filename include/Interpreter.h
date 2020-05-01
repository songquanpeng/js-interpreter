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

private:
    Parser parser;
    std::map<std::string, Variable> variableTable;
    Parser::ASTNode *root;
    string visitNode(Parser::ASTNode *node);
    string visitDeclareNode(Parser::ASTNode *node);
    string visitAssignNode(Parser::ASTNode *node);
    string visitExpressionNode(Parser::ASTNode *node);
    static void error(const std::string& message, const std::string& extra="");
    static void log(const std::string& message, const std::string& extra="");

    static string binaryOpt(const string& opt, const string& left, const string& right);
    void printVariableTable();
};


#endif
