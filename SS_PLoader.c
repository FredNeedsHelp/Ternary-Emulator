//Scrappy Striker Program Loader (SSPL)
#include <ctype.h>
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
        if(strcmp(token, ";") == 0) return skip;
        if(strcmp(token, "halt") == 0) return halt;  
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
                                if(numb > register_count) return -1;
                                else return numb;
                        } 
                }
        } else return -1;
}

void WriteMemTASM(memory *mem, int_t *address, char_t *data)
{
        mem_write(mem, *address, data);
        *address = TernaryAdd(*address, D2T_conversion(1, true), true);
}

void C2T_conversion(char number, char_t rs) //Decimal to ternary conversion
{
        memset(rs, 0, sizeof(char_t));
        int i = 0;

        while(number != 0)
        {
                if(i == 5) {break;} else if(i > 5) {Throw("Attempted to overflow; C2T");}

                int8_t q = number / 3;
                int8_t r = number % 3;

                switch (r)
                        {
                                case 0:
                                        rs[i] = 0;
                                        break;
                                case 1: 
                                        rs[i] = 1;
                                        break;
                                case 2:
                                        rs[i] = -1;
                                        q++;
                                        break;
                                default:
                                        Throw("Error converting to Trits");
                                        break;
                        }

                number = q;
                i++;
        }

        return;
}

void RegEncoder(memory *mem, char *dst, char *src, int_t *address) //Parses for Register and then encodes it into memory
{
        int reg_Dst = 0, reg_Src = 0;
        reg_Dst = Parse4Reg(dst);
        if(src != NULL) {reg_Src = Parse4Reg(src);} else {reg_Src = 0;}
        if(reg_Dst == -1 || (reg_Src == -1 && src != NULL)) {Throw("No Valid Register Found");}
        char_t packed = tern_char_zero;
        C2T_conversion((reg_Dst + reg_Src * register_count), packed);                             
        WriteMemTASM(mem, address, &packed);
}

//Takes any number that is present after the register/op, 
//used for jmp to get the address and set to get the value.
void Number2Mem(memory *mem, char *buffer, int_t *address) 
{
        int i = 0;
        int number = 0;
        
        while(buffer[i] != '\0')
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
                        number = numb;
                        break;
                } 
        }

        char_t temp = tern_char_zero;
        C2T_conversion(number, temp);
        WriteMemTASM(mem, address, &temp);
        return;
}

void ProgramLoader(memory *mem, char *tasm_name)
{
        if(!(tasm_file = fopen(tasm_name, "r"))) {Throw("No Input File detected");} //add input for the file!
          
        char buffer[243]; //line size limit, 3^5 = 243
        int_t address = tern_int_zero;

        //int z = 0;

        while(fgets(buffer, sizeof(buffer), tasm_file))
        {      
                //printf("Iteration: %d\n", z++);
                
                char *op = strtok(buffer, " \t\r\n");
                char *numb = strtok(NULL, " \t\r\n");
                char *regs = strtok(NULL, " \t\r\n");
                char_t code = tern_char_zero;

                if(!op) continue;

                while(op != NULL)
                {
                        op_codes inst = LookUpTable(op);

                        switch (inst)
                        {
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
                                        Number2Mem(mem, numb, &address);
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
                                        break;                   
                                default:
                                        Throw("Error Program Loader, unable to store instruction into memory\n");
                                        break;
                        }

                        op = strtok(NULL, " \t\r\n");
                }
        }
}
