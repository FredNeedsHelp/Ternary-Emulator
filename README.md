# Ternary-Emulator
This is the basic core code for a ternary emulator made in C.

Im happy to announce that the Program Loader is complete, and now you can write your own custom TASM (Ternary Assembly) files
to make your own program. Just simply name the file test.tasm and put it next to the built program.
(I'll Add custom inputs when running the program for the tasm files next commit, I just want to go to sleep)

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


Use ; to comment in your code, do not use it after valid code/instruction, the assembler does not deal with that type of circumstances yet.
