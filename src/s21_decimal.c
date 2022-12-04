#include "s21_decimal.h"

const s21_decimal s21_zero = {0};

union test
{
  float asFloat;
  long int asInteger;
};

int main(){

    s21_decimal number;
  // decimal: 2.0
  // float: 2
  // int: 1073741824
  number.bits[0] = 0b00000000000000000000000000010100;
  number.bits[1] = 0b00000000000000000000000000000000;
  number.bits[2] = 0b00000000000000000000000000000000;
    

    return 0;

}

int s21_from_int_to_decimal(int src, s21_decimal *dst){
    *dst = s21_zero;
    if(src < 0) {
        src *= -1;
        setSign(dst); 
    }
    dst->bits[0] = src;
    return 0;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) { // посмотреть, что ретернить
    int err = 0, tmp = 0;
    if (getScale(src) != 0) {
        err = 1;
    } else if (src.bits[1] == NULL && src.bits[2] == NULL && src.bits[0] != NULL) { // проверка на валидность децимала
        for (int i = 0; i < 32; i++) {
            tmp *= 2;
            if (getBit(src, 31 - i))
                tmp += 1;
        }
        if (getSign(src))
            tmp *= -1;
    }
    *dst = tmp;
    return err;
}

int s21_from_decimal_to_float(s21_decimal src, float* dst) {
    double tmp = 0;
    src = s21_zero;
    for (int i = 0; i < 96; i++) {
        tmp *= 2;
        if (getBit(src, 95 - i))
            tmp += 1;
    }
    for (int j = getScale(src); j != 0; tmp /= 10, j--) {
    }
    if (getSign(src))
        tmp *= -1;
    *dst = tmp;
    return 0;
}

int setSign(s21_decimal *dst) {
    dst->bits[3] |= 0x80000000;
    return 1;
}

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
    bits <<= 1;
    res = res | (bits >> 24);
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

int getScale(s21_decimal src) {
  src.bits[3] <<= 8;
  unsigned int res = src.bits[3] >> 24;
  return res;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
    int err = 0;
    if (getScale(value) > 28) {
        err = 1;
    }
    else {
        if (!getSign(value)) {
            setSign(&value);
        } else {
            value.bits[3] &= 0b01111111111111111111111111111111;
        }
        *result = value;
    }
    return err;
}

void big_decimal(s21_decimal value, s21_big_decimal *result) {
    for (int i = 0; i < 3; i++) {
        result->bits[i] = value.bits[i];
    }
    for (int i = 3; i < 8; i++) {
        result->bits[i] = 0;
        if (i == 7)
            result->bits[i] = value.bits[3];
    }
} 

int big_getScale(s21_big_decimal src) {
  src.bits[7] <<= 8;
  unsigned int res = src.bits[7] >> 24;
  return res;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
    int err = 0;
    int scale = getScale(value);
    if (scale > 28) {
        err = 1;
    } else if (scale) {
        s21_big_decimal bd;
        big_decimal(value, &bd);
        s21_big_decimal ten = {{10, 0, 0, 0, 0, 0, 0}};
        while (scale) {
            s21_big_decimal tmp = {0};

            bd = tmp;
            scale--;
        }
    }
    return err;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int err = 0;

    return err;
}

void big_add(s21_big_decimal a, s21_big_decimal b, s21_big_decimal *result) {
    int tmp = 0;
    for (int i = 0; i < 256; i++) {
        if (get_bigBit(a, i) && get_bigBit(b, i) && tmp == 1) {
            tmp = 1;
            set_bigBit(result, i);
        } else if (get_bigBit(a, i) && get_bigBit(b, i) && tmp == 0) {
            tmp = 1;
        } else if ((get_bigBit(a, i) || get_bigBit(b, i)) && tmp == 1) {
            tmp = 1;
        } else if ((get_bigBit(a, i) || get_bigBit(b, i)) && tmp == 0) {
            big_setBit(result, i);
        } else if ((!get_bigBit(a, i) && !get_bigBit(b, i)) && tmp == 1) {
            big_setBit(result, i);
            tmp = 0;
        }
    }
}

int big_setBit(s21_big_decimal* d, int i) {
    int err = 0;
    unsigned int mask = 1;
    int val = i / 32;
    d->bits[val] = d->bits[val] | (mask << (i - (32 * val)));
    return err;
}

