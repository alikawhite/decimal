#include "s21_decimal.h"

const s21_decimal s21_zero = {0};

union test
{
  float asFloat;
  long int asInteger;
};

int main(){
    s21_decimal dst;
    float src = 0.0001;
    s21_from_float_to_decimal(src, &dst);
    

    return 0;

}

int s21_from_int_to_decimal(int src, s21_decimal *dst){
    *dst = s21_zero;
    if(src < 0) {
        src *= -1;
        setSign(dst); // 0b10000000000000000000000000000000
    }
    dst->bits[0] = src;
    return 0;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
    *dst = s21_zero;
    int err = 0;
    int scale = 0;
    if (src < 0) {
        src *= -1;
        setSign(dst);
    }
    if (src > 1e-28 && src < 1) { // проверка дст
        int exp = getExp(src);
        if (exp > 95 || exp < -95) {
            err = 1;
        } else {
            for (; !((int)src); src *= 10) {
                printf("\n%f     %d", src, scale);
                scale++;
                // if ()
            }
            //  пофиксить ошибку с единицей
            if (scale < )
        }
    } else {
        err = 1;
    }
    printf(" \n%.10f.   %d ", src, scale);
    return err;
}


// //  устанавливает минус
int setSign(s21_decimal *dst) {
    dst->bits[3] |= 0x80000000;  //логическое или 
    return 1;
}
 // получает знак
int getSign(s21_decimal dst) { return dst.bits[3] & 0x80000000 ? 1 : 0;}

int getBit(s21_decimal d, int i) {
    unsigned int mask = 1u << (i % 32);
    int r = 0;
    if (i <= 95 && i >= 0) {
        r = d.bits[i / 32] & mask;
    }
    return r;
}

int getExp(float src) {
    unsigned int fbits = *((unsigned int *) & src);
    int res = 0;
    fbits <<= 1; // убираем знак
    res = res | (fbits >> 24); // убираем мантиссу 
    return res - 127;
}
