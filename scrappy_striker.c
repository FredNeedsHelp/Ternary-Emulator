#include <windows.h>
#include "scrappy_striker.h"

//prototype funcs
results CPU_reset(CPU_t *cpu, memory *mem);
results CPU_execute(CPU_t *cpu, memory *mem, int_t clock_cycle);

void TernFetch(CPU_t *cpu, memory mem, int_t *clock_cycle, char_t *data);
results mem_read(memory *mem, int_t address, char_t *data);
void GetReg(CPU_t *cpu, memory *mem, int_t *clock_cycle, int_t *reg_dst, int_t *reg_src);
trit comp(int_t destination, int_t source1);
void comp_m(CPU_t *cpu, int_t dst, int_t src, bool max);
results load_mem(CPU_t *cpu, int_t destination, memory *mem, int_t memoryAddress);
results t_move(CPU_t *cpu, int_t destination, int_t source1);
results store_mem(CPU_t *cpu, int_t destination, memory *mem, int_t memoryAddress);
void CPU_status_end(CPU_t cpu, results rs);

void DUMP(CPU_t cpu, memory mem);
LARGE_INTEGER freq, start, end;
int CPUcycle = CPU_CYCLE_Default;

int main(void)
{
        char TASM[243];
        printf("TASM file name: ");
        scanf("%s", TASM);

        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&start);

        memory mem;
        CPU_t cpu;
        if(CPU_reset(&cpu, &mem) != success) {Throw("CPU failed to iniatlise");}
        ProgramLoader(&mem, TASM);
        results rs = CPU_execute(&cpu, &mem, D2T_conversion(CPUcycle, false));
        if(rs != success) {Throw("CPU failed to execute");}
        CPU_status_end(cpu, rs);
        DUMP(cpu, mem);

        return 0;
}

void DUMP(CPU_t cpu, memory mem) //DUMPS ALL MEMORY AND CPU STATES
{
        printf("\n\n- - - - - - - DUMP LOG OF EMULATOR - - - - - - -\n\n");

        //Prints Out Memory & CPU
        for(int i = 0; i < MEMORY_SIZE; i++)
        {
                printf("RAM[%d]:", i);
                printf("%d\n", T2C(mem.Data[i]));
        }

        printf("\nPointers: %d\nStack: %d\nFlag: %d", T2D_converter(cpu.pointer, true), T2D_converter(cpu.stack, true), cpu.flag);

        for(int i = 0; i < register_count; i++)
        {
                printf("\nRegister %d: ", i);
                printf("%d", T2D_converter(cpu.registers[i], false));
        }

        QueryPerformanceCounter(&end);

        double time_taken = (double)(end.QuadPart - start.QuadPart) * 1000 / (double)freq.QuadPart;
        printf("\nCPU Execution Time: %.4f ms\n", time_taken);
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
                int_t reg_dst = tern_int_zero, reg_src = tern_int_zero, memoryAddress = tern_int_zero, set_number = tern_int_zero;
                char_t address = tern_char_zero, flg = tern_char_zero;
                int dst_index = 0, src_index = 0; 

                switch(inst2bin)
                {
                        case dvd:
                                //Add Code Later
                                break;

                        case mlp:
                                //Add Code Later
                                break;

                        case set:
                                GetReg(cpu, mem, &clock_cycle, &reg_dst, &reg_src);  
                                dst_index = T2D_converter(reg_dst, false);
                                TernFetch(cpu, *mem, &clock_cycle, &address);
                                memcpy(set_number.int9, address, sizeof(char_t));
                                cpu->registers[dst_index] = set_number;
                                break;

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
                                TernFetch(cpu, *mem, &clock_cycle, &flg);
                                if(cpu->flag != flg[0]) break;
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
                                cpu->flag = comp(cpu->registers[dst_index], cpu->registers[src_index]);  
                                break;

                        case max:
                                GetReg(cpu, mem, &clock_cycle, &reg_dst, &reg_src);
                                comp_m(cpu, reg_dst, reg_src, true);
                                break;

                        case min:
                                GetReg(cpu, mem, &clock_cycle, &reg_dst, &reg_src);
                                comp_m(cpu, reg_dst, reg_src, false);                    
                                break;

                        case shl:
                                GetReg(cpu, mem, &clock_cycle, &reg_dst, NULL);
                                dst_index = T2D_converter(reg_dst, false);
                                TernFetch(cpu, *mem, &clock_cycle, &address);
                                memcpy(set_number.int9, address, sizeof(char_t));
                                shift(&cpu->registers[dst_index], set_number, true, false);
                                break;

                        case shr:
                                GetReg(cpu, mem, &clock_cycle, &reg_dst, NULL);
                                dst_index = T2D_converter(reg_dst, false);
                                TernFetch(cpu, *mem, &clock_cycle, &address);
                                memcpy(set_number.int9, address, sizeof(char_t));
                                shift(&cpu->registers[dst_index], set_number, false, false);
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
        memset(mem, 0, sizeof(memory)); //iniatlise RAM aka ZERO out
        memset(cpu, 0, sizeof(CPU_t));

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
        int add_temp = T2D_converter(memoryAddress, true);
        if(add_temp < 0 || add_temp >= MEMORY_SIZE) {return invalid_memory;}

        int rg_count = T2D_converter(destination, false);
        memcpy(mem->Data[T2D_converter(memoryAddress, true)], cpu->registers[rg_count].int9, sizeof(char_t));
        //*mem->Data[T2D_converter(memoryAddress, false)] = cpu->registers[rg_count].int9;
        return success;
}

results load_mem(CPU_t *cpu, int_t destination, memory *mem, int_t memoryAddress)
{
        int add_temp = T2D_converter(memoryAddress, true);
        if(add_temp < 0 || add_temp >= MEMORY_SIZE) {return invalid_memory;}

        int rg_count = T2D_converter(destination, false);
        //*cpu->registers[rg_count].int9 = *mem->Data[T2D_converter(memoryAddress, false)];
        memcpy(cpu->registers[rg_count].int9, mem->Data[T2D_converter(memoryAddress, true)], sizeof(char_t));
        return success;
}

trit comp(int_t destination, int_t source1) //compare 2 register values
{
        int reg1 = T2D_converter(destination, false);
        int reg2 = T2D_converter(source1, false);

        if(reg1 > reg2) return pos;
        else if(reg1 < reg2) return neg;
        else return net;
}

void comp_m(CPU_t *cpu, int_t dst, int_t src, bool max) //compare min|max, the m stands for both.
{
        int dst_index = T2D_converter(dst, false);
        int src_index = T2D_converter(src, false);

        if(max)
        {
                if(comp(cpu->registers[dst_index], cpu->registers[src_index]) == neg) 
                memcpy(&cpu->registers[dst_index], &cpu->registers[src_index], sizeof(int_t));
        }
        else if(!max)
        {
                if(comp(cpu->registers[dst_index], cpu->registers[src_index]) == pos) 
                memcpy(&cpu->registers[dst_index], &cpu->registers[src_index], sizeof(int_t));     
        }

        return;
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

void Throw(const char * __restrict__ LogMSG,...) //Throws an error
{
        va_list arg;

        va_start(arg, LogMSG);
        vprintf(LogMSG, arg);
        printf("\n");
        va_end(arg);

        exit(1);
}