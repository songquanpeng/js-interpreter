#include "Lexer.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    if(argc < 2) {
        cerr << "usage: " << argv[0] << " <*.js>" <<endl;
        exit(-1);
    }
    string filename(argv[1]);
    Lexer lexer;
    lexer.openFile(filename);
    Lexer::Token token = lexer.nextToken();
    while (token.type != Lexer::END_OF_FILE) {
        lexer.print(token);
        token = lexer.nextToken();
    }
}