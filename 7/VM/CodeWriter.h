#include <unordered_map>
#include <fstream>
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
    	{"add", "//add\n@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nM=M+D\n@SP\nM=M+1"},
      {"sub", "//sub\n@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nM=M-D\n@SP\nM=M+1"},
      {"neg", "//neg\n@SP\nAM=M-1\nM=-M"}
    };
    
public:
    CodeWriter(string filePath) {
        output.open(filePath);
      
    };

    string writeArithmetic(const string& command){
      if (arithmeticCommands.count(command)) {
        return arithmeticCommands[command];
      } else {
        cout << "Unknown arithmetic command\n";
      }
      return "Unknown writeArithmetic\n";
    }

    string writePushPop(const string& command, const string& segment, int& index){
      string comment = "//" + command + " " + segment + " " + to_string(index) + "\n";
      if(command == "C_PUSH"){
        if(segment == "constant"){
          return comment + "@" + to_string(index) + "\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
        } else {
          return comment + "@" + segmentMap[segment] + "\nD=M\n@" + to_string(index) + "\nA=D+A\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
        }
      } else if(command == "C_POP"){
        return  comment +"@" + segmentMap[segment] + "\nD=M\n@" + to_string(index) + "\nD=D+A\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n";
      }
      return "Unknown writePushPop\n";
    }


    void close(){
      output.close();
    }

};
