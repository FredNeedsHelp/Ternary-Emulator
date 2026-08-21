#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "scrappy_striker.h"

//prototype funcs
results CPU_reset(CPU_t *cpu, memory *mem);
results CPU_execute(CPU_t *cpu, memory *mem, int_t clock_cycle);

void TernFetch(CPU_t *cpu, memory mem, int_t *clock_cycle, char_t *data);
int T2D_converter(int_t TernNumber, bool int12);

int_t TernarySub(int_t X, int_t Y, bool int12);
int_t flip(int_t X, bool int12);
results mem_read(memory *mem, int_t address, char_t *data);
void GetReg(CPU_t *cpu, memory *mem, int_t *clock_cycle, int_t *reg_dst, int_t *reg_src);
char T2C(char_t ternary_char);
trit comp(CPU_t *cpu, int_t destination, int_t source1);
results load_mem(CPU_t *cpu, int_t destination, memory *mem, int_t memoryAddress);
results t_move(CPU_t *cpu, int_t destination, int_t source1);
results store_mem(CPU_t *cpu, int_t destination, memory *mem, int_t memoryAddress);
void CPU_status_end(CPU_t cpu, results rs);

void Throw(const char * __restrict__ LogMSG,...);

int main(void)
{
        memory mem;
        CPU_t cpu;
        if(CPU_reset(&cpu, &mem) != success) {Throw("CPU failed to iniatlise");}

        results rs = CPU_execute(&cpu, &mem, D2T_conversion(27, false));
        if(rs != success) {Throw("CPU failed to execute");}
        CPU_status_end(cpu, rs);

        return 0;
}

void CPU_status_end(CPU_t cpu, results rs)
{
        if(cpu.halt)
        {
                switch (rs)
                {
                        case success:
                                printf("CPU stopped\n");
                                break;
                        case unknown_error:
                                Throw("CPU was halted\n");
                                break;
                        case invalid_memory:
                                Throw("Invalid Memory Error; CPU has Halted\n");
                                break;
                        default:
                                Throw("CPU has stopped without a exit code\n");
                                break;
                }
                return;
        }
}

results CPU_execute(CPU_t *cpu, memory *mem, int_t clock_cycle) //clock cycle accepts only a int9
{     
        printf("Clock Cycle(s) : %d\n", T2D_converter(clock_cycle, false));

        while (T2D_converter(clock_cycle, false) > 0)
        {
                char_t instruction;
                TernFetch(cpu, *mem, &clock_cycle, &instruction);
                printf("%d\n", T2D_converter(clock_cycle, false));

                unsigned char inst2bin = T2C(instruction);
                int_t reg_dst = tern_int_zero, reg_src = tern_int_zero, memoryAddress = tern_int_zero;
                char_t address = tern_char_zero;
                int dst_index = 0, src_index = 0; 

                switch(inst2bin)
                {
                        case add:  
                                GetReg(cpu, mem, &clock_cycle, &reg_dst, &reg_src);  
                                dst_index = T2D_converter(reg_dst, false), 
                                src_index = T2D_converter(reg_src, false); 
                                cpu->registers[dst_index] = TernaryAdd(cpu->registers[dst_index], cpu->registers[src_index], false);
                                break; 
                        case sub:
                                GetReg(cpu, mem, &clock_cycle, &reg_dst, &reg_src); 
                                dst_index = T2D_converter(reg_dst, false), 
                                src_index = T2D_converter(reg_src, false);   
                                cpu->registers[dst_index] = TernarySub(cpu->registers[dst_index], cpu->registers[src_index], false);
                                break;
                        
                        case jmp:
                                TernFetch(cpu, *mem, &clock_cycle, &address);
                                memcpy(memoryAddress.int12, address, sizeof(char_t));
                                memcpy(cpu->pointer.int12, memoryAddress.int12, sizeof(trit) * 12);
                                break;
        
                        case move:                               
                                GetReg(cpu, mem, &clock_cycle, &reg_dst, &reg_src);  
                                t_move(cpu, reg_dst, reg_src);
                                break;

                        case flp:
                                GetReg(cpu, mem, &clock_cycle, &reg_dst, NULL);  
                                dst_index = T2D_converter(reg_dst, false);
                                cpu->registers[dst_index] = flip(cpu->registers[dst_index], false);
                                break;

                        case load:
                                GetReg(cpu, mem, &clock_cycle, &reg_dst, NULL);  
                                //dst_index = T2D_converter(reg_dst, false);
                                TernFetch(cpu, *mem, &clock_cycle, &address);
                                memcpy(memoryAddress.int12, address, sizeof(char_t));
                                load_mem(cpu, reg_dst, mem, memoryAddress); //load specfic mem address, incomplete!!!
                                break;

                        case store:
                                GetReg(cpu, mem, &clock_cycle, &reg_dst, NULL);  
                                //dst_index = T2D_converter(reg_dst, false);
                                TernFetch(cpu, *mem, &clock_cycle, &address);
                                memcpy(memoryAddress.int12, address, sizeof(char_t));
                                store_mem(cpu, reg_dst, mem, memoryAddress);
                                break;

                        case cmp:
                                GetReg(cpu, mem, &clock_cycle, &reg_dst, &reg_src);
                                dst_index = T2D_converter(reg_dst, false), 
                                src_index = T2D_converter(reg_src, false); 
                                cpu->flag = comp(cpu, cpu->registers[dst_index], cpu->registers[src_index]);  
                                break;

                        case shl:
                                Throw("Shift_Left has not been added yet\nif your seeing this, this might be memory corruption!");
                                break;

                        case shr:
                                Throw("Shift_Right has not been added yet\nif your seeing this, this might be memory corruption!");
                                break;

                        case quit:
                                cpu->halt = true;
                                //Add here quit function to zero out everything and quit "Peacefully"
                                return success;
                                break;

                        case halt:
                                cpu->halt = true;
                                return unknown_error;
                                break;
                        default:
                                Throw("No Instruction was set %c", inst2bin);
                                break;
                }
        }      
}



void TernFetch(CPU_t *cpu, memory mem, int_t *clock_cycle, char_t *data)
{
        //char_t tmp_data; //= mem.Data[T2D_converter(cpu.pointer, true)];
        if(mem_read(&mem, cpu->pointer, data) != success) {Throw("failed to read memory");}
        cpu->pointer = TernaryAdd(cpu->pointer, D2T_conversion(1, true), true);
        *clock_cycle = TernarySub(*clock_cycle, D2T_conversion(1, false), false);
        return;      
} //No need to refrence the mem or cpu, we are not modifying it. simply just reading.

results CPU_reset(CPU_t *cpu, memory *mem)
{
        memset(mem, 0, sizeof(char_t)); //iniatlise RAM aka ZERO out
        memset(cpu, 0, sizeof(int_t));

        //just in case, btw.
        cpu->halt = false;

        cpu->pointer = tern_int_zero; 
        cpu->stack = tern_int_zero;
        
        for(uint32_t i = 0; i < register_count; i++)
        {
                cpu->registers[i] = tern_int_zero;
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

results store_mem(CPU_t *cpu, int_t destination, memory *mem, int_t memoryAddress)
{
        int rg_count = T2D_converter(destination, false);
        memcpy(mem->Data[T2D_converter(memoryAddress, true)], cpu->registers[rg_count].int9, sizeof(char_t));
        //*mem->Data[T2D_converter(memoryAddress, false)] = cpu->registers[rg_count].int9;
        return success;
}

results load_mem(CPU_t *cpu, int_t destination, memory *mem, int_t memoryAddress)
{
        int rg_count = T2D_converter(destination, false);
        //*cpu->registers[rg_count].int9 = *mem->Data[T2D_converter(memoryAddress, false)];
        memcpy(cpu->registers[rg_count].int9, mem->Data[T2D_converter(memoryAddress, true)], sizeof(char_t));
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
        //data = mem->Data[add_temp];
        memcpy(data, &mem->Data[add_temp], sizeof(char_t));
        return success;
}

results mem_write(memory *mem, int_t address, char_t *data)//writes memory with safe-checks
{
        int add_temp = T2D_converter(address, true);
        if(add_temp < 0 || add_temp >= MEMORY_SIZE) {return invalid_memory;}
        else
        //*mem->Data[add_temp] = *data;
        memcpy(&mem->Data[add_temp], data, sizeof(char_t));
        return success;
}

int_t reg_decoder(char_t cell, bool dst) //returns an int9
{
        int packed = T2C(cell);   
        if(dst) 
        return D2T_conversion(packed % register_count, false); //destination
        else
        return D2T_conversion((packed / register_count) % register_count, false); //source1
}

void GetReg(CPU_t *cpu, memory *mem, int_t *clock_cycle, int_t *reg_dst, int_t *reg_src)
{
        char_t temp_cell = {0};
        TernFetch(cpu, *mem, clock_cycle, &temp_cell);
        if(reg_dst != NULL) *reg_dst = reg_decoder(temp_cell, true); else Throw("Register is NULL");
        if(reg_src != NULL) *reg_src = reg_decoder(temp_cell, false);   
        return;
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