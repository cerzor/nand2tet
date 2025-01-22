#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <cctype>
#include <map>
#include <vector>

using namespace std;
const char* whitespace = " \t\n\r\f\v";

inline string trim(const string& s, const char* t = whitespace)
{
    std::string result = s;
    result.erase(0, result.find_first_not_of(t));
    result.erase(result.find_last_not_of(t) + 1);
    return result;
}
string toBinary16(const string& str){
    int num = stoi(str);
    bitset<16> binary(num);
    return binary.to_string();
}

string findNextMemoryByValue(const map<string, string>& addressMap){
    int num = 100000;
    for (const  pair<const string, string>& pair : addressMap) {
        if (pair.second == to_string(num)) {
            num++;
        } else {
            break;
        }
    }
    bitset<16> binary(num);
    return binary.to_string();
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
    //Lookup tables
    map<string, string> destTable = {
        {"null", "000"},
        {"M","001"},
        {"D","010"},
        {"DM","011"},
        {"A","100"},
        {"AM","101"},
        {"AD","110"},
        {"ADM","111"}
    };

    map<string, string> compTable = {
        {"0","0101010"},
        {"1","0111111"},
        {"-1","0111010"},
        {"D","0001100"},
        {"A","0110000"},
        {"!D","0001101"},
        {"!A","0110001"},
        {"-D","0001111"},
        {"-A","0110011"},
        {"D+1","0011111"},
        {"A+1","0110111"},
        {"D-1","0001110"},
        {"A-1","0110010"},
        {"D+A","0000010"},
        {"D-A","0010011"},
        {"A-D","0000111"},
        {"D&A","0000000"},
        {"D|A","0010101"},
        {"M","1110000"},
        {"!M","1110001"},
        {"-M","1110011"},
        {"M+1","1110111"},
        {"M-1","1110010"},
        {"D+M","1000010"},
        {"D-M","1010011"},
        {"M-D","1000111"},
        {"D&M","1000000"},
        {"D|M","1010101"}
    };

    map<string, string> jmpTable = {
        {"null","000"},
        {"JGT","001"},
        {"JEQ","010"},
        {"JGE","011"},
        {"JLT","100"},
        {"JNE","101"},
        {"JLE","110"},
        {"JMP","111"}
    };
    //End lookup tables

    //vector<pair<string, string>> addressStore; //scared of memory usage
    map<string, string> addressMap; //scared of memory usage
    string D = to_string(0000000000000000);
    string A = to_string(0000000000000000);
    string M = to_string(0000000000000000);
    string binCode = "0000000000000000";
    string comp;
    string dest;
    string jmp;
    string a;
    string line;

    while(getline(file, line)){
        string trimmedLine = trim(line);
        if(trimmedLine.length() == 0) continue;

        if(trimmedLine.substr(0,2) != "//"){ //Check if the line is blank or a commment and ignore it.
            if (trimmedLine[0] == '@' &&
            (trimmedLine.substr(1).find_first_not_of("0123456789") ||
            (trimmedLine[1] == 'R' && trimmedLine.substr(2).find_first_not_of("0123456789")))) { //A instruction by mem reference
                    int offset = 1;
                    if (trimmedLine[1] == 'R') {
                        offset = 2;
                    }
                    binCode = toBinary16(trimmedLine.substr(offset));
                    A = stoi(binCode);
                    bool addPresent = addressMap.find("var") == addressMap.end();
                    if (addPresent){
                        addressMap[trimmedLine.substr(offset)] = stoi(binCode);
                    }
                    cout << binCode << endl;
            } else if (trimmedLine[0] == '@'){ //A instruction dynamic
                string varName = trimmedLine.substr(1, trimmedLine.size() -1);
                bool varPresent = addressMap.find("var") != addressMap.end();
                if (varPresent){
                    A = addressMap["varName"];
                } else {
                    A = findNextMemoryByValue(addressMap);
                }
                cout << A << endl;

            } else if(trimmedLine[0] == '('){ //L instruction
                // cout << trimmedLine << endl;
            }  else { //C instruction
                size_t endPos = trimmedLine.find(";");
                if (endPos == std::string::npos) {
                    jmp = "000";
                    endPos = trimmedLine.length();
                } else {
                    jmp = jmpTable[trim(trimmedLine.substr(trimmedLine.find(";") + 1))];
                }
                comp = compTable[trim(trimmedLine.substr(trimmedLine.find("=") + 1, endPos - (trimmedLine.find("=") + 1)))];
                dest = destTable[trim(trimmedLine.substr(0, trimmedLine.find("=")))];
                a = comp[0];
                binCode = "111" + a + comp.substr(1) + dest + jmp;
                cout << binCode << endl;
            }
        }
    }
    return 0;
}
