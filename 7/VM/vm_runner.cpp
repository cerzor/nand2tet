#include <iostream>
#include <string>
#include <fstream>
#include "Parser.h"
#include "CodeWriter.h"

using namespace std;
int main(int argc, char* argv[]) { 
    const int LCL = 1;
    const int ARG = 2;
    const int THIS = 3;
    const int THAT = 4;
    const int TEMP_LOW = 5;
    const int TEMP_HIGH = 12;
    const int RESERVED_LOW = 13;
    const int RESERVED_HIGH = 15; //where R15 ends
    const int STATIC_LOW = 16;
    const int STATIC_HIGH = 255;
    const int STACK_START = 256;

    if(argc > 2){ 
        cout << "Too many arguements." << endl;
        return 1;	
    } else if(argc <= 1) {
        cout << "Missing arguements." << endl;
        return 1;
    }

    int index = TEMP_LOW;
    Parser parser(argv[1]);
    CodeWriter writer(string(argv[1]).substr(0, string(argv[1]).find(".vm")));
    while (parser.hasMoreCommands()) {
        parser.advance();
        string cmd = parser.commandType();
        if (cmd == "C_PUSH" || cmd == "C_POP") cout << writer.writePushPop(parser.arg1(), parser.arg2(), index);
        else if (cmd == "C_ARITHMETIC") cout << writer.writeArithmetic(parser.arg1());
    }

    return 0;
}

