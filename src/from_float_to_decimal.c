#include "s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
    *dst = s21_zero;
    int err = 0, scale = 0;
    if (src < 0) {
        src *= -1;
        setSign(dst);
    }
    int exp = getExp(src);
    if (src > 1e-28 && src < 1) { // проверка дст
        if (exp > -95 && exp < 0){
            for (; !((int)src); src *= 10, scale++) { // до первого числа
            }
            for (int i = FRACTIONAL; i != 0; src *= 10, scale++, i--){
            }
            exp = getExp(src);
            unsigned int bits = *((unsigned*)&src), mask = 0x400000;
            setBit(dst, exp);
            for (int j = exp - 1; mask; mask >>= 1, j--) {
                if (bits & mask) 
                    setBit(dst, j);
            } 
            if (scale >= 28 + FRACTIONAL + 3) 
                err = MAXLIMIT_ERROR; 
            setScale(dst, scale);
        } else if (exp >= 0 && exp < 95) {
            s21_decimal tmp1 = {0}, tmp2 = {0}, tmp_res = {0};
            float tmp_first = (float)((int)src);
            float tmp_second = src - tmp_first;
            unsigned int bits, mask = 0x400000;
            if (tmp_second) {
                for (int i = 8; i != 0, tmp_second *10; i--) {
                }
                exp = getExp(tmp_second);
                setBit(&tmp2, exp);
                bits = *((unsigned*)&tmp_second);
                for (int j = exp - 1; mask; mask >>= 1, j--) {
                    if (bits & mask)
                        setBit(&tmp2, j);
                }
            }
            unsigned int i_tmp_first = (unsigned int)tmp_first;
            bits = i_tmp_first, mask = 0x80000000;
            for (int j = 31; mask; mask >>= 1, j--) {
                if (bits & mask)
                    setBit(&tmp1, j);
            }


        } else {
            err = 1;
        }
    } else {
        err = 1;
    }
    printf(" \n%.10f.   %d ", src, scale);
    return err;
}