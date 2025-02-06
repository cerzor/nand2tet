//push constant 10
@10
D=A
@SP
A=M
M=D
@SP
M=M+1
//pop stack LCL 0
@LCL
AM=M-1
D=M
//push constant 21
@21
D=A
@SP
A=M
M=D
@SP
M=M+1
//push constant 22
@22
D=A
@SP
A=M
M=D
@SP
M=M+1
//pop stack argument 2
@2
AM=M-1
D=M
//pop stack argument 1
@1
AM=M-1
D=M
//push constant 36
@36
D=A
@SP
A=M
M=D
@SP
M=M+1
//pop stack THIS 6
@THIS
AM=M-1
D=M
//push constant 42
@42
D=A
@SP
A=M
M=D
@SP
M=M+1
//push constant 45
@45
D=A
@SP
A=M
M=D
@SP
M=M+1
//pop stack THAT 5
@THAT
AM=M-1
D=M
//pop stack THAT 2
@THAT
AM=M-1
D=M
//push constant 510
@510
D=A
@SP
A=M
M=D
@SP
M=M+1
//pop stack temp 6
@6
AM=M-1
D=M
//push LCL 0
@LCL
D=A
@SP
A=M
M=D
@SP
M=M+1
//push THAT 5
@THAT
D=A
@SP
A=M
M=D
@SP
M=M+1
//add
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M+D
@SP
A=M
M=D
@SP
M=M+1
//push argument 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
//subtract
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M+1
//push THIS 6
@THIS
D=A
@SP
A=M
M=D
@SP
M=M+1
//push THIS 6
@THIS
D=A
@SP
A=M
M=D
@SP
M=M+1
//add
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M+D
@SP
A=M
M=D
@SP
M=M+1
//subtract
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M+1
//push temp 6
@6
D=A
@SP
A=M
M=D
@SP
M=M+1
//add
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M+D
@SP
A=M
M=D
@SP
M=M+1
