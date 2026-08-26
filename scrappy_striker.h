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
        jmp = 3,  //jump to a line of code
        load = 4, //load from memory
        store = 5, //store in memory
        move = 6, //move between registeries
        shr = 7, //shift to the right
        shl = 8, //shift to the left
        cmp = 9, //compare
        max = 10, 
        min = 11,
        flp = 12, //this is negative op, but due to naming conventions. it will be called flip to flip the numbers "polarity" (✓)
        set = 13,
        mlp = 14, //Not Added 
        dvd = 15, //Not Added
        quit = 0,
        halt = MEMORY_SIZE - 1, //halt should be the max mem.

        cyc_def = MEMORY_SIZE - 3,
        skip = MEMORY_SIZE - 2,
        Unknown_Halt = -1 
} op_codes;

//quit and halt are at the ends of the spectrum, if a overflow occurs and affects a "critical" system, the system would shutdown.
//the program should be able to leave and log it.

typedef enum 
{
        success = 0,
        unknown_error = 1,
        invalid_memory = 2
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


char T2C(char_t ternary_char);
void C2T_conversion(char number, char_t rs);
void ProgramLoader(memory *mem, char *tasm_name);

int T2D_int12(int12 TernNumber);
int T2D_int9(int9 TernNumber);

void TernaryAdd_int12(int12 X, int12 Y, int12 sum);
void TernaryAdd_int9(int9 X, int9 Y, int9 sum);

void TernarySub_int12(int12 X, int12 Y, int12 sum);
void TernarySub_int9(int9 X, int9 Y, int9 sum);

void flip_int12(int12 X, int12 sum);
void flip_int9(int9 X, int9 sum);

void D2T_int12(int8_t number, int12 rs);
void D2T_int9(int8_t number, int9 rs);

void shift_int12(int12 *ternary_number, int12 by, bool left);
void shift_int9(int9 *ternary_number, int9 by, bool left);

#endif