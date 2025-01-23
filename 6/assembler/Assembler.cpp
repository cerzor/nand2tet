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
    vector<int> values;
    for (const auto& pair : addressMap) {
        int value = stoi(pair.second, nullptr, 2);  // Convert binary string to an integer.
        if (value >= 16 && value < 16384) {  // We only care about values in the range [16, 16383].
            values.push_back(value);
        }
    }

    sort(values.begin(), values.end());

    // Start searching for the next available value greater than or equal to 16.
    int num = 16;
    for (int value : values) {
        if (value == num) {
            num++;  // If the value is already taken, move to the next number.
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
        {"MD","011"},
        {"A","100"},
        {"AM","101"},
        {"AD","110"},
        {"MA","101"},
        {"DA","110"},
        {"ADM","111"},
        {"AMD","111"},
        {"DAM","111"},
        {"DMA","111"},
        {"MAD","111"},
        {"MDA","111"}
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

    map<string, string> preDef = {
        {"SP", "0000000000000000"},
        {"LCL", "0000000000000001"},
        {"ARG", "0000000000000010"},
        {"THIS", "0000000000000011"},
        {"THAT", "0000000000000100"},
        {"R0", "0000000000000000"},
        {"R1", "0000000000000001"},
        {"R2", "0000000000000010"},
        {"R3", "0000000000000011"},
        {"R4", "0000000000000100"},
        {"R5", "0000000000000101"},
        {"R6", "0000000000000110"},
        {"R7", "0000000000000111"},
        {"R8", "0000000000001000"},
        {"R9", "0000000000001001"},
        {"R10", "0000000000001010"},
        {"R11", "0000000000001011"},
        {"R12", "0000000000001100"},
        {"R13", "0000000000001101"},
        {"R14", "0000000000001110"},
        {"R15", "0000000000001111"},
        {"SCREEN", "0100000000000000"},
        {"KBD", "0110000000000000"}
    };
    //End lookup tables
    map<string, string> addressMap; //scared of memory usage
    string D = "0000000000000000";
    string A = "0000000000000000";
    string M = "0000000000000000";
    string binCode = "0000000000000000";
    string comp;
    string dest;
    string jmp;
    string a;
    string line;
    int lineCount = 0;
    int commentLines = 0;
    vector<string> lines;
    vector<string> labels;

    while(getline(file, line)){
        string trimmedLine = trim(line);
        if(trimmedLine.length() == 0 || trimmedLine.substr(0,2) == "//") {
            commentLines++;
            continue;
        }
        if(trimmedLine[0] == '('){
            labels.push_back(trimmedLine);
        }
        lines.push_back(trimmedLine);
    }

    for (string& label : labels){
        string varName = label.substr(1, label.size()-2);
        int lineIndex = find(lines.begin(), lines.end(), "(" + varName + ")") - lines.begin();
        bitset<16> binary(lineIndex - lineCount);
        addressMap[varName] = binary.to_string();
        lineCount++;
    }

    for (const pair<const string, string>& pre : preDef){
        addressMap[pre.first] = pre.second;
    }
    for (string& line : lines) {
        if(line.substr(0,2) != "//" && line[0] != '('){ //Check if the line is blank or a commment and ignore it.
            if (line[0] == '@' &&
            (line.substr(1).find_first_not_of("0123456789") ||
            (line[1] == 'R' && line.substr(2).find_first_not_of("0123456789")))) { //A instruction by mem reference
                    int offset = 1;
                    if (line[1] == 'R') {
                        offset = 2;
                    }
                    binCode = toBinary16(line.substr(offset));
                    A = binCode;
                    bool addPresent = addressMap.find(line.substr(offset)) == addressMap.end();
                    if (addPresent){
                        addressMap[line.substr(offset)] = A;
                    }
                    cout << binCode << endl;
            } else if (line[0] == '@'){ //A instruction dynamic
                string varName = line.substr(1, line.size() - 1);
                bool preDefPresent = preDef.find(varName) != preDef.end();
                if (preDefPresent) {
                    A = preDef[varName];
                } else {
                    bool varPresent = addressMap.find(varName) != addressMap.end();
                    if (varPresent){
                        A = addressMap[varName];
                    } else {
                        A = findNextMemoryByValue(addressMap);
                        addressMap[varName] = A;
                    }
                }
                cout << A << endl;

            }  else { //C instruction
                size_t endPos = line.find(";");
                if (endPos == std::string::npos) {
                    jmp = "000";
                    endPos = line.length();
                    dest = destTable[trim(line.substr(0, line.find("=")))];
                } else {
                    dest = destTable[line.substr(0, endPos + 1)];
                    jmp = jmpTable[trim(line.substr(line.find(";") + 1))];
                }
                comp = compTable[trim(line.substr(line.find("=") + 1, endPos - (line.find("=") + 1)))];

                dest = (!dest.empty()) ? dest : "000";
                a = comp[0];
                binCode = "111" + a + comp.substr(1) + dest + jmp;

                cout << binCode << endl;
            }
        }
    }
    return 0;
}
