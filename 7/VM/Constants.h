#ifndef CONSTANTS_H
#define CONSTANTS_H

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

#endif
