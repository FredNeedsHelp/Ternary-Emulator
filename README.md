# Ternary-Emulator
This is the basic core code for a ternary emulator made in C. Nicknamed Scrappy Striker, because im going to try build a ternary 
computer with scraps and it will strike the world with its awesomeness. 
- - -
I would like to note that I have imposed a few limitations on the emulator, such as the ram being 27 trits long even if the memory address can support much more. same also applies to the registries.

if you would like to change them, you can find the definitions in the scrappy_striker header (scrappy_striker.h).

This is an early prototype, many bugs will be present and I have only tested the ALU because that's what I need.

**Notes, Directions, and Extra**:

Im happy to announce that the Program Loader is complete, and now you can write your own custom TASM (Ternary Assembly) files
to make your own program. 

To load the program into the emulator make sure to provide a directory or place the file next to the exe.

to build the program use gcc: gcc scrappy_striker.c SS_PLoader.c -o scrappy_striker

ISA to write your own TASM (Ternary Assembly) code:

| Opcode | Value | Description | Notes |
| :--- | :---: | :--- | :--- |
| `add` | 1 | Add between 2 regs | implemented |
| `sub` | 2 | Subtract between 2 regs | implemented |
| `jmp` | 3 | Jump to a line of code | implemented |
| `load` | 4 | Load from memory | implemented |
| `store` | 5 | Store in memory | implemented |
| `move` | 6 | Move between registries | implemented |
| `shr` | 7 | Shift trit to the right | NOT IMPLEMENTED YET! |
| `shl` | 8 | Shift trit to the left | NOT IMPLEMENTED YET! |
| `cmp` | 9 | Compare between 2 regs| implemented |
| `max` | 10 | Find maximum between 2 regs| NOT IMPLEMENTED YET, USE CMP |
| `min` | 11 | Find minimum between 2 regs| NOT IMPLEMENTED YET, USE CMP |
| `flp` | 12 | Negative op, named flip to change number "polarity" | implemented |
| `set` | 13 | Set a number to a register | implemented |
| `quit` | 0 | Quits the program | Not recommended if using DUMP |
| `halt` | 26 | Halt stops the CPU immediately | implemented |
| `Unknown_Halt` | -1 | Error/Unknown Halt condition | System state |


Use ; to comment in your code, anything and everything after the semicolon will not be interpreted by the assembler/parser/Program Loader

Control flow is fully working! Check the test.TASM file to see how it works. should be quite straightforward.
