#include "s21_decimal.h"

// убрать отсюда нахуй эту константу, чтобы не доебались 

const s21_decimal s21_zero = {0};

union test
{
  float asFloat;
  long int asInteger;
};

int main(){
    // s21_decimal number;
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
    } else if (1 == 1) { // проверка не работает!!!!
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

void setSign(s21_decimal *dst) { dst->bits[3] |= 0x80000000; }

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
        dst->bits[1] = dst->bits[1] | (mask << (i - 32));
    } else if (i >= 64 && i <= 95) {
        dst->bits[2] = dst->bits[2] | (mask << (i - 64));
    } else {
        err = -1;
    }
    return err;
}

void setScale(s21_decimal* dst, int scale) {
    unsigned int mask = 0;
    mask = mask | scale;
    dst->bits[3] = dst->bits[3] | (mask << 16);
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
    for (int i = 3; i < 7; i++) {
        result->bits[i] = 0;
        if (i == 6)
            result->bits[i] = value.bits[3];
    }
} 

int big_getScale(s21_big_decimal src) {
  src.bits[6] <<= 8;
  unsigned int res = src.bits[6] >> 24;
  return res;
}

 // don't ready
// int s21_truncate(s21_decimal value, s21_decimal *result) {
//     int err = 0;
//     int scale = getScale(value);
//     if (scale > 28) {
//         err = 1;
//     } else if (scale) {
//         s21_big_decimal bd;
//         big_decimal(value, &bd);
//         s21_big_decimal ten = {{10, 0, 0, 0, 0, 0, 0}};
//         while (scale) {
//             s21_big_decimal tmp = {0};

//             bd = tmp;
//             scale--;
//         }
//     }
//     return err;
// }

//  // don't ready
// void to_one_scale(s21_decimal *value1, s21_decimal *value2, s21_big_decimal* first, s21_big_decimal *second) {
//     s21_big_decimal result = {0};
//     s21_big_decimal tmp1 = {0};
//     s21_big_decimal tmp2 = {0};

//     big_decimal(*value1, &tmp1);
//     big_decimal(*value2, &tmp2);
//     if (getScale(*value1) < getScale(*value2)) {

//     }

    
// }

 // don't ready
// void scale_up(s21_big_decimal *dst, int value, s21_big_decimal *result) {
//     s21_big_decimal m = {{10, 0, 0, 0, 0, 0, 0}};
//     s21_big_decimal tmp_result = {0};
//     int sign = big_getSign(*dst);
//     int scale = big_getScale(*dst);
//     for (int i = 0; i < value; i++) {
//         big_mult(*dst, m, &tmp_result);
//         *dst = tmp_result;
//         to_zero(&tmp_result);
//     }
//     *result = *dst;
//     big_cleanScale(result);
//     big_setScale(result, scale + value);
//     if (sign)
//         big_setSign(result);
// }

void big_setScale(s21_big_decimal* dst, int scale) {
    unsigned int mask = 0;
    mask = mask | scale;
    dst->bits[6] = dst->bits[6] | (mask << 16);
}

void big_cleanScale(s21_big_decimal* dst) {
  unsigned int mask = 0x10000000;
  dst->bits[6] = dst->bits[6] & mask;
}

void to_zero(s21_big_decimal* dst) {
  for (int i = 0; i < 7; i++) {
    dst->bits[i] = 0;
  }
}

void big_setSign(s21_big_decimal *dst) { dst->bits[6] |= 0x80000000; }


void big_mult(s21_big_decimal a, s21_big_decimal b, s21_big_decimal *result) {
    s21_big_decimal tmp_a = a;
    s21_big_decimal tmp = {0};
    *result = tmp;
    for (int i = 0; i < 192; i++) {
        if (big_getBit(b, i)) {
            s21_big_decimal tmp_res = {0};
            big_add(tmp_a, *result, &tmp_res);
            *result = tmp_res;
        }
        big_shift(&tmp_a, 1);
    }
    
}

void big_shift(s21_big_decimal *dst, int opt) {
    if (opt == 1) {
        for (int i = 191; i != 0; i--) {
            if (big_getBit(*dst, i - 1)) {
                big_setBit(dst, i);
            } else {
                big_cleanBit(dst, i);
            }
        }
        big_cleanBit(dst, 191);
    }
}

void big_cleanBit(s21_big_decimal* dst, int i) {
  unsigned int mask = 1;
  int value = i / 32;
  dst->bits[value] = dst->bits[value] & (~(mask << (i - (32 * value))));
}

void big_add(s21_big_decimal a, s21_big_decimal b, s21_big_decimal *result) {
    int tmp = 0;
    s21_big_decimal zero = {0};
    *result = zero;
    for (int i = 0; i < 192; i++) {
        if (big_getBit(a, i) && big_getBit(b, i) && tmp == 1) {
            tmp = 1;
            big_setBit(result, i);
        } else if (big_getBit(a, i) && big_getBit(b, i) && tmp == 0) {
            tmp = 1;
        } else if ((big_getBit(a, i) || big_getBit(b, i)) && tmp == 1) {
            tmp = 1;
        } else if ((big_getBit(a, i) || big_getBit(b, i)) && tmp == 0) {
            big_setBit(result, i);
        } else if ((!big_getBit(a, i) && !big_getBit(b, i)) && tmp == 1) {
            big_setBit(result, i);
            tmp = 0;
        }
    }
}

 // don't ready
// s21_decimal divTen(s21_decimal src) {
//     s21_big_decimal src_long;
//     s21_big_decimal tmp;
//     s21_big_decimal ten = {{10, 0, 0, 0, 0, 0, 0}};
//     big_decimal(src, &src_long);
//     unsigned int scale = getScale(src);
//     if (scale > 0)
//         scale--;
//     int sign = getSign(src);
//     if (!sign) {
//         src.bits[3] = (scale << 16) | 0x80000000;
//     } else {
//         src.bits[3] = scale << 16;
//     }
//     src_long = div_big_end(src_long, ten, &tmp);
//     src.bits[0] = src_long.bits[0];
//     src.bits[1] = src_long.bits[1];
//     src.bits[2] = src_long.bits[2];
//     return src;
// }

 // don't ready
// s21_big_decimal div_big_end(s21_big_decimal value, s21_big_decimal divider, s21_big_decimal *reminder) {
//     s21_big_decimal res ={{0, 0, 0, 0, 0, 0, 0}};
//     // s21_big_decimal two_long = {{2, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
//     // s21_big_decimal dividend = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}; 
//     for (int i = 7 - 1; i >= 0; i--) {
//         for (int j = 0; j < 32; j++) {
//             unsigned int tmp = value.bits[i];
//             tmp = tmp << j >> 31;
//             // dividend = 
//         }
//     }
//     return res;
// }

int big_getBit(s21_big_decimal dst, int i) {
    int res = 0;
    unsigned int mask = 1;
    int value = i / 32;
    if (dst.bits[value] && (mask << ( i - (32 * value)))) {
        res = 1;
    }
    return res;
}

int big_setBit(s21_big_decimal* dst, int i) {
    int err = 0;
    unsigned int mask = 1;
    int value = i / 32;
    dst->bits[value] = dst->bits[value] | (mask << (i - (32 * value)));
    return err;
}

int big_getSign(s21_big_decimal dst){
    return dst.bits[7] & 0x80000000 ? 1 : 0;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
    int err = 0;
    s21_big_decimal val1 = {0}, val2 = {0};
    s21_big_decimal *tmp = {0};
    big_decimal(value_1, &val1);
    big_decimal(value_2, &val2);
    if ((getSign(value_1) && getSign(value_2)) || (!getSign(value_1) && !getSign(value_2))) {
        big_add(val1, val2, tmp);
    } else {
        big_sub(val1, val2, tmp);
    }
    // big dec to dec
    return err;
}


int big_sub(s21_big_decimal first, s21_big_decimal second, s21_big_decimal* result) {
  s21_big_decimal d = {0};
  *result = d;
  if (big_not_null(second) == 1) {
    big_inverce(second, &d);
  }
  big_add(first, d, result);
  return 1;
}

void big_inverce(s21_big_decimal b, s21_big_decimal* d) {
  s21_big_decimal tmp = {0};
  tmp.bits[0] = ~(b.bits[0]);
  tmp.bits[1] = ~(b.bits[1]);
  tmp.bits[2] = ~(b.bits[2]);
  tmp.bits[3] = ~(b.bits[3]);
  tmp.bits[4] = ~(b.bits[4]);
  tmp.bits[5] = ~(b.bits[5]);
  s21_big_decimal plus_one = {{1, 0, 0, 0, 0, 0, 0}};
  big_add(plus_one, tmp, d);
}

int big_not_null(s21_big_decimal dst) {
  int result = 0;
  for (int i = 0; i < 8; i++) {
    if (dst.bits[i] != 0) {
      result = 1;
    }
  }
  return result;
}

int shift(s21_decimal* dst, int opt) {
  if (opt == 1) {
    for (int i = 96; i != 0; i--) {
      if (getBit(*dst, i - 1)) {
        setBit(dst, i);
      } else {
        cleanBit(dst, i);
      }
    }
    cleanBit(dst, 0);
  } else if (opt == -1) {
    for (int i = 0; i < 96; i++) {
      if (getBit(*dst, i + 1)) {
        setBit(dst, i);
      } else {
        cleanBit(dst, i);
      }
    }
    cleanBit(dst, 95);
  }
  return 1;
}

int cleanBit(s21_decimal* dst, int i) {
  int err = 1;
  unsigned int mask = 1;
  if (i >= 0 && i <= 31) {
    dst->bits[0] = dst->bits[0] & (~(mask << i));
  } else if (i >= 32 && i <= 63) {
    dst->bits[1] = dst->bits[1] & (~(mask << (i - 32)));
  } else if (i >= 64 && i <= 95) {
    dst->bits[2] = dst->bits[2] & (~(mask << (i - 64)));
  } else {
    err = -1;
  }
  return err;
}