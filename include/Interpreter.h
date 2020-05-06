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
    Interpreter();
    void interpretFile(std::string& filename);
    void shell();
    void setDebugMode(bool enable);

private:
    Parser parser;
    std::map<std::string, Parser::ASTNode*> functionTable;
    std::vector<std::map<std::string, Variable>*> variableTable;
    std::map<std::string, std::vector<std::string>*> arrayTable;
    std::vector<string>* getArray(const std::string& name, bool isIdentifier=false);
    string copyArray(const std::string& identifier);
    std::string returnValue;
    int scopeLevel;
    void enterScope();
    void exitScope();
    bool declareVariable(const std::string& name, const Variable& variable);
    bool setVariableValue(const std::string& name, const Variable& variable);
    string getVariableValue(const std::string& name);
    void printVariableTable();
    Parser::ASTNode* getFunction(const std::string& name);
    Parser::ASTNode *root;
    bool debug = false;
    static void error(const std::string& message, const std::string& extra="");
    void log(const std::string& message, const std::string& extra="");
    bool shellExecute(const string& input);
    static string input();
    static void output(const std::string& str);
    string visitNode(Parser::ASTNode *node);
    string visitDeclareNode(Parser::ASTNode *node);
    string visitAssignNode(Parser::ASTNode *node);
    string visitExpressionNode(Parser::ASTNode *node);
    string visitIfNode(Parser::ASTNode *node);
    string visitNegativeNode(Parser::ASTNode *node);
    string visitBinaryOperatorNode(Parser::ASTNode *node);
    string visitWhileNode(Parser::ASTNode *node);
    string visitForNode(Parser::ASTNode *node);
    string visitFunctionDeclareNode(Parser::ASTNode *node);
    string visitFunctionCallNode(Parser::ASTNode *node);
    string visitReturnNode(Parser::ASTNode *node);
    string visitArrayDeclareNode(Parser::ASTNode *node);
    string visitArrayAccessNode(Parser::ASTNode *node);
};


#endif
