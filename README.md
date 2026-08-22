# Ternary-Emulator
This is the basic core code for a ternary emulator made in C.

Im happy to announce that the Program Loader is complete, and now you can write your own custom TASM (Ternary Assembly) files
to make your own program. Just simply name the file test.tasm and put it next to the built program.
(I'll Add custom inputs when running the program for the tasm files next commit, I just want to go to sleep)

to build the program use gcc: gcc scrappy_striker.c SS_PLoader.c -o scrappy_striker

ISA to write your own TASM (Ternary Assembly) code:

add = 1, //Add between 2 regs
sub = 2,  //subtract between 2 regs
jmp = 3,  //jump to a line of code
load = 4, //load from memory
store = 5, //store in memory
move = 6, //move between registries (✓)
shr = 7, //shift trit to the right, NOT IMPLEMENTED YET!
shl = 8, //shift trit to the left, NOT IMPLEMENTED YET!
cmp = 9, //compare
max = 10, //NOT IMPLEMENTED YET, USE CMP
min = 11, //NOT IMPLEMENTED YET, USE CMP
flp = 12, //this is negative op, but due to naming conventions. it will be called flip to flip the numbers "polarity" (✓)
set = 13, //Set a number to a register.
quit = 0, //Quits the program. Not Recommended if your going to use DUMP
halt = 26, //halt stops the CPU immediately  
Unknown_Halt = -1 

Use ; to comment in your code, do not use it after valid code/instruction, the assembler does not deal with that type of circumstances yet.
