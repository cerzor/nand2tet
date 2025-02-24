#include <iostream>
#include <string>
#include <fstream>
#include "Parser.h"
#include "CodeWriter.h"
#include "Constants.h"

using namespace std;
int main(int argc, char* argv[]) { 
    if(argc > 2){ 
        cout << "Too many arguements." << endl;
        return 1;	
    } else if(argc <= 1) {
        cout << "Missing arguements." << endl;
        return 1;
    }

    int temp_index = TEMP_LOW;
     
    Parser parser(argv[1]);
    CodeWriter writer(string(argv[1]).substr(0, string(argv[1]).find(".vm")));
    while (parser.hasMoreCommands()) {
        parser.advance();
        string cmd = parser.commandType();
        if (cmd == "C_PUSH" || cmd == "C_POP") {
          int arg2 = parser.arg2();
          cout << writer.writePushPop(parser.arg1(), to_string(arg2), temp_index);
        } else if (cmd == "C_ARITHMETIC"){
          cout << writer.writeArithmetic(parser.arg1());
        }
    }

    return 0;
}

