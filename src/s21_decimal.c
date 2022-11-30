#include "s21_decimal.h"

const s21_decimal s21_zero = {0};

union test
{
  float asFloat;
  long int asInteger;
};

int main(){
    union test qwe;
    qwe.asFloat = 2.509; 
    int mantissa = 1;
    int n = 0;
    // while(mantissa != 0){
    //     mantissa = qwe.asInteger & 0x7FFFFF;
    //     qwe.asFloat*=10;
    //     n++;
    //     printf("%d\n",mantissa);
    // }
    printf("|%f|\n",log10(pow(2,96))+1);
    return 0;
}

int s21_from_int_to_decimal(int src, s21_decimal *dst){
    *dst = s21_zero;
    if(src < 0) {
        src *= -1;
        dst->bits[3] = 2147483648;
    }
    dst->bits[0] = src;
    return 0;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst){
    *dst = s21_zero;
    int result = 0;
    if(isinf(src) || isnan(src)) result = 1;// is src infinity
    if((0 < src) && (src < 1e-28)) result = 1;
    if(result == 0){
        int n = intpart(src);
        while (n <= 29)
        {
            
        }
        
    }
    int intermidNumber = 0;
    int scale = 0;
    if(src < 0) {
        
    }
    return 0;
}
