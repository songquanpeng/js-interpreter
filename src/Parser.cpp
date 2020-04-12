#include "Parser.h"
#include <iostream>
#include <string>
#include <cassert>

using namespace std;

Parser::Parser(string &filename) {
    lexer.openFile(filename);
    root = nullptr;
    parseProgram();
}

// Load next token.
Lexer::Token Parser::getToken() {
    if (rightTokenBuffer.size() == 0) {
        auto token = lexer.nextToken();
        leftTokenBuffer.push_back(token);
        if(leftTokenBuffer.size() > 10) leftTokenBuffer.pop_front();
    } else {
        auto token = rightTokenBuffer.front();
        rightTokenBuffer.pop_front();
        leftTokenBuffer.push_back(token);
    }
    log("get token ", leftTokenBuffer.back());
    return leftTokenBuffer.back();
}

// Restore current token.
void Parser::restoreToken() {
    auto token = leftTokenBuffer.back();
    leftTokenBuffer.pop_back();
    rightTokenBuffer.push_front(token);
    log("restore token ", token);
}

void Parser::error(string message, Lexer::Token token) {
    cerr << "[Parser] [Error]: " << message << " | [Token]: " << lexer.tokenToString(token) << endl;
    exit(-1);
}

void Parser::log(string message, Lexer::Token token) {
    cout << "[Parser] [Log]: " << message << " | [Token]: " << lexer.tokenToString(token) << endl;
}

void Parser::printAST() {
    // TODO
}

Parser::ASTNode *Parser::getAST() {
    return root;
}

void Parser::parseProgram() {
    root = parseStatementList();
}

Parser::ASTNode *Parser::parseStatementList() {
    ASTNode *node = parseStatement();
    ASTNode *current = node;
    Lexer::Token token = getToken();
    while (token.type == Lexer::SYMBOL && token.value == ";") {
        current->next = parseStatement();
        current = current->next;
        token = getToken();
    }
    restoreToken();
    return node;
}

Parser::ASTNode *Parser::parseStatement() {
    Parser::ASTNode *node = nullptr;
    Lexer::Token token = getToken();
    if (token.value == "if") {
        restoreToken();
        node = parseIfStatement();
    } else if (token.value == "while") {
        restoreToken();
        node = parseWhileStatement();
    } else if (token.value == "return") {
        restoreToken();
        node = parseReturnStatement();
    } else if (token.value == "var") {
        restoreToken();
        node = parseDeclareStatement();
    } else if (token.type == Lexer::ID) {
        token = getToken();
        if (token.value == "=") {
            restoreToken();
            restoreToken();
            node = parseAssignStatement();
        } else if (token.value == "(") {
            restoreToken();
            restoreToken();
            node = parseCallExpression();
        } else {
            error("only '=' and '(' are allowed here", token);
        }
    } else {
        error("no statement start with this unexpected token", token);
    }
    return node;
}


Parser::ASTNode *Parser::parseDeclareStatement() {
    Parser::ASTNode *node = new ASTNode;
    node->type = VAR_DELCARE_NODE;
    Lexer::Token token = getToken();
    assert(token.value == "var");
    token = getToken();
    assert(token.type == Lexer::ID);
    Parser::ASTNode *leftNode = new ASTNode;
    leftNode->token = token;
    token = getToken();
    assert(token.value == "=");
    Parser::ASTNode *rightNode = new ASTNode;
    token = getToken();
    rightNode->token = token;
    node->child[1] = rightNode;
    return node;
}

Parser::ASTNode *Parser::parseParameterList() {
    return new ASTNode;
}

Parser::ASTNode *Parser::parseAssignStatement() {
    return new ASTNode();
}

Parser::ASTNode *Parser::parseIfStatement() {
    return new ASTNode();
}

Parser::ASTNode *Parser::parseWhileStatement() {
    return new ASTNode();
}

Parser::ASTNode *Parser::parseReturnStatement(){
    return new ASTNode();
}

Parser::ASTNode *Parser::parseCallExpression() {
    return new ASTNode();
}

Parser::ASTNode *Parser::parseArgumentList() {
    return new ASTNode();
}

Parser::ASTNode *Parser::parseExpressionList() {
    return new ASTNode();
}

Parser::ASTNode *Parser::parseExpression() {
    return new ASTNode();
}

Parser::ASTNode *Parser::parseRelationalOperator() {
    return new ASTNode();
}

Parser::ASTNode *Parser::parseAdditiveExpression() {
    return new ASTNode();

}

Parser::ASTNode *Parser::parseTerm() {
    ASTNode *node = parseFactor();
    Lexer::Token token = getToken();
    while (token.value == "*" || token.value == "/") {
        ASTNode *parentNode = new ASTNode;
        parentNode->type = BINARY_OPERATOR_NODE;
        parentNode->token = token;
        parentNode->child[0] = node;
        node = parentNode;
        parentNode->child[1] = parseFactor();
        token = getToken();
    }
    restoreToken();
    return node;
}

Parser::ASTNode *Parser::parseFactor() {
    ASTNode *node = nullptr;
    Lexer::Token token = getToken();
    if (token.value == "(") {
        node = parseExpression();
        token = getToken();
        if(token.value != ")") {
            error("expect ) but get ", token);
        }
    } else if (token.type == Lexer::INT) {
        node = new ASTNode;
        node->token = token;
        node->type = INT_NODE;
    } else if (token.type == Lexer::REAL) {
        node = new ASTNode;
        node->token = token;
        node->type = REAL_NODE;
    } else if (token.type == Lexer::STRING) {
        node = new ASTNode;
        node->token = token;
        node->type = STRING_NODE;
    } else if (token.type == Lexer::CHAR) {
        node = new ASTNode;
        node->token = token;
        node->type = CHAR_NODE;
    } else if (token.value == "true" || token.value == "false") {
        node = new ASTNode;
        node->token = token;
        node->type = BOOL_NODE;
    } else if (token.type == Lexer::ID) {
        node = new ASTNode;
        node->token = token;
        node->type = VAR_NODE;
        token = getToken();
        if (token.value == "(") {
            restoreToken();
            restoreToken();
            node = parseCallExpression();
        } else {
            restoreToken();
        }
    } else {
        error("unexpect token when parse factor ", token);
    }
    return node;
}