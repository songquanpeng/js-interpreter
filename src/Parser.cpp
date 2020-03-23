#include "Parser.h"
#include <iostream>
#include <string>

using namespace std;

Parser::Parser(string &filename) {
    lexer.openFile(filename);
    root = nullptr;
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
    return leftTokenBuffer.back();
}

// Restore current token and load last token.
Lexer::Token Parser::restoreToken() {
    auto token = leftTokenBuffer.back();
    leftTokenBuffer.pop_back();
    rightTokenBuffer.push_front(token);
    return leftTokenBuffer.back();
}

void Parser::error(string message, Lexer::Token token) {
    cerr << "[Parser] [Error]: " << message << " | [Token]: " << lexer.tokenToString(token) << endl;
    exit(-1);
}


void Parser::printAST() {
    // TODO
}

Parser::ASTNode *Parser::getAST() {
    return root;
}

void Parser::parseProgrem() {
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
        node = parseReturnStatement();
    } else if (token.type == Lexer::ID) {
        token = getToken();
        if (token.value == "=") {
            restoreToken();
            restoreToken();
            node = parseAssignStatement();
        } else if (token.value == "(") {
            restoreToken();
            restoreToken();
            node = parseCallStatement();
        } else {
            error("only '=' and '(' are allowed here", token);
        }
    } else {
        error("no statement start with this unexpected token", token);
    }
    return node;
}


Parser::ASTNode *Parser::parseDeclareStatement() {
    return new ASTNode();
}

Parser::ASTNode *Parser::parseParameterList() {
    return new ASTNode();

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

Parser::ASTNode *Parser::parseCallStatement() {
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
    return new ASTNode();

}

Parser::ASTNode *Parser::parseFactor() {
    return new ASTNode();

}