//Scrappy Striker Program Loader (SSPL)
#include "scrappy_striker.h"

FILE *tasm_file = NULL;

const char *LK_code[] = 
{ 
        "quit",
        "add",  
        "sub",  
        "jump",
        "load", 
        "store",
        "move", 
        "shr",
        "shl",
        "cmp", 
        "max", 
        "min",
        "flip",
        "set",
        "mlp", 
        "dvd",
        "noop", 
        "CYC" //At the moment CYC is the 17th element of the array which does not line-up with the op_code.
};


op_codes LookUpTable(char *token, LabelInfo Labels_Info)
{     
        if(token == NULL || token[0] == '\0') return skip;

        if(token[0] == ';') return skip;

        if(strchr(token, ':') != NULL) return label;

        //Add Check if its a label.
        for(int i = 0; i < MAX_LABELS; i++)
        {
                if(strcmp(token, Labels_Info.Name[i]) == 0)
                {
                        return label;
                }
        }                       

        for(int i = 0; i < 18; i++) //change to 26 later, when the TISA done.
        {
                if(token[0] == ';') return skip;
                op_codes current_code = (op_codes)i;
                if(LK_code[current_code] == NULL) continue;

                if(strcmp(token, LK_code[current_code]) == 0)
                {
                        if(current_code == 17) return cyc_def;
                        else return current_code;
                }
        }

        return Unknown_Halt;
}

int Parse4Reg(char *buffer)
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
        if(buffer == NULL) return INT_MIN; //Safety issue, passing a NULL string is expected behaviour.
        for(int i = 0; buffer[i] != '\0'; i++)
        {
                trit sign = net;

                if(!isdigit(buffer[i]) || ( i > 0 && isalpha(buffer[i - 1]))) {continue;}
                if((ispunct(buffer[i - 1]) && i > 0) != 0) {sign = neg;}
                else {sign = pos;}

                int numb = 0;

                while(isdigit(buffer[i]))
                {
                        numb = numb * 10 + (buffer[i] - '0');
                        i++;
                }

                if(!isalpha(buffer[i])) 
                {
                        if(sign == pos) return numb;
                        else if(sign == neg) return -numb;
                        else printf("An Error has Occured; Parse 4 Number returned no / net sign");                   
                } 
        }

        return INT_MIN; //returns the min of a 32 bit int (C-standard, I think). very unlikely this function will return negative 2 billion.
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

//Label OR Number to Memory
void LorN2Mem(memory *mem, char *buffer, int12 *address, LabelInfo *LI)
{
        if(Parse4Number(buffer) != INT_MIN)
        {
                Number2Mem(mem, buffer, address);
        }
        else
        {
                char *LabelName = strtok(buffer, ":");
                for(int i = 0; i < 10; i++)
                {
                        if(LabelName == NULL || LI->Name[i] == NULL) continue;
                        if(strcmp(LabelName, LI->Name[i]) == 0)
                        {
                                char_t temp_dat;
                                memcpy(temp_dat, LI->MemoryAddress[i], sizeof(char_t));
                                WriteMemTASM(mem, address, &temp_dat);
                        }
                }          
        }
}

trit State2Flag(memory *mem, char *buffer)
{       
        if(strstr(buffer, "NEG") || strstr(buffer, "neg")) return neg;
        if(strstr(buffer, "POS") || strstr(buffer, "pos")) return pos;
        if(strstr(buffer, "NET") || strstr(buffer, "net")) return net;
        return halt; //Cant use unknown error nor unknown halt, they both use -1 aka VALID VALUE :sob:
}

int InstructionWeight(op_codes OPERATION)
{
        switch(OPERATION)
        {
                case shr:
                case shl:
                case store:
                case load:
                case set:
                case jmp:
                        return 3;

                case add:
                case sub:
                case mlp:
                case dvd:
                case move:
                case flp:
                case cmp:
                case max:
                case min:
                case quit:
                        return 2;

                case halt: return 1;

                default:
                        return 0;
                        
        }
}

//Pre-ProgramLoader Label Maker, formally known as the Pre-Assembler.
results PPL_LabelMaker(char *tasm_name, char *buffer, LabelInfo *Labels_Inf)
{
        int12 pre_address = tern_int_zero; 

        FILE *TASM_FILE_TEMP;
        TASM_FILE_TEMP = fopen(tasm_name, "r"); //I forgot if any functions using the FILE mutates, so i just made a temp. copy

        char buf_cpy[243];
        LabelInfo LI_EMPTY = {0};

        int i = 0;
        while(fgets(buffer, 243, TASM_FILE_TEMP))
        {
                strcpy(buf_cpy, buffer);
                char *LabelName = strchr(buffer, ':');
                char *OP;

                if(LabelName != NULL && i < MAX_LABELS)
                {         
                        //A char is 1 single byte by standard, but for portability I will multiply it by a size of a single char.
                        *LabelName = '\0';            
                        memcpy(Labels_Inf->Name[i], buffer, sizeof(char) * 243);
                        memcpy(Labels_Inf->MemoryAddress[i], pre_address, sizeof(int12));
                        i++;
                } 
                else
                {
                        OP = strtok(buf_cpy, " \t\r\n");
                        op_codes inst = LookUpTable(OP, LI_EMPTY);      

                        int12 rs = {0};
                        D2T_int12(InstructionWeight(inst), rs);
                        TernaryAdd_int12(pre_address, rs, pre_address);
                }

        }

        return success;
}

results ProgramLoader(memory *mem, char *tasm_name, LabelInfo *Labels_Info)
{
        if(!(tasm_file = fopen(tasm_name, "r"))) {Throw("No Input File detected");} //add input for the file!
         
        char buffer[243]; //line size limit, 3^5 = 243
        int12 address = tern_int_zero;

        //Identifies Labels in the code and assigns it a memory value.
        if(PPL_LabelMaker(tasm_name, buffer, Labels_Info) != success) Throw("Pre-Assembler failed");
        
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
                        op_codes inst = LookUpTable(op, *Labels_Info);

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

                                case mlp:
                                        C2T_conversion(mlp, code);
                                        WriteMemTASM(mem, &address, &code);
                                        RegEncoder(mem, numb, regs, &address);
                                        break;

                                case dvd:
                                        C2T_conversion(dvd, code);
                                        WriteMemTASM(mem, &address, &code);
                                        RegEncoder(mem, numb, regs, &address);
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
                                        //Add if statement to check if its a memory address or label.
                                        LorN2Mem(mem, regs, &address, Labels_Info);
                                        //Number2Mem(mem, regs, &address);
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

                                case label:
                                        op = NULL;
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
                                        Throw("Error Program Loader, unable to store instruction into memory: %d\n", inst);
                                        break;
                        }

                        op = strtok(NULL, " \t\r\n");
                }
        }

        return success;
}
