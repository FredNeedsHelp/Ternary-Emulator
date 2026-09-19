# Ternary-Emulator
This is the basic core code for a ternary emulator made in C. Nicknamed Scrappy Striker, because im going to try build a ternary 
computer with scraps and it will strike the world with its awesomeness. 
- - -
As of 19/09/2026, development will stop or at the very least be extremely slow. I have reached a point where the emulator is very capable while being still outputting correctly, so I have decided that I should start
designing the Ternary ALU and the real hardware for the real Ternary CPU. That was always the goal, this emulator was simply a requisite for that goal.

if you'd like to ask me any questions or help/assist me, here is my discord: thedevthatcantcode

I would like to note that I have imposed a few limitations on the emulator, such as the ram being 85 cells big, which equates to 1020 trits of data available to the system. The memory address system can handle much more, roughly being able to handle half a million addresses.

if you would like to change them, you can find the definitions in the scrappy_striker header (scrappy_striker.h).

This is an early prototype, many bugs will be present and I have only tested a small part of the emulator.

**Notes, Directions, and Extra**:

As I have promised, Labels have been added. Check the Easy_Labels.tasm and the fibonci_sequence.tasm to see how labels are used. (fibonci_sequence showcasing recursion/loops).

To load the program into the emulator make sure to provide a directory or place the file next to the exe.

I should note for anybody on a POSIX/UNIX system, that the main {scrappy_striker.c} does include a windows header for execution measurements, if your using linux/macos. simply edit the main and dump functions to replace windows with POSIX/UNIX alternatives. 

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
| `dvd` | 15 | Divide between 2 regs | implemented |
| `noop` | 16 | No Operation | implemented |
| `quit` | 0 | Quits the program | Not recommended if using DUMP |
| `cyc_def` | 24 | defines how many cycles the CPU should run for | implemented |
| `halt` | 26 | Halt stops the CPU immediately | implemented |

Any Negative is value is controlled by the system! if any negative op codes appear, your cooked lollll. (Check RAM DUMP to see op codes, use the values from either the header or this table. dont forgot that data is also mixed in with the op codes; however, just follow your tasm file. its quite easy to translate from tasm to memory (machine code). Btw, this does use a Von' Neumann Architecture if that wasn't clear.

Also anything between 16 and 24 is reserved for future TISA (Ternary Instruction Set Architecture) instructions, such as abs. 

Use ; to comment in your code, anything and everything after the semicolon will not be interpreted by the assembler/parser/Program Loader
I should also note that adding comments in-front of instructions that take only a single argument like quit and cyc will be UB and the 
program loader will fail (and throw a Unkown_Halt which will be -1). 

I would to also note that this readme.md / architect.txt is probably not up-to-date. skim the code if your facing a problem or need something.

Control flow is fully working! Check the test.TASM file to see how it works. should be quite straightforward.

TASM code now requires an exit code when using quit ISA instruction, use 0 for success and anything else for an error. Result Enum for reference. 
Leaving it empty will result in a error / undefined behavior.

List of future additions:
- [ ] True Dvd (division)
- [ ] Standard Library for TASM code

Ill probably have more stuff to add, but forgot to put it in the list. 
