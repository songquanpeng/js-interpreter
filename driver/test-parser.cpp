#include "Parser.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    if(argc < 2) {
        cerr << "usage: " << argv[0] << " <*.js>" <<endl;
        exit(-1);
    }
    string filename(argv[1]);
    Parser parser(filename);
}