@SP
AM=M-1
D=M
@output.0
M=D
@output.1
D=M
@SP
AM=M+1
A=A-1
M=D
@SP
M=M+1
A=M-1
M=1
@SP
AM=M-1
D=M
A=A-1
M=M-D
@SP
AM=M-1
D=M
@output.1
M=D
@output.backspace$if_end1
0;JMP
@31
D=A
@SP
AM=M+1
A=A-1
M=D
@SP
AM=M-1
D=M
@output.0
M=D
@output.1
D=M
@SP
