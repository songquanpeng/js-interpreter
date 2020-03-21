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

void Parser::printAST() {

}

Parser::ASTNode* Parser::getAST() {
    return root;
}