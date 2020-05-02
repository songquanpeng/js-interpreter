#include "Interpreter.h"
#include <iostream>
#include <cstring>

using namespace std;

int main(int argc, char *argv[]) {
    Interpreter interpreter;
    if (argc == 1) {
        interpreter.shell();
    }
    if (argc >=3 && strncmp(argv[2], "-d", 2) == 0) {
        interpreter.setDebugMode(true);
    }
    string filename(argv[1]);
    interpreter.interpretFile(filename);
}
