#include "Lexer.h"
#include <iostream>
#include <string>
#include <utility>

using namespace std;

Lexer::Lexer() {
    rowNumber = 0;
    rowBufferPos = 0;
    initKeywordsAndSymbols();
}

void Lexer::tokenizeInput(string i) {
    input = std::move(i);
    rowNumber++;
    rowBuffer = input;
    rowBuffer += '\n';
    rowBufferPos = 0;
}

void Lexer::openFile(const string &filename) {
    file.open(filename);
    if (file.fail()) {
        cerr << "file " << filename << " cannot not be open." << endl;
        exit(-1);
    }
}

void Lexer::closeFile() {
    file.close();
}

void Lexer::initKeywordsAndSymbols() {
    keywords.insert("function");
    keywords.insert("var");
    keywords.insert("let");
    keywords.insert("const");
    keywords.insert("true");
    keywords.insert("false");
    keywords.insert("this");
    keywords.insert("if");
    keywords.insert("else");
    keywords.insert("while");
    keywords.insert("return");
    keywords.insert("undefined");
    keywords.insert("null");
    keywords.insert("for");
    keywords.insert("break");
    keywords.insert("continue");
    keywords.insert("class");

    symbols.insert("{");
    symbols.insert("}");
    symbols.insert("(");
    symbols.insert(")");
    symbols.insert("[");
    symbols.insert("]");
    symbols.insert(".");
    symbols.insert(",");
    symbols.insert(";");
    symbols.insert("+");
    symbols.insert("-");
    symbols.insert("*");
    symbols.insert("/");
    symbols.insert("%");
    symbols.insert("&");
    symbols.insert("|");
    symbols.insert("!");
    symbols.insert("<");
    symbols.insert(">");
    symbols.insert("=");
    symbols.insert(">=");
    symbols.insert("<=");
    symbols.insert("==");
    symbols.insert("!=");
    symbols.insert("&&");
    symbols.insert("||");
}

void Lexer::resetRow() {
    rowNumber = 0;
}

bool Lexer::isKeyword(const string &str) {
    return keywords.find(str) != keywords.end();
}

bool Lexer::isSymbol(const string &str) {
    return symbols.find(str) != symbols.end();
}

char Lexer::nextChar() {
    if (rowBufferPos >= rowBuffer.size()) {
        rowNumber++;
        if (input.empty()) { // Tokenize file.
            getline(file, rowBuffer);
            rowBuffer += '\n';
            if (!file.fail()) {
                rowBufferPos = 0;
                return rowBuffer[rowBufferPos++];
            } else {
                return EOF;
            }
        } else { // Tokenize input string.
            return '\0'; // Means end of line.
        }
    } else {
        return rowBuffer[rowBufferPos++];
    }
}

void Lexer::rollBack() {
    rowBufferPos--;
}

Lexer::Token Lexer::nextToken() {
    char currentChar = nextChar();
    Token token;
    while (true) {
        // Check EOF.
        if (currentChar == EOF) {
            token.type = END_OF_FILE;
            break;
        }
        // Check dnd of line.
        if (currentChar == '\0') {
            token.type = END_OF_LINE;
            break;
        }
        // Skip all blank characters.
        if (isspace(currentChar)) {
            currentChar = nextChar();
            continue;
        }
        // Construct identifier and keyword token.
        if (isalpha(currentChar) || currentChar == '_') {
            token.type = ID;
            token.value += currentChar;
            currentChar = nextChar();
            while (isalpha(currentChar) || currentChar == '_' || isdigit(currentChar)) {
                token.value += currentChar;
                currentChar = nextChar();
            }
            rollBack();
            if (isKeyword(token.value)) token.type = KEYWORD;
            // if(token.value == "true" || token.value == "false") token.type = BOOL;
            break;
        }
        // Construct int and real token.
        if (isdigit(currentChar)) {
            token.value += currentChar;
            currentChar = nextChar();
            while (true) {
                if (isdigit(currentChar)) {
                    token.value += currentChar;
                    currentChar = nextChar();
                    continue;
                }
                if (currentChar == '.') {
                    if (token.type != REAL) {
                        token.value += currentChar;
                        token.type = REAL;
                        currentChar = nextChar();
                        continue;
                    } else {
                        break;
                    }
                }
                break;
            }
            rollBack();
            if (token.type != REAL) token.type = INT;
            break;
        }
        // Construct string token.
        if (currentChar == '"') {
            char lastChar = '\0';
            currentChar = nextChar();
            token.type = STRING;
            while (true) {
                // The end of string.
                if (lastChar != '\\' && currentChar == '\"') {
                    if (lastChar != '\0') token.value += lastChar;
                    break;
                } else {
                    if (lastChar != '\0') token.value += lastChar;
                    lastChar = currentChar;
                    currentChar = nextChar();
                    continue;
                }
            }
            break;
        }
        // Construct char token.
        if (currentChar == '\'') {
            currentChar = nextChar();
            if (currentChar != '\\') {
                token.value = currentChar;
                currentChar = nextChar();
            } else {
                token.value = currentChar;
                currentChar = nextChar();
                token.value += currentChar;
                currentChar = nextChar();
            }
            if (currentChar != '\'') {
                error("too much characters in single quote", currentChar);
            }
            token.type = CHAR;
            break;
        }
        // Construct symbol token.
        std::string str(1, currentChar);
        if (Lexer::isSymbol(str)) {
            token.type = Lexer::SYMBOL;
            token.value += str;
            if (currentChar == '=' || currentChar == '>' || currentChar == '<' || currentChar == '!') {
                currentChar = nextChar();
                if (currentChar == '=') {
                    token.value += '=';
                } else {
                    rollBack();
                }
            } else if (currentChar == '&') {
                currentChar = nextChar();
                if (currentChar == '&') {
                    token.value += '&';
                } else {
                    rollBack();
                }
            } else if (currentChar == '|') {
                currentChar = nextChar();
                if (currentChar == '|') {
                    token.value += '|';
                } else {
                    rollBack();
                }
            }
            break;
        }
        error(&"unexpected character "[currentChar], currentChar);
    }
    token.rowNumber = rowNumber;
    return token;
}

void Lexer::print(const Lexer::Token &token) {
    cout << tokenToString(token) << endl;
}

string Lexer::tokenToString(const Lexer::Token &token) {
    std::string tokenType;
    std::string tokenValue = token.value;
    switch (token.type) {
        case Lexer::ID:
            tokenType = "ID";
            break;
        case Lexer::KEYWORD:
            tokenType = "KEYWORD";
            break;
        case Lexer::REAL:
            tokenType = "REAL";
            break;
        case Lexer::INT:
            tokenType = "INT";
            break;
        case Lexer::STRING:
            tokenType = "STRING";
            tokenValue = "\"" + tokenValue + "\"";
            break;
        case Lexer::CHAR:
            tokenType = "CHAR";
            tokenValue = "'" + tokenValue + "'";
            break;
        case Lexer::SYMBOL:
            tokenType = "SYMBOL";
            tokenValue = "\"" + tokenValue + "\"";
            break;
        case Lexer::NONE:
            tokenType = "NONE";
            break;
        case Lexer::ERROR:
            tokenType = "ERROR";
            break;
        case Lexer::END_OF_FILE:
            tokenType = "END_OF_FILE";
            break;
        default:
            error(&"unexpected token type: "[token.type], '\0');
    }
    return "<" + tokenType + ", " + tokenValue + ", " + to_string(token.rowNumber) + ">";
}


void Lexer::error(const std::string &message, char currentChar) {
    if (currentChar != '\0') {
        cerr << "[Lexer] [Error]: " << message << " when process character '" << currentChar << "' at row " << rowNumber
             << " col " << rowBufferPos << "." << endl;
    } else {
        cerr << "[Lexer] [Error]: " << message << "." << endl;
    }
    if (debug) exit(-1);
}

void Lexer::setDebugMode(bool enable) {
    debug = enable;
}

