#ifndef _LEXER_H
#define _LEXER_H

#include <set>
#include <string>
#include <fstream>

class Lexer {
public:
    enum TokenType {
        ID,
        KEYWORD,
        REAL,
        INT,
        STRING,
        CHAR,
        SYMBOL,
        NONE,
        ERROR,
        END_OF_FILE, // EOF
        END_OF_LINE // Used in interactive mode
    };
struct Token {
        TokenType type; // Token's type.
        std::string value; // Token's value.
        unsigned rowNumber; // The row where the token is located.
    };

private:
    std::string rowBuffer; // Store current row.
    unsigned rowBufferPos; // Pointer position for row buffer.
    unsigned rowNumber; // Current row.
    std::ifstream file; // File input stream object.
    std::string input;
    char nextChar(); // Get next char.
    void rollBack(); // Roll back line buffer (rowBufferPos--).
    void initKeywordsAndSymbols();
    bool isKeyword(std::string const& str);
    bool isSymbol(std::string const& str);
    bool debug = false;

public:
    std::set<std::string> keywords;
    std::set<std::string> symbols;
    Lexer(); // Constructor function.
    void openFile(std::string const& filename); // Open source file.
    void closeFile(); // Close source file.
    void tokenizeInput(std::string i);
    Token nextToken(); // Get next token.
    void resetRow(); // Reset rowNumber to 0.
    void print(const Token& token); // Print token.
    std::string tokenToString(const Token& token); // Convert token to string.
    void error(const std::string& message, char currentChar);
    void setDebugMode(bool enable);
};

#endif