#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
using namespace std;

class Parser {
  private:
    ifstream input;
    string currentLine;
    vector<string> tokens;
    const char* whitespace = " \t\n\r\f\v";
  public:
    string trim(const string& s){
      string result = s;
      result.erase(0, result.find_first_not_of(whitespace));
      result.erase(result.find_last_not_of(whitespace) + 1);
      return result;
    }

    Parser(string filePath) {
        input.open(filePath);
        if (!input.is_open()) throw runtime_error("File not found");
    }
    bool hasMoreCommands() { 
      return !input.eof(); 
    }
    
    void advance(){
        tokens.clear();
        getline(input, currentLine);
        currentLine = trim(currentLine);
        if (currentLine.empty() || currentLine.substr(0, 2) == "//") return;
        istringstream lineStream(currentLine);
        string word;
        while (lineStream >> word) tokens.push_back(word);
    }

    string commandType() {
        if (tokens.empty()) return "";
        if (tokens[0] == "push") return "C_PUSH";
        if (tokens[0] == "pop") return "C_POP";
        if (tokens[0] == "label") return "C_LABEL";
        if (tokens[0] == "goto") return "C_GOTO";
        if (tokens[0] == "if-goto") return "C_IF";
        if (tokens[0] == "function") return "C_FUNCTION";
        if (tokens[0] == "return") return "C_RETURN";
        if (tokens[0] == "call") return "C_CALL";
        
        return "C_ARITHMETIC"; // add, sub, etc.
    }

    string arg1() { return (commandType() == "C_ARITHMETIC") ? tokens[0] : tokens[1]; }
    int arg2() { return (commandType() == "C_PUSH" || commandType() == "C_POP") ? stoi(tokens[2]) : 0; }
};
