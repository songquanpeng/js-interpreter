#include "Parser.h"
#include <iostream>
#include <string>
#include <cassert>
#include <utility>

using namespace std;

Parser::Parser() {
    root = nullptr;
}

void Parser::parseFile(std::string &filename) {
    lexer.openFile(filename);
    parseProgram();
}

Parser::ASTNode *Parser::parseInput(string input) {
    lexer.tokenizeInput(std::move(input));
    return parseStatement();
}

// Load next token.
Lexer::Token Parser::getToken() {
    if (rightTokenBuffer.empty()) {
        auto token = lexer.nextToken();
        leftTokenBuffer.push_back(token);
        if (leftTokenBuffer.size() > 10) leftTokenBuffer.pop_front();
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

void Parser::error(const string &message, const Lexer::Token &token) {
    cerr << "[Parser] [Error]: " << message << " | [Token]: " << lexer.tokenToString(token) << endl;
    exit(-1);
}

void Parser::log(const string &message, const Lexer::Token &token) {
    if (!debug) return;
    cout << "[Parser] [Log]: " << message << " | [Token]: " << lexer.tokenToString(token) << endl;
}

void Parser::printAST() {
    printASTHelper(root, 0);
}

void Parser::printASTHelper(Parser::ASTNode *node, int depth) {

}

void Parser::setDebugMode(bool enable) {
    debug = enable;
    lexer.setDebugMode(enable);
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
    while (token.value == "var" || token.value == "const" ||
           token.value == "let" || token.value == "function" ||
           token.type == Lexer::ID || token.value == "if" ||
           token.value == "while" || token.value == "return" ||
           token.value == "for") {
        restoreToken();
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
    } else if (token.value == "for") {
        restoreToken();
        node = parseForStatement();
    } else if (token.value == "return") {
        restoreToken();
        node = parseReturnStatement();
    } else if (token.value == "var" || token.value == "let" || token.value == "const") {
        restoreToken();
        node = parseDeclareStatement();
    } else if (token.value == "function") {
        restoreToken();
        node = parseFunction();
    } else if (token.type == Lexer::ID) {
        token = getToken();
        if (token.value == "=" || token.value == "[") {
            restoreToken();
            restoreToken();
            node = parseAssignStatement();
        } else if (token.value == "(") {
            restoreToken();
            restoreToken();
            node = parseCallExpression();
            token = getToken();
            if (token.value != ";") restoreToken();
        } else {
            // Four cases:
            // ID EOL
            // ID ; EOL
            // ID ;
            // ID
            restoreToken(); // Return the token after the ID token.
            restoreToken(); // Return the ID token.
            token = getToken(); // Get the ID token back.
            Lexer::Token nextToken = getToken(); // Discard token if its EOL or `;`.
            if (nextToken.type != Lexer::END_OF_LINE && nextToken.value != ";") {
                restoreToken();
            } else {
                nextToken = getToken();
                if (nextToken.type != Lexer::END_OF_LINE) restoreToken();
            }
            node = new ASTNode;
            node->type = VAR_NODE;
            node->token = token;
            log("this statement only has one ID", token);
        }
    } else {
        error("no statement start with this unexpected token", token);
    }
    return node;
}

Parser::ASTNode *Parser::parseDeclareStatement() {
    auto *node = new ASTNode;
    node->type = VAR_DECLARE_NODE;
    Lexer::Token token = getToken();
    if(token.value == ";") {
        node->type = NONE;
        return node;
    }
    assert(token.value == "var" || token.value == "let" || token.value == "const");
    token = getToken();
    assert(token.type == Lexer::ID);
    node->token = token;
    token = getToken();
    assert(token.value == "=");
    node->child[0] = parseExpression();
    token = getToken();
    if (token.value != ";") restoreToken();
    return node;
}

Parser::ASTNode *Parser::parseParameterList() {
    auto *node = new ASTNode;
    auto *parent = node;
    Lexer::Token token = getToken();
    while (token.type == Lexer::ID) {
        node->token = token;
        node->type = ARGUMENT_NODE;
        token = getToken();
        if (token.value == ",") {
            token = getToken();
            node->next = new ASTNode;
            node = node->next;
            continue;
        } else {
            assert(token.value == ")");
            restoreToken();
            break;
        }
    }
    return parent;
}

Parser::ASTNode *Parser::parseAssignStatement() {
    auto *node = new ASTNode;
    node->type = VAR_ASSIGN_NODE;
    Lexer::Token token = getToken();
    assert(token.type == Lexer::ID);
    node->token = token;
    token = getToken();
    if(token.value == "[") {
        node->child[1] = parseExpression();
        token = getToken();
        assert(token.value == "]");
        token = getToken();
    }
    assert(token.value == "=");
    node->child[0] = parseExpression();
    token = getToken();
    if (token.value != ";") restoreToken();
    return node;
}

Parser::ASTNode *Parser::parseIfStatement() {
    auto *node = new ASTNode;
    node->type = IF_NODE;
    Lexer::Token token = getToken();
    assert(token.value == "if");
    token = getToken();
    assert(token.value == "(");
    node->child[0] = parseExpression();
    token = getToken();
    assert(token.value == ")");
    token = getToken();
    assert(token.value == "{");
    node->child[1] = parseStatementList();
    token = getToken();
    assert(token.value == "}");
    token = getToken();
    if (token.value == "else") {
        token = getToken();
        assert(token.value == "{");
        node->child[2] = parseStatementList();
        token = getToken();
        assert(token.value == "}");
    } else {
        restoreToken();
    }
    return node;
}

Parser::ASTNode *Parser::parseWhileStatement() {
    auto *node = new ASTNode;
    node->type = WHILE_NODE;
    Lexer::Token token = getToken();
    assert(token.value == "while");
    token = getToken();
    assert(token.value == "(");
    node->child[0] = parseExpression();
    token = getToken();
    assert(token.value == ")");
    token = getToken();
    assert(token.value == "{");
    node->child[1] = parseStatementList();
    token = getToken();
    assert(token.value == "}");
    return node;
}

Parser::ASTNode *Parser::parseForStatement() {
    auto *node = new ASTNode;
    node->type = FOR_NODE;
    Lexer::Token token = getToken();
    assert(token.value == "for");
    token = getToken();
    assert(token.value == "(");
    node->child[0] = parseDeclareStatement();
    node->child[1] = parseExpression();
    token = getToken();
    assert(token.value == ";");
    node->child[2] = parseAssignStatement();
    token = getToken();
    assert(token.value == ")");
    token = getToken();
    assert(token.value == "{");
    node->child[3] = parseStatementList();
    token = getToken();
    assert(token.value == "}");
    return node;
}

Parser::ASTNode *Parser::parseReturnStatement() {
    Lexer::Token token = getToken();
    assert(token.value == "return");
    auto *node = new ASTNode;
    node->type = RETURN_NODE;
    node->child[0] = parseExpression();
    token = getToken();
    if (token.value != ";") restoreToken();
    return node;
}

Parser::ASTNode *Parser::parseCallExpression() {
    auto *node = new ASTNode;
    node->type = FUNCTION_CALL_NODE;
    Lexer::Token token = getToken();
    assert(token.type == Lexer::ID);
    node->token = token;
    token = getToken();
    assert(token.value == "(");
    node->child[0] = parseArgumentList();
    token = getToken();
    assert(token.value == ")");
    return node;
}

Parser::ASTNode *Parser::parseArgumentList() {
    Lexer::Token token = getToken();
    if (token.value == ")") {
        restoreToken();
        return nullptr;
    }
    restoreToken();
    auto *node = parseExpression();
    auto *parent = node;
    token = getToken();
    while (token.value == ",") {
        node->next = parseExpression();
        node = node->next;
        token = getToken();
    }
    restoreToken();
    return parent;
}

Parser::ASTNode *Parser::parseFactorList() {
    auto *node = parseFactor();
    auto *parent = node;
    auto token = getToken();
    while (token.value == ",") {
        node->next = parseFactor();
        node = node->next;
        token = getToken();
    }
    restoreToken();
    return parent;
}

Parser::ASTNode *Parser::parseExpression() {
    Lexer::Token token = getToken();
    restoreToken();
    if (token.value == "[") {
        return parseArrayDeclareExpression();
    }
    auto *node = parseAdditiveExpression();
    token = getToken();
    if (token.value == "<=" || token.value == ">=" || token.value == "==" ||
        token.value == "<" || token.value == ">" || token.value == "!=" ||
        token.value == "&&" || token.value == "||") {
        auto *parent = new ASTNode;
        parent->type = BINARY_OPERATOR_NODE;
        parent->token = token;
        parent->child[0] = node;
        parent->child[1] = parseAdditiveExpression();
        node = parent;
    } else {
        restoreToken();
    }
    auto *result = new ASTNode;
    result->type = EXPRESSION_NODE;
    result->child[0] = node;
    return result;
}

Parser::ASTNode *Parser::parseAdditiveExpression() {
    ASTNode *node = parseTerm();
    Lexer::Token token = getToken();
    while (token.value == "+" || token.value == "-") {
        auto *parent = new ASTNode;
        parent->type = BINARY_OPERATOR_NODE;
        parent->token = token;
        parent->child[0] = node;
        parent->child[1] = parseTerm();
        node = parent;
        token = getToken();
    }
    restoreToken();
    return node;
}

Parser::ASTNode *Parser::parseTerm() {
    ASTNode *node = parseFactor();
    Lexer::Token token = getToken();
    while (token.value == "*" || token.value == "/") {
        auto *parentNode = new ASTNode;
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
    Lexer::Token token = getToken();
    if (token.value == "-") {
        auto *node = new ASTNode;
        node->token = token;
        node->child[0] = parsePositiveFactor();
        node->type = NEGATIVE_NODE;
        return node;
    } else {
        restoreToken();
        return parsePositiveFactor();
    }
}

Parser::ASTNode *Parser::parsePositiveFactor() {
    ASTNode *node = nullptr;
    Lexer::Token token = getToken();
    if (token.value == "(") {
        node = parseExpression();
        token = getToken();
        if (token.value != ")") {
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
        token = getToken();
        if (token.value == "(") {
            restoreToken();
            restoreToken();
            node = parseCallExpression();
        } else if (token.value == "[") {
            restoreToken();
            restoreToken();
            node = parseArrayAccessExpression();
        } else {
            restoreToken();
            restoreToken();
            token = getToken();
            node = new ASTNode;
            node->token = token;
            node->type = VAR_NODE;
        }
    } else {
        error("unexpect token when parse factor ", token);
    }
    return node;
}

Parser::ASTNode *Parser::parseFunction() {
    auto *node = new ASTNode;
    node->type = FUNCTION_DECLARE_NODE;
    Lexer::Token token = getToken();
    assert(token.value == "function");
    token = getToken();
    assert(token.type == Lexer::ID);
    node->token = token;
    token = getToken();
    assert(token.value == "(");
    token = getToken();
    if(token.value != ")") {
        restoreToken();
        node->child[0] = parseParameterList();
        token = getToken();
        assert(token.value == ")");
    }
    token = getToken();
    assert(token.value == "{");
    node->child[1] = parseStatementList();
    token = getToken();
    assert(token.value == "}");
    return node;
}

Parser::ASTNode *Parser::parseArrayDeclareExpression() {
    auto *node = new ASTNode;
    node->type = ARRAY_DECLARE_NODE;
    Lexer::Token token = getToken();
    assert(token.value == "[");
    node->child[0] = parseFactorList();
    token = getToken();
    assert(token.value == "]");
    return node;
}

Parser::ASTNode *Parser::parseArrayAccessExpression() {
    auto *node = new ASTNode;
    node->type = ARRAY_ACCESS_NODE;
    Lexer::Token token = getToken();
    assert(token.type == Lexer::ID);
    node->token = token;
    token = getToken();
    assert(token.value == "[");
    node->child[0] = parseExpression();
    token = getToken();
    assert(token.value == "]");
    return node;
}
