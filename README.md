
# Perkys Esoteric Programming Language

This is an esoteric programming language made by PerkyElixir22



## Instructions
<space>{reg}{reg} : Adds value second register to first register  
!{reg}{reg} : Subtracts value of second register from first register  
"{reg} : Checks if register is 0  
#{rel} : Jumps to code pointer + 8 bit signed offset if register is not 0  
${reg}{rel} : Loads value at code pointer + 8 bit signed offset into register  
%{rel}{reg} : Stores value in register into code pointer + 8 bit signed offset  
&{reg} : Increments register  
'{reg} : Decrements register  
({reg} : Prints register as ASCII  
){reg} : Reads character from input and stores it in register  
*{rel} : Jumps to code pointer + 8 bit signed offset  
+{reg}{reg} : Copies value of second register into first register  
,{reg} : Sets register value to 0  
-{reg} : Bitwise ANDs register with 0xFF  
.{reg}{imm} : Loads 4 bit immediate value into register  
/ : Exits program  

## Registers
< : 16 bit general purpose register  
\> : 16 bit general purpose register  
~ : 16 bit general purpose register  
^ : 16 bit general purpose register  
? : 16 bit general purpose register  
@ : 16 bit general purpose register  
= : 16 bit jump offset register (offsets JNE/JMP)  
| : 16 bit variable offset register (offsets Load/Store)  

## Numbers
0 : <space>  
1 : !  
2 : "  
3 : #  
4 : $  
5 : %  
6 : &  
7 : '  
8 : (  
9 : )  
A : *  
B : +  
C : ,  
D : -  
E : .  
F : /  
For example 244 would be /$
