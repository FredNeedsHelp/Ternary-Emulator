#ifndef core_tern_header
#define core_tern_header

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#include <limits.h> //This emulator is meant to be built in C23, however; I will support older versions of C-standards
#include <stdbool.h> //At this moment (and to my knowledge), my code is supported by C99. im too lazy to check.

//Settings!
#define register_count 3 //For testing, i will be using 3 regs (2 regs is the min btw). 
#define MEMORY_SIZE 85 //cells of data, a single cell is a char_t which is 12 trits long. 12 * Memory_Size = How many trits the system has. 
#define CPU_CYCLE_Default 27 //Use CYC in TASM code to change.
#define MAX_LABELS 20 //Labels are very memory & cycle intensive, if your planning on using a large # of labels its also advised to increase memory and cycles.

extern int CPUcycle;

typedef enum
{
        pos = 1,
        net = 0,
        neg = -1
} trit;

#define Ternary_Char_Size 12 //Changed from 5 to 12 so mem to reg(and vice versa) would not be UB and for mem cell to hold more than 243 values (-121 to 121)
#define Ternary_short_int 9
#define Ternary_int 12
#define Ternary_wide_int 24

typedef trit char_t[12]; //a custom trit char
typedef trit int9[9]; //9-trit values
typedef trit int12[12]; //12-trit values
typedef trit int24[24]; //24-trit values

//Might add a "Normal int" that is X long and can change the emulator architecture.

static const char surprise[] = "How in the flying f*ck did you get this error? this is meant to be unreachable.\npersonally reach to me if you get this";

#define tern_zero (trit)0 //In earlier versions of the emulator, tern was used to refer to trit; due to this being load-bearing and me being a lazy piece of shit.
#define tern_int_zero {0} //Im not changing it unless it starts to annoy me, which sounds like never as my laziness exceeds all non-critical feelings. 
#define tern_char_zero {0} //I also know this is a 5 min fix, but lwk... I js dont wanna.

typedef enum 
{     
        add = 1,  
        sub = 2,  
        jmp = 3,  //jump to a line of code (memory address or label)
        load = 4, //load from memory to regs
        store = 5, //store in memory from reg
        move = 6, //move between regs
        shr = 7, //shift trits to the right by x amount
        shl = 8, //shift trits to the left by x amount
        cmp = 9, //compare
        max = 10, 
        min = 11,
        flp = 12, //this is negative op, but due to naming conventions. it will be called flip to flip the numbers "polarity"
        set = 13,
        mlp = 14, 
        dvd = 15, //Semi-dvd
        noop = 16, //No Operation
        quit = 0,
        halt = 26,

        cyc_def = 23,
        label = 24,
        skip = 25,
        Unknown_Halt = -1,
} op_codes;

/*
All Numbers between 16-23 is reserved for future TISA instructions, but due to all the primitives (that are needed) are already added. 
New TISA instructions would most likely regard the stack, e.g: pop, push, call, return, etc. OR, might be abstractions even though
TISA is a RISC-based instruction set. 

TISA is a blend between RISC and CISC, if you havent realised yet. small Instruction set while still using flags for branching.
*/


typedef enum 
{
        success,
        unknown_error,
        invalid_memory,
        out_of_mem,
        depleted_cycle
} results;

typedef struct 
{
        char_t Data[MEMORY_SIZE];
} memory;

typedef struct
{
        //pointer means the program counter btw
        int12 registers[register_count], pointer, stack;      
        trit flag;
        bool halt; //might change all bools to trits so it fits the vibe, even tho the third value would probably will never be used.
} CPU_t;

typedef struct
{
        char Name[MAX_LABELS][243]; //Name of the label
        int12 MemoryAddress[MAX_LABELS]; //Memory Address the label points to
} LabelInfo;

void Throw(const char * __restrict__ LogMSG,...);

results mem_write(memory *mem, int12 address, char_t *data);
trit comp(int12 destination, int12 source1);
void comp_m(CPU_t *cpu, int12 dst, int12 src, bool max);

results ProgramLoader(memory *mem, char *tasm_name, LabelInfo *Labels_Info);

//(Common) Math Functions Below: 

int T2C(char_t ternary_char); //Ternary to Char/number
void C2T_conversion(int number, char_t rs);
//Due to char most commenly being a uint8, which is 256, it does not cleanly convert into new Char_t which is 3^12, which is half a million.
//C2T is now an Int to Char_T converter. 

int T2D_int12(int12 TernNumber);
int T2D_int9(int9 TernNumber);

void TernaryAdd_int12(int12 X, int12 Y, int12 sum);
void TernaryAdd_int9(int9 X, int9 Y, int9 sum);

void TernarySub_int12(int12 X, int12 Y, int12 sum);
void TernarySub_int9(int9 X, int9 Y, int9 sum);

void mlp_int12(int12 x, int12 y, int12 *product);
void mlp_int9(int9 x, int9 y, int9 *product);

void dvd_int12(int12 x, int12 y, int12 *rs);
void dvd_int9(int9 x, int9 y, int9 *rs);

void flip_int12(int12 X);
void flip_int9(int9 X);

void D2T_int12(int16_t number, int12 rs);
void D2T_int9(int8_t number, int9 rs);

void shift_int12(int12 *ternary_number, int12 by, bool left);
void shift_int9(int9 *ternary_number, int9 by, bool left);

trit is_zero_int12(int12 x);
trit is_zero_int9(int9 x);

void abs_int12(int12 x);
void abs_int9(int9 x);

#endif