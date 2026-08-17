#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

#define register_count 3
#define MEMORY_SIZE 27 //terns of data, at moment 3^3=27
#define ArraySize(arr) (sizeof(arr) / sizeof(arr[0]))

typedef enum
{
        pos = 1,
        net = 0,
        neg = -1
} trit;

typedef struct
{
        trit int9[9]; //9-trit values
        trit int12[12]; //12-trit values
} int_t;

typedef trit char_t[5]; //a custom trit char

#define tern_zero (trit)0
#define tern_char_zero (char_t){0}

typedef enum 
{
        add = 1,  //(✓)
        sub = 2,  //(✓)
        jmp = 3,  //jump to a line of code
        load = 4, //load from memory
        store = 5, //store in memory
        move = 6, //move between registeries (✓)
        shr = 7, //shift to the right
        shl = 8, //shift to the left
        cmp = 9, //compare (✓)
        max = 10, //min & max are functionally redundent, only usefull for convience.
        min = 11,
        flp = 12, //this is negative op, but due to naming conventions. it will be called flip to flip the numbers "polarity" (✓)
        quit = 0,
        halt = MEMORY_SIZE - 1 //halt should be the max mem.(✓)
} op_codes;

//quit and halt are at the ends of the spectrum, if a overflow occurs and affects a "critical" system.
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
        int_t pointer, stack; //pointer means the program the counter btw
        int_t registers[register_count]; //int9       
        trit flag;
        bool halt;
} CPU_t;

//prototype funcs
results CPU_reset(CPU_t *cpu, memory *mem);
results CPU_execute(CPU_t *cpu, memory *mem, int_t clock_cycle);
int_t D2T_conversion(int8_t number, bool int12);
void TernFetch(CPU_t cpu, memory mem, int_t *clock_cycle, char_t *data);
int T2D_converter(int_t TernNumber, bool int12);
int_t TernaryAdd(int_t X, int_t Y, bool int12);
int_t TernarySub(int_t X, int_t Y, bool int12);
int_t flip(int_t X, bool int12);
results mem_read(memory *mem, int_t address, char_t *data);
results mem_write(memory *mem, int_t address, char_t *data);
void Throw(const char * __restrict__ LogMSG,...);

int main(void*)
{
        memory mem;
        CPU_t cpu;
        if(CPU_reset(&cpu, &mem) != success) {Throw("CPU failed to iniatlise");}
        if(CPU_execute(&cpu, &mem, D2T_conversion(2, false)) != success) {Throw("CPU failed to execute");}
        if(cpu.halt) {Throw("CPU was halted");}
        return 0;
}

results CPU_execute(CPU_t *cpu, memory *mem, int_t clock_cycle) //clock cycle accepts only a int9
{     
        printf("Clock Cycle(s) : %d\n", T2D_converter(clock_cycle, false));

        while (T2D_converter(clock_cycle, false) > 0)
        {
                char_t instruction = (char_t){0};
                TernFetch(*cpu, *mem, &clock_cycle, &instruction);
                printf("%d\n", T2D_converter(clock_cycle, false));

                unsigned char inst2bin = T2C(instruction);

                switch(inst2bin)
                {
                        case add:                               
                                break;
                        
                        case sub:
                                break;
                        
                        case jmp:
                                break;
                        
                        case move:
                                break;

                        case flp:
                                break;

                        case load:
                                break;

                        case store:
                                break;

                        case cmp:
                                break;

                        case quit:
                                break;

                        case halt:
                                cpu->halt = true;
                                return success;
                                break;
                        default:
                                Throw("No Instruction was set %c", inst2bin);
                                break;
                }
        }      
}

void TernFetch(CPU_t cpu, memory mem, int_t *clock_cycle, char_t *data)
{
        //char_t tmp_data; //= mem.Data[T2D_converter(cpu.pointer, true)];
        if(mem_read(&mem, cpu.pointer, &data) != success) {Throw("failed to read memory");}
        cpu.pointer = TernaryAdd(cpu.pointer, D2T_conversion(1, true), true);
        *clock_cycle = TernarySub(*clock_cycle, D2T_conversion(1, false), false);
        return;      
} //No need to refrence the mem or cpu, we are not modifying it. simply just reading.

results CPU_reset(CPU_t *cpu, memory *mem)
{
        //inaitlise the ram, so zero out
        for(uint32_t i = 0; i < MEMORY_SIZE; i++)
        {
                *mem->Data[i] = tern_char_zero;
        }

        cpu->halt = false;

        cpu->pointer = (int_t){0}; 
        cpu->stack = (int_t){0};
        
        for(uint32_t i = 0; i < register_count; i++)
        {
                cpu->registers[i] = (int_t){0};
        }
 
        return success;
}

char T2C(char_t ternary_char)
{
        int result = 0;
        int pw = 1;

        for(int i = 0; i < 5; i++)
                {
                        result += ternary_char[i] * pw;
                        pw *= 3;
                }

        return result;
}

results t_move(CPU_t *cpu, int_t destination, int_t source1) //move function to move between register(s)
{
        int rg_count = T2D_converter(destination, false);
        int rg_count1 = T2D_converter(source1, false);
        if(rg_count < 0 || rg_count >= register_count || rg_count1 < 0 || rg_count1 >= register_count) {Throw("Invalid Register Number");}

        cpu->registers[rg_count] = cpu->registers[rg_count1];
        return success;
}

results load_mem(CPU_t *cpu, int_t destination, memory *mem)
{
        int rg_count = T2D_converter(destination, false);
        *cpu->registers[rg_count].int9 = mem->Data;
        return success;
}

trit comp(CPU_t *cpu, int_t destination, int_t source1) //compare 2 register values
{
        int reg1 = T2D_converter(destination, false);
        int reg2 = T2D_converter(source1, false);

        if(reg1 > reg2) return pos;
        else if(reg1 < reg2) return neg;
        else return net;
}

//use these later for the safety checks.
results mem_read(memory *mem, int_t address, char_t *data) //reads memory with safe-checks
{
        int add_temp = T2D_converter(address, true);
        if(add_temp < 0 || add_temp >= MEMORY_SIZE) {return invalid_memory;}
        else
        data = mem->Data[add_temp];
        return success;
}

results mem_write(memory *mem, int_t address, char_t *data)//writes memory with safe-checks
{
        int add_temp = T2D_converter(address, true);
        if(add_temp < 0 || add_temp >= MEMORY_SIZE) {return invalid_memory;}
        else
        *mem->Data[add_temp] = *data;
        return success;
}

int T2D_converter(int_t TernNumber, bool int12) //Ternary to Decimal Converter
{
        int result = 0;
        int pw = 1;

        if(int12)
        {
                for(int i = 0; i < 12; i++)
                {
                        result += TernNumber.int12[i] * pw;
                        pw *= 3;
                }
        }
        else
        {
                for(int i = 0; i < 9; i++)
                {
                        result += TernNumber.int9[i] * pw;
                        pw *= 3;
                }
        }
        

        return result;
}

int_t D2T_conversion(int8_t number, bool int12) //Decimal to ternary conversion
{
        int_t rs = {0};
        int i = 0;

        while(number != 0)
        {
                if(int12) {if(i == 12) {break;} else if(i > 12) {Throw("Attempted to overflow; D2T");}}
                else {if(i == 9) {break;} else if(i > 9) {Throw("Attempted to overflow; D2T");}}

                int8_t q = number / 3;
                int8_t r = number % 3;

                if(int12)
                {
                        switch (r)
                        {
                                case 0:
                                        rs.int12[i] = 0;
                                        break;
                                case 1: 
                                        rs.int12[i] = 1;
                                        break;
                                case 2:
                                        rs.int12[i] = -1;
                                        q++;
                                        break;
                                default:
                                        Throw("Error converting to Trits");
                                        break;
                        }
                }
                else
                {
                        switch (r)
                        {
                                case 0:
                                        rs.int9[i] = 0;
                                        break;
                                case 1: 
                                        rs.int9[i] = 1;
                                        break;
                                case 2:
                                        rs.int9[i] = -1;
                                        q++;
                                        break;
                                default:
                                        Throw("Error converting to Trits");
                                        break;
                        }
                }

                number = q;
                i++;
        }

        return rs;
}

int_t TernaryAdd(int_t X, int_t Y, bool int12) //Both X and Y have to be the same int type
{
        trit carry = {0};
        int_t sum = {0};

        if(int12)
        for(int i = 0; i < 12; i++)
        {        
                int temp_sum = X.int12[i] + Y.int12[i] + carry;
                
                switch (temp_sum)
                {
                        case -3:
                                sum.int12[i] = 0;
                                carry = -1;
                                break;
                        case -2:
                                sum.int12[i] = 1;
                                carry = -1;
                                break;

                        case -1:
                                sum.int12[i] = -1;
                                carry = 0;
                                break;

                        case 0:
                                sum.int12[i] = 0;
                                carry = 0;
                                break;

                        case 1:
                                sum.int12[i] = 1;
                                carry = 0;
                                break;

                        case 2:
                                sum.int12[i] = -1;
                                carry = 1;
                                break;
                        case 3:
                                sum.int12[i] = 0;
                                carry = 1;
                                break;
                }
        
        }
        else
        for(int i = 0; i < 9; i++)
        {        
                int temp_sum = X.int9[i] + Y.int9[i] + carry;
                
                switch (temp_sum)
                {
                        case -3:
                                sum.int9[i] = 0;
                                carry = -1;
                                break;

                        case -2:
                                sum.int9[i] = 1;
                                carry = -1;
                                break;

                        case -1:
                                sum.int9[i] = -1;
                                carry = 0;
                                break;

                        case 0:
                                sum.int9[i] = 0;
                                carry = 0;
                                break;

                        case 1:
                                sum.int9[i] = 1;
                                carry = 0;
                                break;

                        case 2:
                                sum.int9[i] = -1;
                                carry = 1;
                                break;
                        case 3:
                                sum.int9[i] = 0;
                                carry = 1;
                                break;
                }
        
        }
        return sum;
}

int_t TernarySub(int_t X, int_t Y, bool int12)  //Both X and Y have to be the same int type
{
        return TernaryAdd(X, flip(Y, int12), int12);
}

int_t flip(int_t X, bool int12)
{
        int_t sum = {0};

        if(int12)
        for(int i = 0; i < 12; i++)
        {
                int temp = X.int12[i];

                switch (temp)
                {
                        case -1:
                                sum.int12[i] = 1;
                                break;
                        case 0:
                                sum.int12[i] = 0;
                                break;
                        case 1:
                                sum.int12[i] = -1;
                                break;
                }
        }
        else
        for(int i = 0; i < 9; i++)
        {
                int temp = X.int9[i];

                switch (temp)
                {
                        case -1:
                                sum.int9[i] = 1;
                                break;
                        case 0:
                                sum.int9[i] = 0;
                                break;
                        case 1:
                                sum.int9[i] = -1;
                                break;
                }
        }

        return sum;
}

void Throw(const char * __restrict__ LogMSG,...) //Throws an error
{
        va_list arg;

        va_start(arg, LogMSG);
        vprintf(LogMSG, arg);
        printf("\n");
        va_end(arg);

        exit(1);
}

