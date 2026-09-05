# Ternary-Emulator
This is the basic core code for a ternary emulator made in C. Nicknamed Scrappy Striker, because im going to try build a ternary 
computer with scraps and it will strike the world with its awesomeness. 
- - -
I would like to note that I have imposed a few limitations on the emulator, such as the ram being 27 cells of size big (3^3 = 27, 27 * char_t(5) = 135 trits of memory) even if the memory address can support much more. same also applies to the registries.

if you would like to change them, you can find the definitions in the scrappy_striker header (scrappy_striker.h).

This is an early prototype, many bugs will be present and I have only tested a small part of the emulator.

**Notes, Directions, and Extra**:

At the moment, the TASM code and Assembler cant process negative numbers; however, the CPU can work and store/load negative numbers.
if you truly require negative numbers, use the flip function temporarily. This will be fixed next commit. 

Im happy to announce that the Program Loader is complete, and now you can write your own custom TASM (Ternary Assembly) files
to make your own program. 

To load the program into the emulator make sure to provide a directory or place the file next to the exe.

to build the program use gcc: gcc scrappy_striker.c SS_PLoader.c scrappy_striker_math.c -o scrappy_striker
The main scrappy_striker.c does include a windows header for execution measurements, if your using linux/macos. simply edit the main function to replace windows with POSIX/UNIX func. 

ISA to write your own TASM (Ternary Assembly) code:

| Opcode | Value | Description | Notes |
| :--- | :---: | :--- | :--- |
| `add` | 1 | Add between 2 regs | implemented |
| `sub` | 2 | Subtract between 2 regs | implemented |
| `jmp` | 3 | Jump to a line of code | implemented |
| `load` | 4 | Load from memory | implemented |
| `store` | 5 | Store in memory | implemented |
| `move` | 6 | Move between registries | implemented |
| `shr` | 7 | Shift trit to the right | implemented |
| `shl` | 8 | Shift trit to the left | implemented |
| `cmp` | 9 | Compare between 2 regs| implemented |
| `max` | 10 | Find maximum between 2 regs| implemented |
| `min` | 11 | Find minimum between 2 regs| implemented |
| `flp` | 12 | Negative op, named flip to change number "polarity" | implemented |
| `set` | 13 | Set a number to a register | implemented |
| `mlp` | 14 | Multiply between 2 regs | implemented |
| `dvd` | 15 | Divide between 2 regs | NOT IMPLEMENTED YET |
| `quit` | 0 | Quits the program | Not recommended if using DUMP |
| `cyc_def` | 24 | defines how many cycles the CPU should run for | implemented |
| `halt` | 26 | Halt stops the CPU immediately | implemented |

Any Negative is value is controlled by the system! if any negative op codes appear, your cooked lollll. (Check RAM DUMP to see op codes, use the values from either the header or this table. dont forgot that data is also mixed in with the op codes; however, just follow your tasm file. its quite easy to translate from tasm to memory (machine code).

Also anything between 15 and 24 is reserved for future TISA (Ternary Instruction Set Architecture) instructions, such as abs. 

Use ; to comment in your code, anything and everything after the semicolon will not be interpreted by the assembler/parser/Program Loader

Right, this readme / architect.txt is probably not up-to-date. skim the code if your facing a problem or need something.

Control flow is fully working! Check the test.TASM file to see how it works. should be quite straightforward.

TASM code now requires an exit code when using quit ISA instruction, use 0 for success and anything else for an error. Result Enum for reference. 
Leaving it empty will result in a error / undefined behavior.

List of future additions:
- [ ] Assembler accepts Negative Numbers
- [ ] Dvd (division)
- [ ] Labels for Jump (control flaw)
- [ ] More Abstractions/TISA instructions.

Ill probably have more stuff to add, but forgot to put it in the list. 
