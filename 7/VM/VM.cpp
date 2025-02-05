#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

//some constant values
const char* whitespace = " \t\n\r\f\v";
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

inline string trim(const string& s, const char* t = whitespace)
{
    string result = s;
    result.erase(0, result.find_first_not_of(t));
    result.erase(result.find_last_not_of(t) + 1);
    return result;
}

string replaceVariables(const string &input, const vector<string> &replacements, const std::vector<int> &intReplacements) {
	string temp = input;
	string findString = "{string}";
	size_t size_findString = findString.size();
	size_t pos = temp.find(findString);
	size_t replacementIndex = 0;

	while (pos != string::npos && replacementIndex < replacements.size()) {
		temp.replace(pos, size_findString, replacements[replacementIndex]);
		pos = temp.find(findString, pos + replacements[replacementIndex].size());
		replacementIndex++;
	}

    std::string findInt = "{int}";
    size_t findIntSize = findInt.size();
    pos = temp.find(findInt);
    size_t intIndex = 0;

    while (pos != std::string::npos && intIndex < intReplacements.size()) {
        temp.replace(pos, findIntSize, std::to_string(intReplacements[intIndex]));
        pos = temp.find(findInt, pos + std::to_string(intReplacements[intIndex]).size());
        intIndex++;
    }

	return temp;
}

string push(string &segment, string &val) {
	vector<string> replacements;
	replacements.push_back(val);
	replacements.push_back(segment);
	replacements.push_back(segment);
	return replaceVariables("//push {string} {string}\n@{string}\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1", replacements, vector<int>());
}

string pop(int &constant){
	//stub
	return "//pop {string}";
}

string arithmetic(string &str, array<string, 3> &INTEGER_COMMANDS){
	string command;
	if (str == INTEGER_COMMANDS[0]) {
		command = "//add\n@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nD=M+D\n@SP\nA=M\nM=D\n@SP\nM=M+1\n@SP\nM=M+1";
	} else if (str == INTEGER_COMMANDS[1]) {
		command = "//subtract\n@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nD=M-D\n@SP\nA=M\nM=D\n@SP\nM=M+1\n@SP\nM=M+1";
	} else if (str == INTEGER_COMMANDS[2]) {
		command = "//negate variable\n@SP\nA=M\nM=-M\n@SP\nM=M+1";
	}

	return command;
}

string boolean(string &str, array<string, 6> &BOOLEAN_COMMANDS, int &stepCounter){
	string command;
	vector<int> replacements;
	replacements.push_back(stepCounter);
	replacements.push_back(stepCounter);
	replacements.push_back(stepCounter);
	replacements.push_back(stepCounter);

	if (str == BOOLEAN_COMMANDS[0]) {
		command = replaceVariables("//eq\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=M-D\n@EQUAL_{int}\nD;JEQ\n@SP\nA=M\nM=0\n@CONTINUE_{int}\n0;JMP\n(EQUAL_{int})\n@SP\nA=M\nM=-1\n(CONTINUE_{int})\n@SP\nM=M+1", vector<string>(), replacements);
	} else if (str == BOOLEAN_COMMANDS[1]) {
		command = replaceVariables("//gt\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=M-D\n@GREATER_{int}\nD;JGT\n@SP\nA=M\nM=0\n@CONTINUE_{int}\n0;JMP\n(GREATER_{int})\n@SP\nA=M\nM=-1\n(CONTINUE_{int})\n@SP\nM=M+1", vector<string>(), replacements);
	} else if (str == BOOLEAN_COMMANDS[2]) {
		command = replaceVariables("//lt\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=M-D\n@LESSER_{int}\nD;JLT\n@SP\nA=M\nM=0\n@CONTINUE_{int}\n0;JMP\n(LESSER_{int})\n@SP\nA=M\nM=-1\n(CONTINUE_{int})\n@SP\nM=M+1", vector<string>(), replacements);
	} else if (str == BOOLEAN_COMMANDS[3]) {
		command = "//and\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=D&M\nM=D\n@SP\nM=M+1";
	} else if (str == BOOLEAN_COMMANDS[4]) {
		command = "//or\n@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=D|M\nM=D\n@SP\nM=M+1";
	} else if (str == BOOLEAN_COMMANDS[5]) {
		command = "//not\n@SP\nM=M-1\nA=M\nD=!M\n@SP\nM=M+1";
	}

	return command;
}

int main(int argc, char* argv[]) { 

    if(argc > 2){ 
        cout << "Too many arguements." << endl;
        return 1;	
    } else if(argc <= 1) {
        cout << "Missing arguements." << endl;
        return 1;
    }
    
    string filePath = argv[1];
    ifstream file(filePath);

   if (!file.is_open()) {
       cerr << "Error: Could not open the file '" << filePath << "'." << endl;
       return 1;
    }
	array<string, 2> STACK_COMMANDS = {"push", "pop"};
	array<string, 3> INTEGER_COMMANDS = {"add", "sub", "neg"};
	array<string, 6> BOOLEAN_COMMANDS = {"eq", "gt", "lt", "and", "or", "not"};
    vector<vector<string> > lines;
	vector<string> tokens;
	string currentLine;
	string word;
	string line;
	int sp = STACK_START;
	int stepCounter = 0;

    while(getline(file, line)){
        string trimmedLine = trim(line);
        if(trimmedLine.length() != 0 && trimmedLine.substr(0,2) != "//") {
	    	istringstream lineStream(trimmedLine);
		    
		    while (lineStream >> word) {
		         tokens.push_back(word);
		     }
		     if (!tokens.empty()) {
		        lines.push_back(tokens);
		     	tokens.clear();
		     }
		 }
    }

	file.close();

	for (vector<string>& tline : lines) {
		if (find(STACK_COMMANDS.begin(), STACK_COMMANDS.end(), tline[0]) != STACK_COMMANDS.end()) {
			if (STACK_COMMANDS[0] == tline[0]) {
				cout << push(tline[2], tline[1]) << endl;
				sp++;
			} else if (STACK_COMMANDS[1] == tline[0]) {
				cout << line[2] << endl;
			}
		} else if (find(INTEGER_COMMANDS.begin(), INTEGER_COMMANDS.end(), tline[0]) != INTEGER_COMMANDS.end()) {
				cout << arithmetic(tline[0], INTEGER_COMMANDS) << endl;
		} else if (find(BOOLEAN_COMMANDS.begin(), BOOLEAN_COMMANDS.end(), tline[0]) != BOOLEAN_COMMANDS.end()) {
			cout << boolean(tline[0], BOOLEAN_COMMANDS, stepCounter) << endl;
		} else {
			cout << "Invalid operation!" << endl;
		}
		stepCounter++;

	}

	return 0;

}