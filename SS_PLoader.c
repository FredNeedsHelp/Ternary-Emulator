//Scrappy Striker Program Loader (SSPL)

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "scrappy_striker.h"

FILE *tasm_file = NULL;

op_codes LookUpTable(char *token)
{
        if(strcmp(token, "add") == 0)
                return add;
        if(strcmp(token, "sub") == 0)
                return sub;
        if(strcmp(token, "jump") == 0)
                return jmp;
        if(strcmp(token, "load") == 0)
                return load;
        if(strcmp(token, "store") == 0)
                return store;
        if(strcmp(token, "move") == 0)
                return move;
        if(strcmp(token, "cmp") == 0)
                return cmp;
        if(strcmp(token, "max") == 0)
                return max;
        if(strcmp(token, "min") == 0)
                return min;
        if(strcmp(token, "flip") == 0)
                return flp;
        if(strcmp(token, "quit") == 0)
                return quit;
        if(strcmp(token, ";") == 0)
                return skip;

        return halt; //Unknown Instruction
}

void ProgramLoader(memory *mem)
{
        if(!(tasm_file = fopen("test.tasm", "r"))) {printf("No Input File detected");} //add input for the file!
          
        char buffer[243]; //file size limit, 3^5 = 243
        int_t address = tern_int_zero;

        while(fgets(buffer, sizeof(buffer), tasm_file))
        {       
                char *op = strtok(buffer, " \t \n");
                char *numb = strtok(NULL, " \t\n");
                char *regs = strtok(NULL, " \t\n");
                if(!op) continue;

                while(op != NULL)
                {
                        op_codes inst = LookUpTable(op);

                        switch (inst)
                        {
                                case add:
                                        mem_write(mem, address, add);
                                        address = TernaryAdd(address, D2T_conversion(1, true), true);
                                        break;
                                case sub:
                                        mem_write(mem, address, sub);
                                        address = TernaryAdd(address, D2T_conversion(1, true), true);
                                        break;
                                case load:
                                        mem_write(mem, address, load);
                                        address = TernaryAdd(address, D2T_conversion(1, true), true);
                                        break;
                                case store:
                                        mem_write(mem, address, store);
                                        address = TernaryAdd(address, D2T_conversion(1, true), true);
                                        break;
                                case jmp:
                                        mem_write(mem, address, jmp);
                                        address = TernaryAdd(address, D2T_conversion(1, true), true);
                                        break;
                                case move:
                                        mem_write(mem, address, move);
                                        address = TernaryAdd(address, D2T_conversion(1, true), true);
                                        break;
                                case flp:
                                        mem_write(mem, address, flp);
                                        address = TernaryAdd(address, D2T_conversion(1, true), true);
                                        break;
                                case cmp:
                                        mem_write(mem, address, cmp);
                                        address = TernaryAdd(address, D2T_conversion(1, true), true);
                                        break;
                                case max:
                                        mem_write(mem, address, max);
                                        address = TernaryAdd(address, D2T_conversion(1, true), true);
                                        break;
                                case min:
                                        mem_write(mem, address, min);
                                        address = TernaryAdd(address, D2T_conversion(1, true), true);
                                        break;
                                case skip:
                                        mem_write(mem, address, skip);
                                        address = TernaryAdd(address, D2T_conversion(1, true), true);
                                        break;
                                case halt:
                                        mem_write(mem, address, halt);
                                        address = TernaryAdd(address, D2T_conversion(1, true), true);
                                        break;                        
                                default:
                                        printf("Error Program Loader, unable to store instruction into memory");
                                        break;
                        }
                }
        }
}
