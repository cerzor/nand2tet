// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

//// Replace this comment with your code.


(LOOP)

@KBD
A=M
D=A
@BLACK
D;JNE
@WHITE
D;JEQ
@LOOP
0;JMP

(BLACK)
@8191
D=A
@i
M=D
@SCREEN
M=-1

(REPB)
@i
D=M
@LOOP
D;JEQ
@SCREEN
A=A+D
M=-1
@i
M=M-1
@REPB
0;JMP

(WHITE)
@8191
D=A
@i
M=D
@SCREEN
M=0

(REPW)
@i
D=M
@LOOP
D;JEQ
@SCREEN
A=A+D
M=0
@i
M=M-1
@REPW
0;JMP

(END)
@END
0;JMP