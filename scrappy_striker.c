#include <windows.h>
#include "scrappy_striker.h"

//prototype funcs
results CPU_reset(CPU_t *cpu, memory *mem);
results CPU_execute(CPU_t *cpu, memory *mem, int12 clock_cycle);
results TERN_QUIT(CPU_t *cpu, memory *mem, results EXIT_CODE);

void TernFetch(CPU_t *cpu, memory mem, int12 clock_cycle, char_t *data);
results mem_read(memory *mem, int12 address, char_t *data);
void GetReg(CPU_t *cpu, memory *mem, int12 clock_cycle, int12 *reg_dst, int12 *reg_src);
results load_mem(CPU_t *cpu, int12 destination, memory *mem, int12 memoryAddress);
results t_move(CPU_t *cpu, int12 destination, int12 source1);
results store_mem(CPU_t *cpu, int12 destination, memory *mem, int12 memoryAddress);
void CPU_status_end(CPU_t cpu, results rs);

void DUMP(CPU_t cpu, memory mem, LabelInfo Label_info);
LARGE_INTEGER freq, start, end;
int CPUcycle = CPU_CYCLE_Default;

int main(void)
{
        char TASM[243];
        printf("TASM file name: ");
        scanf("%243s", TASM); //change later, unsafe lol; but lwk if it works it works.

        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&start);

        memory mem;
        CPU_t cpu;
        LabelInfo LI = {0};
        //Might make these globals, so Throw function will also DUMP for better debugging.

        if(CPU_reset(&cpu, &mem) != success) {Throw("CPU failed to iniatlise");}
        if(ProgramLoader(&mem, TASM, &LI) != success) Throw("Assembler Failed");
        int12 temp = {0}; D2T_int12(CPUcycle, temp); abs_int12(temp);
        results rs = CPU_execute(&cpu, &mem, temp);
        if(rs != success) {printf("CPU has encountered an Error: %d\n", rs);}
        CPU_status_end(cpu, rs);
        DUMP(cpu, mem, LI);

        return 0;
}

//Might make a stand-alone source file and have a debugging revolution basically (just make every error a known, and not spit out unknown_error).

void DUMP(CPU_t cpu, memory mem, LabelInfo Labels_Inf) //DUMPS ALL MEMORY, CPU STATES, AND LABELS (All Data basically)
{
        printf("\n\n- - - - - - - DUMP LOG OF EMULATOR - - - - - - -\n\n");

        //Prints out labels
        printf("Labels Produced By The Program:\n\n"); int l_counter = 0;
        for(int x = 0; x < MAX_LABELS; x++)
        {
                if(Labels_Inf.Name[x][0] != '\0')
                {
                        printf("|Label Name: %s & Memory Address: %d |", Labels_Inf.Name[x], T2D_int12(Labels_Inf.MemoryAddress[x]));
                        if(((x + 1) % 2) == 0) printf("\n"); 
                        l_counter++;
                }
                
                if(x + 1 == MAX_LABELS && l_counter == 0) printf("--NO LABELS PRODUCED--\n");
                if(x + 1  == MAX_LABELS) printf("\n\n");
        }

        //Prints Out Memory & CPU
        printf("Memory DUMP:\n\n");
        for(int i = 0; i < MEMORY_SIZE; i++)
        {
                printf("| RAM[%d]:", i);
                
                int tmp = T2C(mem.Data[i]);
                if(tmp > 9) printf("%d|", T2C(mem.Data[i]));
                else printf("%d |", T2C(mem.Data[i]));
                if(((i + 1) % 5) == 0) printf("\n"); 
        }

        printf("\nCPU DUMP:\n");
        printf("\nPointers: %d\nStack: %d\nFlag: %d", T2D_int12(cpu.pointer), T2D_int12(cpu.stack), cpu.flag);

        for(int i = 0; i < register_count; i++)
        {
                printf("\nRegister[%d]: %d", i, T2D_int12(cpu.registers[i]));
        }

        QueryPerformanceCounter(&end);

        double time_taken = (double)(end.QuadPart - start.QuadPart) * 1000 / (double)freq.QuadPart;
        printf("\nCPU Execution Time: %.4f ms\n", time_taken);
        printf("\n\n- - - - - - - By FreddyAdam - - - - - - -\n\n");
}

void CPU_status_end(CPU_t cpu, results rs)
{
        if(cpu.halt)
        {
                switch (rs)
                {
                        case success:
                                printf("CPU stopped succesfully\n");
                                break;
                        case unknown_error:
                                Throw("CPU was halted with an unknown error; 1\n");
                                break;
                        case invalid_memory:
                                Throw("Invalid Memory Error; CPU has Halted; 2\n");
                                break;
                        default:
                                Throw("CPU has stopped without a exit code: %d\n", rs);
                                break;
                }
                return;
        }
}

results CPU_execute(CPU_t *cpu, memory *mem, int12 clock_cycle)
{     
        printf("Clock Cycle(s) : %d\n", T2D_int12(clock_cycle));

        //more efficient than checking if clock cycle is larger than 0, abs is used so clock cycle cant be neg.
        while (is_zero_int12(clock_cycle) != pos) 
        {
                char_t instruction;
                TernFetch(cpu, *mem, clock_cycle, &instruction);
                printf("%d\n", T2D_int12(clock_cycle));

                unsigned char inst2bin = T2C(instruction);
                int12 reg_dst = tern_int_zero, reg_src = tern_int_zero, memoryAddress = tern_int_zero, set_number = tern_int_zero;
                char_t address = tern_char_zero, flg = tern_char_zero;
                int dst_index = 0, src_index = 0; 

                switch(inst2bin)
                {
                        case dvd:
                                GetReg(cpu, mem, clock_cycle, &reg_dst, &reg_src);  
                                dst_index = T2D_int12(reg_dst), 
                                src_index = T2D_int12(reg_src);          
                                dvd_int12(cpu->registers[dst_index], cpu->registers[src_index], &cpu->registers[dst_index]);
                                break;

                        case mlp:
                                GetReg(cpu, mem, clock_cycle, &reg_dst, &reg_src);  
                                dst_index = T2D_int12(reg_dst), 
                                src_index = T2D_int12(reg_src);          
                                mlp_int12(cpu->registers[dst_index], cpu->registers[src_index], &cpu->registers[dst_index]);
                                break;

                        case set:
                                GetReg(cpu, mem, clock_cycle, &reg_dst, &reg_src);  
                                dst_index = T2D_int12(reg_dst);
                                TernFetch(cpu, *mem, clock_cycle, &address);
                                memcpy(set_number, address, sizeof(char_t));
                                memcpy(cpu->registers[dst_index], set_number, sizeof(int12));
                                break;

                        case add:  
                                GetReg(cpu, mem, clock_cycle, &reg_dst, &reg_src);  
                                dst_index = T2D_int12(reg_dst), 
                                src_index = T2D_int12(reg_src);          
                                TernaryAdd_int12(cpu->registers[dst_index], cpu->registers[src_index], cpu->registers[dst_index]);
                                break; 
                        case sub:
                                GetReg(cpu, mem, clock_cycle, &reg_dst, &reg_src); 
                                dst_index = T2D_int12(reg_dst), 
                                src_index = T2D_int12(reg_src);          
                                TernarySub_int12(cpu->registers[dst_index], cpu->registers[src_index], cpu->registers[dst_index]);
                                break;
                        
                        case jmp:
                                TernFetch(cpu, *mem, clock_cycle, &flg);
                                //putting this before the check allows the counter to advance correctly even if the jump never happend.
                                TernFetch(cpu, *mem, clock_cycle, &address);
                                if(cpu->flag != flg[0]) break;                         
                                memcpy(memoryAddress, address, sizeof(char_t));
                                memcpy(cpu->pointer, memoryAddress, sizeof(int12));
                                break;
        
                        case move:                               
                                GetReg(cpu, mem, clock_cycle, &reg_dst, &reg_src);  
                                t_move(cpu, reg_dst, reg_src);
                                break;

                        case flp:
                                GetReg(cpu, mem, clock_cycle, &reg_dst, NULL);  
                                dst_index = T2D_int12(reg_dst);
                                flip_int12(cpu->registers[dst_index]);
                                break;

                        case load:
                                GetReg(cpu, mem, clock_cycle, &reg_dst, NULL);  
                                //dst_index = T2D_converter(reg_dst, false);
                                TernFetch(cpu, *mem, clock_cycle, &address);
                                memcpy(memoryAddress, address, sizeof(char_t));
                                load_mem(cpu, reg_dst, mem, memoryAddress); //load specfic mem address, incomplete!!!
                                break;

                        case store:
                                GetReg(cpu, mem, clock_cycle, &reg_dst, NULL);  
                                //dst_index = T2D_converter(reg_dst, false);
                                TernFetch(cpu, *mem, clock_cycle, &address);
                                memcpy(memoryAddress, address, sizeof(char_t));
                                store_mem(cpu, reg_dst, mem, memoryAddress);
                                break;

                        case cmp:
                                GetReg(cpu, mem, clock_cycle, &reg_dst, &reg_src);
                                dst_index = T2D_int12(reg_dst), 
                                src_index = T2D_int12(reg_src); 
                                cpu->flag = comp(cpu->registers[dst_index], cpu->registers[src_index]);  
                                break;

                        case max:
                                GetReg(cpu, mem, clock_cycle, &reg_dst, &reg_src);
                                comp_m(cpu, reg_dst, reg_src, true);
                                break;

                        case min:
                                GetReg(cpu, mem, clock_cycle, &reg_dst, &reg_src);
                                comp_m(cpu, reg_dst, reg_src, false);                    
                                break;

                        case shl:
                                GetReg(cpu, mem, clock_cycle, &reg_dst, NULL);
                                dst_index = T2D_int12(reg_dst);
                                TernFetch(cpu, *mem, clock_cycle, &address);
                                memcpy(set_number, address, sizeof(char_t));
                                //shift(&cpu->registers[dst_index], set_number, true, false);
                                shift_int12(&cpu->registers[dst_index], set_number, true);
                                break;

                        case shr:
                                GetReg(cpu, mem, clock_cycle, &reg_dst, NULL);
                                dst_index = T2D_int12(reg_dst);
                                TernFetch(cpu, *mem, clock_cycle, &address);
                                memcpy(set_number, address, sizeof(char_t));
                                //shift(&cpu->registers[dst_index], set_number, false, false);
                                shift_int12(&cpu->registers[dst_index], set_number, false);
                                break;

                        case quit:
                                cpu->halt = true;
                                TernFetch(cpu, *mem, clock_cycle, &address);                       
                                return TERN_QUIT(cpu, mem, T2C(address));
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
        
        return depleted_cycle; //just in-case cpu runs out of clock cycle
}

void TernFetch(CPU_t *cpu, memory mem, int12 clock_cycle, char_t *data)
{
        //char_t tmp_data; //= mem.Data[T2D_converter(cpu.pointer, true)];
        //printf("\n[Pointer DATA: %d]\n", T2D_int12(cpu->pointer));
        results rs_mem = mem_read(&mem, cpu->pointer, data);
        if(rs_mem != success) Throw("Ternary CPU fetch, failed to read memory: %d", rs_mem);
        //cpu->pointer = TernaryAdd(cpu->pointer, D2T_conversion(1, true), true);
        int12 rs = {0};
        D2T_int12(1, rs);
        TernaryAdd_int12(cpu->pointer, rs, cpu->pointer);
        //*clock_cycle = TernarySub(*clock_cycle, D2T_conversion(1, false), false);
        int12 temp = {0};
        D2T_int12(1, temp);
        TernarySub_int12(clock_cycle, temp, clock_cycle);
        return;      
}

results CPU_reset(CPU_t *cpu, memory *mem)
{
        memset(mem, 0, sizeof(memory)); //iniatlise RAM aka ZERO out
        memset(cpu, 0, sizeof(CPU_t));
        return success;
}

results t_move(CPU_t *cpu, int12 destination, int12 source1) //move function to move between register(s)
{
        int rg_count = T2D_int12(destination);
        int rg_count1 = T2D_int12(source1);
        if(rg_count < 0 || rg_count >= register_count || rg_count1 < 0 || rg_count1 >= register_count) {Throw("Invalid Register Number");}

        memcpy(cpu->registers[rg_count], cpu->registers[rg_count1], sizeof(int12));
        return success;
}

results store_mem(CPU_t *cpu, int12 destination, memory *mem, int12 memoryAddress)
{
        int add_temp = T2D_int12(memoryAddress);
        if(add_temp < 0) return invalid_memory;
        if(add_temp >= MEMORY_SIZE) {Throw("--SYSTEM ERROR: OUT OF MEMORY--"); return out_of_mem;}

        int rg_count = T2D_int12(destination);
        memcpy(mem->Data[add_temp], cpu->registers[rg_count], sizeof(char_t));
        //*mem->Data[T2D_converter(memoryAddress, false)] = cpu->registers[rg_count].int9;
        return success;
}

results load_mem(CPU_t *cpu, int12 destination, memory *mem, int12 memoryAddress)
{
        int add_temp = T2D_int12(memoryAddress);
        if(add_temp < 0) return invalid_memory;
        if(add_temp >= MEMORY_SIZE) {Throw("--SYSTEM ERROR: OUT OF MEMORY--"); return out_of_mem;}

        int rg_count = T2D_int12(destination);
        //*cpu->registers[rg_count].int9 = *mem->Data[T2D_converter(memoryAddress, false)];
        memcpy(cpu->registers[rg_count], mem->Data[add_temp], sizeof(char_t));
        return success;
}

trit comp(int12 destination, int12 source1) //compare 2 register values
{
        int12 diff = {0};
        TernarySub_int12(destination, source1, diff);
        if(is_zero_int12(diff) == pos) return net;

        for(int i = (Ternary_int - 1); i >= 0; i--)
        {
                if(diff[i] != 0) return diff[i];
        }
        
        return halt; //fall-back
}

void comp_m(CPU_t *cpu, int12 dst, int12 src, bool max) //compare min|max, the m stands for both.
{
        int dst_index = T2D_int12(dst);
        int src_index = T2D_int12(src);

        if(max)
        {
                if(comp(cpu->registers[dst_index], cpu->registers[src_index]) == neg) 
                memcpy(&cpu->registers[dst_index], &cpu->registers[src_index], sizeof(int12));
        }
        else if(!max)
        {
                if(comp(cpu->registers[dst_index], cpu->registers[src_index]) == pos) 
                memcpy(&cpu->registers[dst_index], &cpu->registers[src_index], sizeof(int12));     
        }

        return;
}

//use these later for the safety checks.
results mem_read(memory *mem, int12 address, char_t *data) //reads memory with safe-checks
{
        int add_temp =  T2D_int12(address);
        if(add_temp < 0) return invalid_memory;
        if(add_temp >= MEMORY_SIZE) {Throw("--SYSTEM ERROR: OUT OF MEMORY--"); return out_of_mem;}

        //data = mem->Data[add_temp];
        memcpy(data, &mem->Data[add_temp], sizeof(char_t));
        return success;
}

results mem_write(memory *mem, int12 address, char_t *data)//writes memory with safe-checks
{
        int add_temp =  T2D_int12(address);
        if(add_temp < 0) return invalid_memory;
        if(add_temp >= MEMORY_SIZE) {Throw("--SYSTEM ERROR: OUT OF MEMORY--"); return out_of_mem;}

        //*mem->Data[add_temp] = *data;
        memcpy(&mem->Data[add_temp], data, sizeof(char_t));
        return success;
}

void reg_decoder(char_t cell, int12 dst, int12 src)
{
        int packed = T2C(cell);   
        D2T_int12(packed % register_count, dst); //destination
        if(src == NULL) return;
        D2T_int12((packed / register_count) % register_count, src); //source1
        return;
}

void GetReg(CPU_t *cpu, memory *mem, int12 clock_cycle, int12 *reg_dst, int12 *reg_src)
{
        char_t temp_cell = {0};
        TernFetch(cpu, *mem, clock_cycle, &temp_cell);
        if(reg_dst == NULL) {Throw("GetReg Function has received a NULL for reg_dst");}
        reg_decoder(temp_cell, *reg_dst, *reg_src);
        return;
}

results TERN_QUIT(CPU_t *cpu, memory *mem, results EXIT_CODE)
{
        switch (EXIT_CODE)
        {
                case success:
                        //if(CPU_reset(cpu, mem) != success) {Throw("CPU failed to safely exit");}
                        return success;   
                default:
                        return EXIT_CODE;
        }

        //I made a switch statement so for future exit code implentation will be easier.
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

//Throw used to write to a .txt file, but that got annoying quick. 