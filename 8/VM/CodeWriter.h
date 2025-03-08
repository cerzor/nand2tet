#include <unordered_map>
#include <fstream>
#include <vector>
#include <algorithm>
#include "Constants.h"

using namespace std;
class CodeWriter {
private:
    ofstream output;
    int labelCounter = 0;
    unordered_map<string, string> segmentMap = {
      {"local", "LCL"}, 
      {"argument", "ARG"}, 
      {"this", "THIS"}, 
      {"that", "THAT"},
    };
    
    unordered_map<string, string> arithmeticCommands = {
    	{"add", "//add\n@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nM=M+D\n@SP\nM=M+1\n"},
      {"sub", "//sub\n@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nM=M-D\n@SP\nM=M+1\n"},
      {"neg", "//neg\n@SP\nAM=M-1\nM=-M\n@SP\nM=M+1\n"},
      {"eq", "//eq\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=M-D\n@EQUAL_{int}\nD;JEQ\n@SP\nA=M\nM=0\n@CONTINUE_{int}\n0;JMP\n(EQUAL_{int})\n@SP\nA=M\nM=-1\n(CONTINUE_{int})\n@SP\nM=M+1\n"},
      {"gt", "//gt\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=M-D\n@GREATER_{int}\nD;JGT\n@SP\nA=M\nM=0\n@CONTINUE_{int}\n0;JMP\n(GREATER_{int})\n@SP\nA=M\nM=-1\n(CONTINUE_{int})\n@SP\nM=M+1\n"},
      //{"lt", "//lt\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=M-D\n@LESSER_{int}\nD;JLT\n@SP\nA=M\nM=0\n@CONTINUE_{int}\n0;JMP\n(LESSER_{int})\n@SP\nA=M\nM=-1\n(CONTINUE_{int})\n@SP\nM=M+1\n"},
      {"lt", "@SP\nM=M-1\nA=M\nD=M\n@R13\nM=D\n@SP\nM=M-1\nA=M\nD=M\n@R13\nD=D-M\n@LESSER_{int}\nD;JLT\n@SP\nA=M\nM=0\n@CONTINUE_{int}\n0;JMP\n(LESSER_{int})\n@SP\nA=M\nM=-1\n(CONTINUE_{int})\n@SP\nM=M+1\n"},
      {"and", "//and\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=D&M\nM=D\n@SP\nM=M+1\n"},
      {"or", "//or\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=D|M\nM=D\n@SP\nM=M+1\n"},
      {"not", "//not\n@SP\nM=M-1\nA=M\nM=!M\n@SP\nM=M+1\n"}
    };

    vector<string> simpleCommands = {"add", "sub", "neg"};
    string pointer[2] = {"THIS", "THAT"};
    
public:
    CodeWriter(string filePath) {
        output.open(filePath);
      
    };

    string replaceIntPlaceholder(const string& templateStr, int index) {
      string result = templateStr;
      string placeholder = "{int}";
      string replacement = to_string(index);  // Convert int to string

      size_t pos = result.find(placeholder);
      while (pos != string::npos) {
        result.replace(pos, placeholder.length(), replacement);
        pos = result.find(placeholder, pos + replacement.length());
      }

      return result;
    }

    void writeBootstrap(const string& fucntionName) {
      cout << "@256\n"
        "D=A\n"
        "@SP\n"
        "M=D\n"
        "@after_bootstrap\n"
        "D=A\n"
        "@SP\n"
        "A=M\n"
        "M=D\n"
        "@SP\n"
        "M=M+1\n"
        "@LCL\n"
        "D=M\n"
        "@SP\n"
        "A=M\n"
        "M=D\n"
        "@SP\n"
        "M=M+1\n"
        "@ARG\n"
        "D=M\n"
        "@SP\n"
        "A=M\n"
        "M=D\n"
        "@SP\n"
        "M=M+1\n"
        "@THIS\n"
        "D=M\n"
        "@SP\n"
        "A=M\n"
        "M=D\n"
        "@SP\n"
        "M=M+1\n"
        "@THAT\n"
        "D=M\n"
        "@SP\n"
        "A=M\n"
        "M=D\n"
        "@SP\n"
        "M=M+1\n"
        "@5\n"
        "D=A\n"
        "@0\n"
        "D=D+A\n"
        "@SP\n"
        "D=M-D\n"
        "@ARG\n"
        "M=D\n"
        "@SP\n"
        "D=M\n"
        "@LCL\n"
        "M=D\n"
        "@" + fucntionName + "\n"
        "0;JMP\n"
        "(after_bootstrap)\n";
    }

    string writeArithmetic(const string& command, int& index){
      if (find(simpleCommands.begin(), simpleCommands.end(), command) != simpleCommands.end()) {
        return arithmeticCommands[command];
      }
      if (arithmeticCommands.count(command)) {
        string translate = replaceIntPlaceholder(arithmeticCommands[command], index);
        index++;
        return translate;
      }
      return "Unknown writeArithmetic\n";
    }

    string writePushPop(const string& commandType, const string& command, const string& segment, const string& functionName){
      string comment = "//" + commandType + " " + command + " " + segment + "\n";
      string staticLabel = functionName.empty() ? "Static" : (functionName.find('.') == string::npos ? functionName : functionName.substr(0, functionName.find('.')));
      if(commandType == "C_PUSH"){
        if(command == "constant"){
          return comment + "@" + segment + "\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
        }
        if (command == "temp") {
          return comment + "@5\nD=A\n@" + segment  + "\nA=A+D\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
        }
        if (command == "pointer") {
          return comment +  "@" + pointer[stoi(segment)] + "\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
        }
        if (command == "static") {
          return comment + "@" + staticLabel + "." + segment + "\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
        }
        return comment + "@" + segmentMap[command] + "\nD=M\n@" + segment + "\nA=D+A\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";

      }

      if(commandType == "C_POP"){
        if (command == "temp") {
          return  comment +"@5\nD=A\n@" + segment + "\nD=D+A\n@R13\nM=D\n@SP\nM=M-1\nA=M\nD=M\n@R13\nA=M\nM=D\n";
        }
        if (command == "pointer") {
          return comment +  "@SP\nAM=M-1\nD=M\n@" + pointer[stoi(segment)] + "\nM=D\n";
        }
        if (command == "static") {
          return comment + "@SP\nAM=M-1\nD=M\n@" + staticLabel + "." + segment + "\nM=D\n";
        }
        return  comment +"@" + segmentMap[command] + "\nD=M\n@" + segment + "\nD=D+A\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n";
      }
      return "Unknown writePushPop\n";
    }

    string writeLabel(const string& label) {
      string comment = "//label-" + label + "\n";
      return comment + "(" + label + ")\n";
    }

   string writeGoto(const string& label) {
      string comment = "//goto-" + label + "\n";
      return comment + "@" + label + "\n0;JMP\n";
    }

   string writeIf(const string& label) {
      string comment = "//ifgoto-" + label + "\n";
      return comment + "@SP\nAM=M-1\nD=M\n@" + label + "\nD;JNE\n";
    }

  string writeFunction(const string& function, int nVars) {
      string commands = "//write function " + function + "\n(" + function + ")\n";
      for (int i = 0; i < nVars; i++) {
        commands += "@0\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
      }
      return commands;
    }

  string writeCall(const string& function, int nArgs, int& returnCount, const string& caller) {
      string returnLabel = (caller.empty() ? function : caller) + "$return." + to_string(returnCount++);
      string commands = "//call(" + function + ")" + to_string(nArgs) + "\n";
      commands += "@" + returnLabel + "\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
      commands += "@LCL\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
      commands += "@ARG\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
      commands += "@THIS\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
      commands += "@THAT\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
      commands += "@5\nD=A\n@" + to_string(nArgs) + "\nD=D+A\n@SP\nD=M-D\n@ARG\nM=D\n";
      commands += "@SP\nD=M\n@LCL\nM=D\n";
      commands += "@" + function + "\n0;JMP\n";
      commands += "(" + returnLabel + ")\n";
      return commands;
    }

  string writeReturn() {
      string commands = "//return\n";
      commands += "@LCL\nD=M\n@R13\nM=D\n";
      commands += "@R13\nD=M\n@5\nA=D-A\nD=M\n@R14\nM=D\n";
      commands += "@SP\nAM=M-1\nD=M\n@ARG\nA=M\nM=D\n";
      commands += "@ARG\nD=M+1\n@SP\nM=D\n";
      commands += "@R13\nA=M-1\nD=M\n@THAT\nM=D\n";
      commands += "@R13\nD=M\n@2\nA=D-A\nD=M\n@THIS\nM=D\n";
      commands += "@R13\nD=M\n@3\nA=D-A\nD=M\n@ARG\nM=D\n";
      commands += "@R13\nD=M\n@4\nA=D-A\nD=M\n@LCL\nM=D\n";
      commands += "@R14\nA=M\n0;JMP\n";
      return commands;
    }

    void close(){
      output.close();
    }

};
