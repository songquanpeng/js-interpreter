#include "Interpreter.h"
#include <iostream>
#include <cassert>

using namespace std;

void Interpreter::interpretFile(string &filename) {
    parser.parseFile(filename);
    root = parser.getAST();
    visitAST(root);
}

void Interpreter::error(const string &message, Parser::ASTNode *node) {
    cerr << "[Interpreter] [Error]: " << message << " | [Node]: " << node->type << endl;
    exit(-1);
}

void Interpreter::log(const string &message, Parser::ASTNode *node) {
    cout << "[Interpreter] [Log]: " << message << " | [Node]: " << node->type << endl;
}

void Interpreter::visitAST(Parser::ASTNode *node) {
    switch(node->type) {
        case Parser::VAR_DECLARE_NODE:
            visitDeclareNode(node);
            break;
        default:
            break;
    }
}


void Interpreter::visitDeclareNode(Parser::ASTNode *node) {
    Variable var;
    var.type = node->token.type;
    string varName = node->token.value;
    switch(node->child[0]->type) {
        case Parser::INT_NODE:
        case Parser::REAL_NODE:
        case Parser::STRING_NODE:
        case Parser::CHAR_NODE:
        case Parser::BOOL_NODE:
            var.value = node->child[0]->token.value;
            break;
        case Parser::BINARY_OPERATOR_NODE:
            var.value = visitBinaryOptNode(node->child[0]);
            break;
        default:
            error("unexpected node type", node);
            break;
    }
    variableTable.insert({varName, var});
}

string Interpreter::visitBinaryOptNode(Parser::ASTNode *node) {
    string left = visitExpressionNode(node->child[0]);
    string right = visitExpressionNode(node->child[1]);

}

string Interpreter::visitExpressionNode(Parser::ASTNode *node) {

}



