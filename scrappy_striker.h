#ifndef core_tern_header
#define core_tern_header

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

//Settings!
#define register_count 3
#define MEMORY_SIZE 27 //cells of data, at moment 3^3=27. 27 * char_t = 135 trits of data.
#define CPU_CYCLE_Default 27

#define ArraySize(arr) (sizeof(arr) / sizeof(arr[0]))

extern int CPUcycle;

typedef enum
{
        pos = 1,
        net = 0,
        neg = -1
} trit;

typedef trit char_t[5]; //a custom trit char
typedef trit int9[9]; //9-trit values
typedef trit int12[12]; //12-trit values

static const char surprise[] = "How in the flying f*ck did you get this error? this is meant to be unreachable.\npersonally reach to me if you get this";

#define tern_zero (trit)0
#define tern_int_zero {0}
#define tern_char_zero {0}

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
        flp = 12, //this is negative op, but due to naming conventions. it will be called flip to flip the numbers "polarity" (✓)
        set = 13,
        mlp = 14, 
        dvd = 15, //Not Added(yet)
        quit = 0,
        halt = 26,

        cyc_def = 23,
        label = 24,
        skip = 25,
        Unknown_Halt = -1 
} op_codes;

//quit and halt are at the ends of the spectrum, if a overflow occurs and affects a "critical" system, the system would shutdown.
//the program should be able to leave and log it.

typedef enum 
{
        success = 0,
        unknown_error = 1,
        invalid_memory = 2,
        depleted_cycle = 3
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
        bool halt;
} CPU_t;

void Throw(const char * __restrict__ LogMSG,...);

results mem_write(memory *mem, int12 address, char_t *data);
trit comp(int12 destination, int12 source1);
void comp_m(CPU_t *cpu, int12 dst, int12 src, bool max);

char T2C(char_t ternary_char);
void C2T_conversion(char number, char_t rs);
void ProgramLoader(memory *mem, char *tasm_name);

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

void D2T_int12(int8_t number, int12 rs);
void D2T_int9(int8_t number, int9 rs);

void shift_int12(int12 *ternary_number, int12 by, bool left);
void shift_int9(int9 *ternary_number, int9 by, bool left);

trit is_zero_int12(int12 x);
trit is_zero_int9(int9 x);

void abs_int12(int12 x);
void abs_int9(int9 x);

#endif