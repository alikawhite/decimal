#include "s21_decimal.h"

const s21_decimal s21_zero = {0};

union test
{
  float asFloat;
  long int asInteger;
};

int main(){
    s21_decimal dst;
    float src = 0.0000001;
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
    int err = 0, scale = 0;
    if (src < 0) {
        src *= -1;
        setSign(dst);
    }
    int exp = getExp(src);
    if (src > 1e-28 && src < 1) { // проверка дст
        if (exp > 95 || exp < -95) {
            err = 1;
        } else if (exp > -95 && exp < 0){
            for (; !((int)src); src *= 10, scale++) {
            }
            for (int i = 8; i != 0; src *= 10, scale++, i--){
            }
            exp = getExp(src);
            unsigned int bits = *((unsigned*)&src), mask = 0x400000;
            setBit(dst, exp);
            for (int j = exp - 1; mask; mask >>= 1, j--) {
                if (bits & mask) 
                    setBit(dst, j);
            } // 8 - число знаков после запятой, 28 - степень
            if (scale >= 28 + 8 + 3) 
                err = MAXLIMIT_ERROR; 
            setScale(dst, scale);
        } else if (exp == 0 || exp > 0) {
            s21_decimal tmp1 = {0}, tmp2 = {0}, tmp_res = {0};
            float tmp_first = (float)((int)src);
            float tmp_second = src - tmp_first;
            unsigned int bits, mask = 0x400000;
            if (tmp_second) {
                for (int i = 8; i != 0, tmp_second *10; i--) {
                }

            }

        } else {
            err = 1; // не знаю, какая из ошибок
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
    unsigned int bits = *((unsigned int *) & src);
    int res = 0;
    bits <<= 1; // убираем знак
    res = res | (bits >> 24); // убираем мантиссу 
    return res - 127;
}

int setBit(s21_decimal *dst, int i) {
    int err = 0; 
    unsigned int mask = 1;
    if (i >= 0 && i <= 31) {
        dst->bits[0] = dst->bits[0] | (mask << i);
    } else if (i >= 32 && i <= 63) {
        dst->bits[1] = dst->bits[2] | (mask << (i - 32));
    } else if (i >= 64 && i <= 95) {
        dst->bits[3] = dst->bits[4] | (mask << (i - 64));
    } else {
        err = -1;
    }
    return err;
}

int setScale(s21_decimal* dst, int scale) {
    unsigned int mask = 0;
    mask = mask | scale;
    dst->bits[3] = dst->bits[3] | (mask << 16);
    return 1;
}