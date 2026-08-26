//Scrappy Striker Program Loader (SSPL)
#include "scrappy_striker.h"

FILE *tasm_file = NULL;

op_codes LookUpTable(char *token)
{
        if(strcmp(token, "add") == 0) return add;                
        if(strcmp(token, "sub") == 0) return sub;                
        if(strcmp(token, "jump") == 0) return jmp;                
        if(strcmp(token, "load") == 0) return load;                
        if(strcmp(token, "store") == 0) return store;                
        if(strcmp(token, "move") == 0) return move;                
        if(strcmp(token, "cmp") == 0) return cmp;                
        if(strcmp(token, "max") == 0) return max;                
        if(strcmp(token, "min") == 0) return min;                
        if(strcmp(token, "flip") == 0) return flp;               
        if(strcmp(token, "quit") == 0) return quit;     
        if(strcmp(token, "set") == 0) return set;          
        if(token[0] == ';') return skip;
        if(strcmp(token, "halt") == 0) return halt;
        if(strcmp(token, "shl") == 0) return shl;
        if(strcmp(token, "shr") == 0) return shr;  
        if(strcmp(token, "CYC") == 0) return cyc_def;
        return Unknown_Halt;
}

int Parse4Reg(char *buffer)//hard coded for 3 registers, will change later to be dynamic with the register_count definition
{    
        if(strstr(buffer, "R") != NULL || strstr(buffer, "r") != NULL)
        {
                for(int i = 0; buffer[i] != '\0'; i++)
                {
                        if(!isdigit(buffer[i]) || ( i > 0 && isalpha(buffer[i - 1]) && (buffer[i - 1] != 'r' && buffer[i - 1] != 'R'))) {continue;}
                        
                        int numb = 0;

                        while(isdigit(buffer[i]))
                        {
                                numb = numb * 10 + (buffer[i] - '0');
                                i++;
                        }

                        if(!isalpha(buffer[i])) 
                        {
                                if(numb >= register_count) return -1;
                                else return numb;
                        } 
                }
        } else return -1;
}

void WriteMemTASM(memory *mem, int12 *address, char_t *data)
{
        mem_write(mem, *address, data);
        int12 rs = {0};
        D2T_int12(1, rs);
        TernaryAdd_int12(*address, rs, *address);
}


void RegEncoder(memory *mem, char *dst, char *src, int12 *address) //Parses for Register and then encodes it into memory
{
        int reg_Dst = 0, reg_Src = 0;
        reg_Dst = Parse4Reg(dst);
        if(src != NULL) {reg_Src = Parse4Reg(src);} else {reg_Src = 0;}
        if(reg_Dst == -1 || (reg_Src == -1 && src != NULL)) {Throw("No Valid Register Found");}
        char_t packed = tern_char_zero;
        C2T_conversion((reg_Dst + reg_Src * register_count), packed);                             
        WriteMemTASM(mem, address, &packed);
}

int Parse4Number(char *buffer)
{
        for(int i = 0; buffer[i] != '\0'; i++)
        {
                if(!isdigit(buffer[i]) || ( i > 0 && isalpha(buffer[i - 1]))) {i++; continue;}
                
                int numb = 0;

                while(isdigit(buffer[i]))
                {
                        numb = numb * 10 + (buffer[i] - '0');
                        i++;
                }

                if(!isalpha(buffer[i])) 
                {
                        return numb;
                } 
        }
}

//Takes any number that is present after the register/op, 
//used for jmp to get the address and set to get the value.
void Number2Mem(memory *mem, char *buffer, int12 *address) 
{
        int number = Parse4Number(buffer);
        char_t temp = tern_char_zero;
        C2T_conversion(number, temp);
        WriteMemTASM(mem, address, &temp);
        return;
}

trit State2Flag(memory *mem, char *buffer)
{       
        if(strstr(buffer, "NEG") || strstr(buffer, "neg")) return neg;
        if(strstr(buffer, "POS") || strstr(buffer, "pos")) return pos;
        if(strstr(buffer, "NET") || strstr(buffer, "net")) return net;
        return halt; //Cant use unknown error nor unknown halt, they both use -1 aka VALID VALUE :sob:
}

void ProgramLoader(memory *mem, char *tasm_name)
{
        if(!(tasm_file = fopen(tasm_name, "r"))) {Throw("No Input File detected");} //add input for the file!
          
        char buffer[243]; //line size limit, 3^5 = 243
        int12 address = tern_int_zero;

        //int z = 0;

        while(fgets(buffer, sizeof(buffer), tasm_file))
        {      
                //printf("Iteration: %d\n", z++);
                
                char *op = strtok(buffer, " \t\r\n");
                char *numb = strtok(NULL, " \t\r\n");
                char *regs = strtok(NULL, " \t\r\n");
                char_t code = tern_char_zero, flag = tern_char_zero;

                if(!op) continue;

                while(op != NULL)
                {
                        op_codes inst = LookUpTable(op);

                        switch (inst)
                        {
                                case shr:
                                        C2T_conversion(shr, code);
                                        WriteMemTASM(mem, &address, &code);
                                        RegEncoder(mem, numb, NULL, &address);
                                        Number2Mem(mem, regs, &address);
                                        break;

                                case shl:
                                        C2T_conversion(shl, code);
                                        WriteMemTASM(mem, &address, &code);
                                        RegEncoder(mem, numb, NULL, &address);
                                        Number2Mem(mem, regs, &address);
                                        break;

                                case cyc_def:
                                        CPUcycle = Parse4Number(numb);
                                        break;

                                case set:   
                                        C2T_conversion(set, code);
                                        WriteMemTASM(mem, &address, &code);
                                        RegEncoder(mem, numb, NULL, &address);
                                        Number2Mem(mem, regs, &address);
                                        break;
                                case add:
                                        C2T_conversion(add, code);
                                        WriteMemTASM(mem, &address, &code);
                                        RegEncoder(mem, numb, regs, &address);
                                        break;
                                case sub:
                                        C2T_conversion(sub, code);
                                        WriteMemTASM(mem, &address, &code);
                                        RegEncoder(mem, numb, regs, &address);
                                        break;
                                case load:
                                        C2T_conversion(load, code);
                                        WriteMemTASM(mem, &address, &code);
                                        RegEncoder(mem, numb, NULL, &address);
                                        Number2Mem(mem, regs, &address);
                                        break;
                                case store:
                                        C2T_conversion(store, code);
                                        WriteMemTASM(mem, &address, &code);
                                        RegEncoder(mem, numb, NULL, &address);
                                        Number2Mem(mem, regs, &address);        
                                        break;
                                case jmp:
                                        C2T_conversion(jmp, code);
                                        WriteMemTASM(mem, &address, &code);
                                        flag[0] = State2Flag(mem, numb);
                                        WriteMemTASM(mem, &address, &flag);
                                        Number2Mem(mem, regs, &address);
                                        break;
                                case move:
                                        C2T_conversion(move, code);
                                        WriteMemTASM(mem, &address, &code);
                                        RegEncoder(mem, numb, regs, &address);
                                        break;
                                case flp:
                                        C2T_conversion(flp, code);
                                        WriteMemTASM(mem, &address, &code);
                                        RegEncoder(mem, numb, NULL, &address);
                                        break;
                                case cmp:
                                        C2T_conversion(cmp, code);
                                        WriteMemTASM(mem, &address, &code);
                                        RegEncoder(mem, numb, regs, &address);
                                        break;
                                case max:
                                        C2T_conversion(max, code);
                                        WriteMemTASM(mem, &address, &code);
                                        RegEncoder(mem, numb, regs, &address);
                                        break;
                                case min:
                                        C2T_conversion(min, code);
                                        WriteMemTASM(mem, &address, &code);
                                        RegEncoder(mem, numb, regs, &address);
                                        break;
                                case skip:
                                        op = NULL; //Skip the Line
                                        continue;
                                case halt:
                                        C2T_conversion(halt, code);
                                        WriteMemTASM(mem, &address, &code);
                                        break;      
                                case quit:
                                        C2T_conversion(quit, code);
                                        WriteMemTASM(mem, &address, &code);
                                        Number2Mem(mem, numb, &address);
                                        break;                   
                                default:
                                        Throw("Error Program Loader, unable to store instruction into memory\n");
                                        break;
                        }

                        op = strtok(NULL, " \t\r\n");
                }
        }
}
