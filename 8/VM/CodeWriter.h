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
      {"lt", "//lt\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=M-D\n@LESSER_{int}\nD;JLT\n@SP\nA=M\nM=0\n@CONTINUE_{int}\n0;JMP\n(LESSER_{int})\n@SP\nA=M\nM=-1\n(CONTINUE_{int})\n@SP\nM=M+1\n"},
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

    string writeArithmetic(const string& command, int& index){
      if (find(simpleCommands.begin(), simpleCommands.end(), command) != simpleCommands.end()) {
        return arithmeticCommands[command];
      }
      if (arithmeticCommands.count(command)) {
        return replaceIntPlaceholder(arithmeticCommands[command], index++);
      }
      return "Unknown writeArithmetic\n";
    }

    string writePushPop(const string& commandType, const string& command, const string& segment, int& index){
      string comment = "//" + commandType + " " + command + " " + segment + "\n";
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
          return comment + "@Static." + segment + "\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
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
          return comment + "@SP\nAM=M-1\nD=M\n@Static." + segment + "\nM=D\n";
        }
        return  comment +"@" + segmentMap[command] + "\nD=M\n@" + segment + "\nD=D+A\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n";
      }
      return "Unknown writePushPop\n";
    }

    string writeLabel(const string& label) {
        return "(" + label + ")\n";
    }

   string writeGoto(const string& label) {

    }

   string writeIf(const string& label) {
        return "@SP\nAM=M-1\nD=M\n@" + label + "\nD;JNE\n";
    }

  string writeFunction(const string& function, int nVars[]) {

    }

  string writeCall(const string& function, int nArgs[]) {

    }

  string writeReturn() {

    }

    void close(){
      output.close();
    }

};
