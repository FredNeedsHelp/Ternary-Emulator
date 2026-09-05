#include "scrappy_striker.h"

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

int T2D_int12(int12 TernNumber) //Ternary to Decimal Converter
{
        int result = 0;
        int pw = 1;

        for(int i = 0; i < 12; i++)
                {
                        result += TernNumber[i] * pw;
                        pw *= 3;
                }
        return result;
}

int T2D_int9(int9 TernNumber) //Ternary to Decimal Converter
{
        int result = 0;
        int pw = 1;

        for(int i = 0; i < 9; i++)
                {
                        result += TernNumber[i] * pw;
                        pw *= 3;
                }
        return result;
}

void TernaryAdd_int9(int9 X, int9 Y, int9 sum) //Both X and Y have to be the same int type
{
        trit carry = {0};

        for(int i = 0; i < 9; i++)
        {        
                int temp_sum = X[i] + Y[i] + carry;
                
                switch (temp_sum)
                {
                        case -3:
                                sum[i] = 0;
                                carry = -1;
                                break;
                        case -2:
                                sum[i] = 1;
                                carry = -1;
                                break;

                        case -1:
                                sum[i] = -1;
                                carry = 0;
                                break;

                        case 0:
                                sum[i] = 0;
                                carry = 0;
                                break;

                        case 1:
                                sum[i] = 1;
                                carry = 0;
                                break;

                        case 2:
                                sum[i] = -1;
                                carry = 1;
                                break;
                        case 3:
                                sum[i] = 0;
                                carry = 1;
                                break;
                }
        
        }
        return;
}

void TernaryAdd_int12(int12 X, int12 Y, int12 sum) //Both X and Y have to be the same int type
{
        trit carry = {0};

        for(int i = 0; i < 12; i++)
        {        
                int temp_sum = X[i] + Y[i] + carry;
                
                switch (temp_sum)
                {
                        case -3:
                                sum[i] = 0;
                                carry = -1;
                                break;
                        case -2:
                                sum[i] = 1;
                                carry = -1;
                                break;

                        case -1:
                                sum[i] = -1;
                                carry = 0;
                                break;

                        case 0:
                                sum[i] = 0;
                                carry = 0;
                                break;

                        case 1:
                                sum[i] = 1;
                                carry = 0;
                                break;

                        case 2:
                                sum[i] = -1;
                                carry = 1;
                                break;
                        case 3:
                                sum[i] = 0;
                                carry = 1;
                                break;
                }
        
        }
        return;
}

void TernarySub_int12(int12 X, int12 Y, int12 sum)  //Both X and Y have to be the same int type
{
        int12 Z; memcpy(Z, Y, sizeof(int12));
        flip_int12(Z);
        TernaryAdd_int12(X, Z, sum);
}

void TernarySub_int9(int9 X, int9 Y, int9 sum)  //Both X and Y have to be the same int type
{
        int9 Z; memcpy(Z, Y, sizeof(int9));
        flip_int9(Z);
        TernaryAdd_int9(X, Z, sum);
}

void flip_int9(int9 X)
{
        int9 temp_x;
        memset(temp_x, 0, sizeof(int9));

        for(int i = 0; i < 9; i++)
        {
                switch (X[i])
                {
                        case -1:
                                temp_x[i] = 1;
                                break;
                        case 0:
                                temp_x[i] = 0;
                                break;
                        case 1:
                                temp_x[i] = -1;
                                break;
                }
        }

        memcpy(X, temp_x, sizeof(int9));
        return;
}

void flip_int12(int12 X)
{
        int12 temp_x;
        memset(temp_x, 0, sizeof(int12));

        for(int i = 0; i < 12; i++)
        {
                switch (X[i])
                {
                        case -1:
                                temp_x[i] = 1;
                                break;
                        case 0:
                                temp_x[i] = 0;
                                break;
                        case 1:
                                temp_x[i] = -1;
                                break;
                }
        }

        memcpy(X, temp_x, sizeof(int12));
        return;
}

//Returns the sign of the Ternary int12 number
trit signis_int12(int12 x) 
{
        for(int i = 11; i >= 0; i--)
        {
                if(x[i] != 0)
                {
                        return x[i];
                }
        }

        return net;
}

//returns int9
trit signis_int9(int9 x) 
{
        for(int i = 8; i >= 0; i--)
        {
                if(x[i] != 0)
                {
                        return x[i];
                }
        }

        return net;
}

//Checks if the ternary number is zero
trit is_zero_int12(int12 x) 
{
        for(int i = 0; i < 12; i++)
        {
                if(x[i] != 0) return neg;
        }

        return pos;
}

//Checks if the ternary number is zero
trit is_zero_int9(int9 x) 
{
        for(int i = 0; i < 9; i++)
        {
                if(x[i] != 0) return neg;
        }

        return pos;
}

//Strips the sign of the Ternary Number.
void abs_int12(int12 x)
{
        if(signis_int12(x) == neg)
        {
               flip_int12(x);
               return; 
        }

        return; 
}

void abs_int9(int9 x)
{
        if(signis_int9(x) == neg)
        {
               flip_int9(x);
               return; 
        }

        return; 
}

void C2T_conversion(char number, char_t rs) //Char to ternary conversion
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
                                case -1:
                                        rs[i] = -1;
                                        break;
                                case -2:
                                        rs[i] = 1;
                                        q--;
                                        break;

                                default:
                                        Throw("Error converting to Trits: C2T");
                                        break;
                        }

                number = q;
                i++;
        }

        return;
}

void D2T_int9(int8_t number, int9 rs) //Decimal to ternary conversion
{
        memset(rs, 0, sizeof(int9));
        int i = 0;

        while(number != 0)
        {
                if(i == 9) {break;} else if(i > 9) {Throw("Attempted to overflow; D2T");}

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
                                case -1:
                                        rs[i] = -1;
                                        break;
                                case -2:
                                        rs[i] = 1;
                                        q--;
                                        break;
                                default:
                                        Throw("Error converting to Trits: D2T_int9");
                                        break;
                        }
                

                number = q;
                i++;
        }
        return;
}

void D2T_int12(int8_t number, int12 rs) //Decimal to ternary conversion
{
        memset(rs, 0, sizeof(int12));
        int i = 0;

        while(number != 0)
        {
                if(i == 12) {break;} else if(i > 12) {Throw("Attempted to overflow; D2T");}

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
                                case -1:
                                        rs[i] = -1;
                                        break;
                                case -2:
                                        rs[i] = 1;
                                        q--;
                                        break;
                                default:
                                        Throw("Error converting to Trits: D2T_int12");
                                        break;
                        }
                

                number = q;
                i++;
        }
        return;
}


//by par. is int9 and bool int12 only regards of ternary_number. 
//left bool is just the direction of the shift
void shift_int12(int12 *ternary_number, int12 by, bool left)
{
        int by_amount = T2D_int12(by);

        if(left)
        {
                        for(int i = 0; i < by_amount; i++)
                        {
                                for(int x = 11; x >= 0; x--)
                                {
                                   (*ternary_number)[x] = (*ternary_number)[x - 1];  
                                }

                                (*ternary_number)[i] = net;
                        }
                
        }
        else if(!left)
        {
                        for(int i = 0; i < by_amount; i++)
                        {
                                for(int x = 0; x <= 11; x++)
                                {
                                   (*ternary_number)[x] = (*ternary_number)[x + 1];  
                                }

                                (*ternary_number)[11 - i] = net;
                        }
        }
        return;
}

void shift_int9(int9 *ternary_number, int9 by, bool left)
{
        int by_amount = T2D_int9(by);

        if(left)
        {
                        for(int i = 0; i < by_amount; i++)
                        {
                                for(int x = 8; x >= 0; x--)
                                {
                                   (*ternary_number)[x] = (*ternary_number)[x - 1];  
                                }

                                (*ternary_number)[i] = net;
                        }
                
        }
        else if(!left)
        {
                        for(int i = 0; i < by_amount; i++)
                        {
                                for(int x = 0; x <= 8; x++)
                                {
                                   (*ternary_number)[x] = (*ternary_number)[x - 1];  
                                }

                                (*ternary_number)[8 - i] = net;
                        }
        }
        return;
}

void mlp_int12(int12 x, int12 y, int12 *product)
{
        if(T2D_int12(x) == 0 || T2D_int12(y) == 0) {memset(product, 0, sizeof(int12)); return;}

        int12 temp_1 = {0}, temp_prod = {0}, temp_x = {0}, temp_y = {0};
        memcpy(temp_x, x, sizeof(int12));
        memcpy(temp_y, y, sizeof(int12)); 
        D2T_int12(1, temp_1);

        while(is_zero_int12(temp_x) != pos)
        {
                if(temp_x[0] == 1) TernaryAdd_int12(temp_prod, temp_y, temp_prod);
                else if(temp_x[0] == -1) TernarySub_int12(temp_prod, temp_y, temp_prod);
                        
                shift_int12(&temp_y, temp_1, true);
                shift_int12(&temp_x, temp_1, false);
        }

        memcpy(product, temp_prod, sizeof(int12));
}

void mlp_int9(int9 x, int9 y, int9 *product)
{
        if(T2D_int9(x) == 0 || T2D_int9(y) == 0) {memset(product, 0, sizeof(int9)); return;}

        int9 temp_1 = {0}, temp_prod = {0}, temp_x = {0}, temp_y = {0};
        memcpy(temp_x, x, sizeof(int9));
        memcpy(temp_y, y, sizeof(int9)); 
        D2T_int9(1, temp_1);

        while(is_zero_int9(temp_x) != pos)
        {
                if(temp_x[0] == 1) TernaryAdd_int9(temp_prod, temp_y, temp_prod);
                else if(temp_x[0] == -1) TernarySub_int9(temp_prod, temp_y, temp_prod);
                        
                shift_int9(&temp_y, temp_1, true);
                shift_int9(&temp_x, temp_1, false);
        }

        memcpy(product, temp_prod, sizeof(int9));
}

void dvd_int12(int12 x, int12 y, int12 *rs)
{
        //Temp. Cheap Trick.
        uint8_t sum = T2D_int12(x) / T2D_int12(y);
        int12 tmp_rs; D2T_int12(sum, tmp_rs);
        memcpy(rs, tmp_rs, sizeof(int12));
        return;

        //CANT FIGURE IT OUT!!!
        /*
        if(is_zero_int12(y) == pos) {Throw("Attempted to Divide by Zero; INFINITY");}
        if(is_zero_int12(x) == pos) {memset(rs, 0, sizeof(int12)); printf("Dvd enumarator is zero"); return;}

        int12 temp, ternary_i, temp_x, temp_y, q;
        memset(temp, 0, sizeof(int12));
        memcpy(temp_x, x, sizeof(int12));
        memcpy(temp_y, y, sizeof(int12));

        abs_int12(temp_x);
        abs_int12(temp_y);

        D2T_int12(1, ternary_i);

        for(int i = 11; i >= 0; i--)
        {       
                shift_int12(&temp, ternary_i, true);

                temp[0] = temp_x[i];

                if(T2D_int12(temp) >= T2D_int12(temp_y))
                {
                        TernarySub_int12(temp, temp_y, temp);
                        q[i] = pos;
                        
                } 
                else if(T2D_int12(temp) <= -T2D_int12(temp_y))
                {
                        TernaryAdd_int12(temp, temp_y, temp);
                        q[i] = neg;
                }
                else 
                {
                        q[i] = net;
                }
        }

        if((signis_int12(x) == neg) && (signis_int12(y) == neg)) {memcpy(rs, q, sizeof(int12)); return;}
        else if((signis_int12(x) == neg) || (signis_int12(y) == neg)) {flip_int12(q); memcpy(rs, q, sizeof(int12)); return;}
        else {memcpy(rs, q, sizeof(int12)); return;}
        */
}

void dvd_int9(int9 x, int9 y, int9 *rs)
{
        //Still cant figure it out.

        //Temp. Cheap Trick.
        uint8_t sum = T2D_int9(x) / T2D_int9(y);
        int9 tmp_rs; D2T_int9(sum, tmp_rs);
        memcpy(rs, tmp_rs, sizeof(int9));
        return;
}