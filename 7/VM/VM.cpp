#include <iostream>

using namespace std;

//some constant values
const int SP = 0;
const int LCL = 1;
const int ARG = 2;
const int THIS = 3;
const int THAT = 4;
const int TEMP_LOW = 5;
const int TEMP_HIGH = 12;
const int RESERVED_LOW = 13;
const int RESERVED_HIGH = 15; //where R15 ends
const int STATIC_LOW = 16;
const int STATIC_HIGH 255;
const int STACK_START = 256;

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

    


}