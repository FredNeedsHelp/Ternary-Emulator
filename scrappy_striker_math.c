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
                                default:
                                        Throw("Error converting to Trits");
                                        break;
                        }

                number = q;
                i++;
        }

        return;
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


//by par. is int9 and bool int12 only regards of ternary_number. 
//left bool is just the direction of the shift
void shift(int_t *ternary_number, int_t by, bool left, bool int12)
{
        int by_amount = T2D_converter(by, false);

        if(left)
        {
                if(int12)
                {
                        for(int i = 0; i < by_amount; i++)
                        {
                                for(int x = 11; x > 0; x--)
                                {
                                   ternary_number->int12[x] = ternary_number->int12[x - 1];  
                                }

                                ternary_number->int12[i] = net;
                        }
                }
                else if(!int12)
                {    
                        for(int i = 0; i < by_amount; i++)
                        {
                                for(int x = 8; x > 0; x--)
                                {
                                   ternary_number->int9[x] = ternary_number->int9[x - 1];  
                                }

                                ternary_number->int9[i] = net;
                        }
                }
        }
        else if(!left)
        {
                if(int12)
                {
                        for(int i = 0; i < by_amount; i++)
                        {
                                for(int x = 0; x < 11; x++)
                                {
                                   ternary_number->int12[x] = ternary_number->int12[x + 1];  
                                }

                                ternary_number->int12[11 - i] = net;
                        }
                }
                else if(!int12)
                {    
                        for(int i = 0; i < by_amount; i++)
                        {
                                for(int x = 0; x < 8; x++)
                                {
                                   ternary_number->int9[x] = ternary_number->int9[x + 1];  
                                }

                                ternary_number->int9[8 - i] = net;
                        }
                }
        }
        
        double x = T2D_converter(*ternary_number, int12);
        if(x != (long)x) printf("YOUR SHIFT HAS RESULTED IN A FLOATING-POINT. NOT IMPLEMENTED YET!!");
}