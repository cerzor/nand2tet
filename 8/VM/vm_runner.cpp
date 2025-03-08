#include <iostream>
#include <string>
#include <fstream>
#include "Parser.h"
#include "CodeWriter.h"
#include "Constants.h"

using namespace std;
int main(int argc, char* argv[]) { 
    if(argc > 3){
        cout << "Too many arguements." << endl;
        return 1;	
    } else if(argc <= 1) {
        cout << "Missing arguements." << endl;
        return 1;
    }

    int tempBooleanIndex = 0;
    string currentFunction = "";
     
    Parser parser(argv[1]);
    CodeWriter writer(string(argv[1]).substr(0, string(argv[1]).find(".vm")));
    int returnCount = 0;
    if (argc >=3) {
        writer.writeBootstrap(argv[2]);
    }
    while (parser.hasMoreCommands()) {
        parser.advance();
        string cmd = parser.commandType();
        if (cmd == "C_PUSH" || cmd == "C_POP") {
          cout << writer.writePushPop(cmd, parser.arg1(), to_string(parser.arg2()), currentFunction);
        } else if (cmd == "C_ARITHMETIC"){
          cout << writer.writeArithmetic(parser.arg1(), tempBooleanIndex);
        } else if (cmd == "C_LABEL") {
            cout << writer.writeLabel(parser.arg1());
        } else if (cmd == "C_IF") {
            cout << writer.writeIf(parser.arg1());
        } else if (cmd == "C_GOTO") {
            cout << writer.writeGoto(parser.arg1());
        } else if (cmd == "C_FUNCTION") {
            currentFunction = parser.arg1();
            int arg2 = parser.arg2();
            cout << writer.writeFunction(currentFunction, arg2);
        } else if (cmd == "C_CALL") {
            cout << writer.writeCall(parser.arg1(), parser.arg2(), returnCount, currentFunction);
        } else if (cmd == "C_RETURN") {
            cout << writer.writeReturn();
        }
    }

    return 0;
}

