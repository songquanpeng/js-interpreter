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
    string visitNode(Parser::ASTNode *node);
    string visitDeclareNode(Parser::ASTNode *node);
    string visitAssignNode(Parser::ASTNode *node);
    string visitExpressionNode(Parser::ASTNode *node);
    string visitIfNode(Parser::ASTNode *node);
    static void error(const std::string& message, const std::string& extra="");
    void log(const std::string& message, const std::string& extra="");
    static string binaryOpt(const string& opt, const string& left, const string& right);
    void printVariableTable();
    bool shellExecute(const string& input);
};


#endif
