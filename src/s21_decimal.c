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

// int s21_from_decimal_to_int(s21_decimal src, int *dst) { // посмотреть, что ретернить
//     int err = 0, tmp = 0;
//     if (getScale(src) != 0) {
//         err = 1;
//     } else if (1 == 1) { // проверка не работает!!!!
//         for (int i = 0; i < 32; i++) {
//             tmp *= 2;
//             if (getBit(src, 31 - i))
//                 tmp += 1;
//         }
//         if (getSign(src))
//             tmp *= -1;
//     }
//     *dst = tmp;
//     return err;
// }

// int s21_from_decimal_to_float(s21_decimal src, float* dst) {
//     double tmp = 0;
//     src = s21_zero;
//     for (int i = 0; i < 96; i++) {
//         tmp *= 2;
//         if (getBit(src, 95 - i))
//             tmp += 1;
//     }
//     for (int j = getScale(src); j != 0; tmp /= 10, j--) {
//     }
//     if (getSign(src))
//         tmp *= -1;
//     *dst = tmp;
//     return 0;
// }

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

// int s21_negate(s21_decimal value, s21_decimal *result) {
//     int err = 0;
//     if (getScale(value) > 28) {
//         err = 1;
//     }
//     else {
//         if (!getSign(value)) {
//             setSign(&value);
//         } else {
//             value.bits[3] &= 0b01111111111111111111111111111111;
//         }
//         *result = value;
//     }
//     return err;
// }

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

void to_one_scale(s21_decimal *value1, s21_decimal *value2, s21_big_decimal* first, s21_big_decimal *second) {
    s21_big_decimal result = {0};
    s21_big_decimal tmp1 = {0};
    s21_big_decimal tmp2 = {0};
    big_decimal(*value1, &tmp1);
    big_decimal(*value2, &tmp2);
    if (getScale(*value1) < getScale(*value2)) {
      scale_up(&tmp1, getScale(*value1) - getScale(*value2), &result);
      *first = result;
      *second = tmp2;
    } else if (getScale(*value1) > getScale(*value2)) {
      scale_up(&tmp2, getScale(*value1) - getScale(*value2), &result);
      *second = result;
      *first = tmp1;
    } else {
      *first = tmp1;
      *second = tmp2;
    }
}

void scale_up(s21_big_decimal *dst, int value, s21_big_decimal *result) {
  s21_big_decimal m = {{10, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal tmp_result = {0};
  int sign = big_getSign(*dst);
  int scale = big_getScale(*dst);
  for (int i = 0; i < value; i++) {
    big_mult(*dst, m, &tmp_result);
    *dst = tmp_result;
    to_zero(&tmp_result);
  }
  *result = *dst;
  big_cleanScale(result);
  big_setScale(result, scale + value);
  if (sign)
    big_setSign(result);
}

void scale_down(s21_big_decimal* dst, int value, s21_big_decimal* result) {
  s21_big_decimal tmp_result = {0};
  int sign = big_getSign(*dst);
  int scale = big_getScale(*dst);
  for (int i = 0; i < value; i++) {
    big_div10(*dst, &tmp_result);
    *dst = tmp_result;
    to_zero(&tmp_result);
  }
  *result = *dst;
  big_cleanScale(result);
  big_setScale(result, scale - value);
  if (sign)
    big_setSign(result);
}

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
  result->bits[7] = a.bits[7];
}

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

// сравнение модуля!!!!!
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int err = 0;
  s21_big_decimal val1 = {0}, val2 = {0};
  s21_big_decimal *tmp = {0};
  to_one_scale(&value_1, &value_2, &val1, &val2);
  if ((getSign(value_1) && getSign(value_2)) || (!getSign(value_1) && !getSign(value_2))) { // + + + & - + -
    big_add(val1, val2, tmp);
  } else if (getSign(value_1) && !getSign(value_2)) { // - + +
    big_cleanSign(&val1); 
    big_cleanSign(&val2); 
    if (s21_is_greater(value_1, value_2))  {
      big_sub(val1, val2, tmp);
      big_setSign(tmp);
    } else {
      big_sub(val2, val1, tmp);
    }
  } else { // + + -
    big_cleanSign(&val1); 
    big_cleanSign(&val2); 
    if (s21_is_greater(value_1, value_2))  {
      big_sub(val1, val2, tmp);
    } else {
      big_sub(val2, val1, tmp);
      big_setSign(tmp);      
    }
  }
  big_to_dec(*tmp, result);
  return err;
}

void big_sub(s21_big_decimal first, s21_big_decimal second, s21_big_decimal* result) {
  s21_big_decimal d = {0};
  *result = d;
  if (big_not_null(second)) {
    big_inverce(second, &d);
  }
  big_add(first, d, result);
  for (int i = 191, bit = 0; bit != 1; i--) {
    bit = big_getBit(*result, i);
    if (bit)
      big_cleanBit(result, i);
  }
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

int big_to_dec(s21_big_decimal dst, s21_decimal* result) {
  int err = 0;
  int scale = big_getScale(dst);
  int trunc;
  s21_big_decimal tmp = dst;
  s21_big_decimal tmp_result = {0};
  if (scale <= 28 && tmp.bits[3] == 0 && tmp.bits[4] && tmp.bits[5] == 0) {
    result->bits[0] = tmp.bits[0];
    result->bits[1] = tmp.bits[1];
    result->bits[2] = tmp.bits[2];
    result->bits[3] = tmp.bits[6];
  } else {
    while (scale && (scale > 28 || tmp.bits[3] || tmp.bits[4] || tmp.bits[5])) {
      scale_down(&tmp, 1, &tmp_result);
      tmp = tmp_result;
      trunc = tmp_result.bits[0] % 10;
      to_zero(&tmp_result);
      scale--;
    }
    if (trunc > 5 || (trunc = 5 && big_getBit(tmp, 0))) {
      s21_big_decimal tmp_trunc = {0};
      s21_big_decimal tmp_trunc2 = {{1, 0, 0, 0, 0, 0, 0}};
      big_add(tmp, tmp_trunc2, &tmp_trunc);
      tmp = tmp_trunc;
    }
    if (tmp.bits[3] == 0 && tmp.bits[4] == 0 && tmp.bits[5] == 0) {
      result->bits[0] = tmp.bits[0];
      result->bits[1] = tmp.bits[1];
      result->bits[2] = tmp.bits[2];
      result->bits[3] = tmp.bits[6];
    } else {
      if (big_getSign(dst))
        err = 2;
      else
        err = 1;
    }
  }
  return err;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int err = 0;
  s21_big_decimal val1 = {0}, val2 = {0};
  s21_big_decimal *tmp = {0};
  to_one_scale(&value_1, &value_2, &val1, &val2);
  if ((!getSign(value_1) && getSign(value_2)) || (getSign(value_1) && !getSign(value_2))) { // + - - & - - +
    big_add(val1, val2, tmp);
  } else if (getSign(value_1) && getSign(value_2)) { // - - - 
    big_cleanSign(&val1); 
    big_cleanSign(&val2); 
    if (s21_is_greater(value_1, value_2))  {
      big_sub(val1, val2, tmp);
      big_setSign(tmp);
    } else {
      big_sub(val2, val1, tmp);
    }
  } else { // + - +
    if (s21_is_greater(value_1, value_2))  {
      big_sub(val1, val2, tmp);
    } else {
      big_sub(val2, val1, tmp);
      big_setSign(tmp);      
    }
  }
  big_to_dec(*tmp, result);
  return err;
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

void cleanSign(s21_decimal* value) {
  value->bits[3] &= 0b01111111111111111111111111111111;
}

void big_cleanSign(s21_big_decimal* value) {
  value->bits[7] &= 0b01111111111111111111111111111111;
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


// Арифметика
// int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
//   s21_negate(value_2, &value_2);
//   return s21_add(value_1, value_2, result);
// }

// Сравнения
int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  int result = 0;
  if (s21_is_null(value_1) && s21_is_null(value_1)) {
    result = 0;
  } else {
    unsigned buf1[6] = {0, 0, 0, 0, 0, 0};
    unsigned buf2[6] = {0, 0, 0, 0, 0, 0};
    int scale = s21_get_scale(value_1) > s21_get_scale(value_2)
                    ? s21_get_scale(value_1)
                    : s21_get_scale(value_2);
    s21_to_scale(value_1, scale, buf1, 6);
    s21_to_scale(value_2, scale, buf1, 6);
    result = s21_data_gt(buf1, value_1.bits[3] & 0x80000000, buf2,
                         value_2.bits[3] & 0x80000000, 6);
  }
  return result;
}
int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  int result = 0;
  if (s21_is_null(value_1) && s21_is_null(value_2)) {
    result = 1;
  } else {
    if ((value_1.bits[3] & 0x80000000) == (value_2.bits[3] & 0x80000000)) {
      unsigned buf1[6] = {0, 0, 0, 0, 0, 0};
      unsigned buf2[6] = {0, 0, 0, 0, 0, 0};
      int scale = s21_get_scale(value_1) > s21_get_scale(value_2)
                      ? s21_get_scale(value_1)
                      : s21_get_scale(value_2);
      s21_to_scale(value_1, scale, buf1, 6);
      s21_to_scale(value_2, scale, buf2, 6);
      result = s21_data_eq(buf1, buf2, 6);
    }
  }
  return result;
}
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_equal(value_1, value_2);
}
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return s21_is_greater(value_1, value_2) || s21_is_equal(value_1, value_2);
}
int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  return s21_is_greater(value_2, value_1);
}
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return s21_is_greater_or_equal(value_2, value_1);
}

// Переводы
int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  for (int i = 0; i < 4; i++) {
    dst->bits[i] = 0;
  }
  dst->bits[0] = src & 0x7fffffff;
  dst->bits[3] = src & 0x80000000;

  return 0;
}
int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int result = 1;
  s21_truncate(src, &src);
  if (!(src.bits[2] || src.bits[2] || (src.bits[0] & 0x80000000))) {
    *dst = (src.bits[3] & 0x80000000) | (src.bits[0] & ~0x80000000);
    result = 0;
  }
  return result;
}
int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int scale = s21_get_scale(src);
  while (!(src.bits[2] == 0 && src.bits[1] == 0 && src.bits[0] > 100000000)) {
    s21_div10(src, &src);
    scale--;
  }
  if (src.bits[0] <= 10000000) {
    src.bits[0] += 5;
    s21_div10(src, &src);
    scale--;
  }
  char buf[32] = {0};
  sprintf(buf, "%c%de%d", (src.bits[3] & 0x80000000 ? '-' : '+'), src.bits[0],
          -scale);
  sscanf(buf, "%e", dst);
  return 0;
}

// Другое
int s21_negate(s21_decimal value, s21_decimal *result) {
  for (int i = 0; i < 4; i++) {
    result->bits[i] = value.bits[i];
  }
  result->bits[3] =
      (value.bits[3] & 0x7fffffff) | ((~value.bits[3]) & 0x80000000);
  printf("Ok\n\n");
  return 0;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  for (int i = 0; i < 3; i++) {
    result->bits[i] = value.bits[i];
  }
  result->bits[3] = ((~value.bits[3]) & 0x80000000);
  int scale = s21_get_scale(value);
  for (; scale >= 0; scale--) {
    s21_div10(*result, result);
  }

  return 0;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  for (int i = 0; i < 3; i++) {
    result->bits[i] = value.bits[i];
  }
  result->bits[3] = ((~value.bits[3]) & 0x80000000);
  int scale = s21_get_scale(value);
  for (; scale >= 1; scale--) {
    s21_div10(*result, result);
  }
  if (scale) {
    if (s21_mod10(value) >= 5) {
      s21_div10(*result, result);
      s21_inc(result);
    } else {
      s21_div10(*result, result);
    }
  }

  return 0;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  for (int i = 0; i < 3; i++) {
    result->bits[i] = value.bits[i];
  }
  result->bits[3] = ((~value.bits[3]) & 0x80000000);
  int scale = s21_get_scale(value);
  for (; scale >= 1; scale--) {
    s21_div10(*result, result);
  }
  if (scale) {
    if (result->bits[3] && (s21_mod10(value) > 0)) {
      s21_div10(*result, result);
      s21_inc(result);
    } else {
      s21_div10(*result, result);
    }
  }
  return 0;
}

// Вспомогательное
int s21_data_eq(const unsigned *data1, const unsigned *data2, int size) {
  int flag = 1;
  for (int i = size - 1; i >= 0; i--) {
    flag = flag && (data1[i] == data2[i]);
  }
  return flag;
}

int s21_data_gt(const unsigned *data1, int sign1, const unsigned *data2,
                int sign2, int size) {
  int result = 0;
  int flag = 0;
  if (!sign1 > !sign2) {
    // positive / negative
    result = 1;
  } else if (!sign1 < !sign2) {
    // negative / positive
    result = 0;
  } else if (sign1 == sign2) {
    // p/p n/n
    for (int i = 0; (i < size) && (!flag); i++) {
      if (data1[i] > data2[i]) {
        flag = 1;
        result = 1;
      } else if (data1[i] < data2[i]) {
        flag = 1;
        result = 0;
      }
    }
    if (!sign1 == 0) {
      // negative / negative
      if (flag) {
        result = !result;
      }
    }
  }
  return result;
}

int s21_div10(s21_decimal value, s21_decimal *result) {
  unsigned long long buf = 0ull;
  unsigned mod = 0;
  for (int i = 0; i < 4; i++) {
    result->bits[i] = value.bits[i];
  }
  for (int i = 2; i >= 0; i--) {
    buf <<= 32;
    mod = (buf + result->bits[i]) % 10;
    result->bits[i] = (unsigned)((buf + result->bits[i]) / 10);
    buf = mod;
  }

  return 0;
}

void big_div10(s21_big_decimal value, s21_big_decimal *result) {
  unsigned long long buf = 0ull;
  unsigned mod = 0;
  for (int i = 0; i < 7; i++) {
    result->bits[i] = value.bits[i];
  }
  for (int i = 5; i >= 0; i--) {
    buf <<= 32;
    mod = (buf + result->bits[i]) % 10;
    result->bits[i] = (unsigned)((buf + result->bits[i]) / 10);
    buf = mod;
  }
}

int s21_is_null(s21_decimal value) {
  return (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0);
}

int s21_get_scale(s21_decimal value) { return (value.bits[3] >> 16) & 0xff; }

int s21_mod10(s21_decimal value) {
  unsigned long long buf = 0ull;
  unsigned mod = 0;
  for (int i = 2; i >= 0; i--) {
    buf <<= 32;
    mod = (buf + value.bits[i]) % 10;
    value.bits[i] = (unsigned)((buf + value.bits[i]) / 10);
    buf = mod;
  }
  return mod;
}
int s21_inc(s21_decimal *result) {
  unsigned long long buf = 0ull;
  for (int i = 0; i < 3; i++) {
    buf += result->bits[i] + 1;
    result->bits[i] = (unsigned)buf;
    buf >>= 32;
  }
  return 0;
}
int s21_mul10(s21_decimal value, s21_decimal *result) {
  int status = 0;
  unsigned long long buf = 0ull;
  for (int i = 0; i < 3; i++) {
    buf += value.bits[i] * 10;
    value.bits[i] = (unsigned)buf;
    buf >>= 32;
  }
  if (!buf) {
    for (int i = 0; i < 4; i++) {
      result->bits[i] = value.bits[i];
    }
  } else {
    status = 1;
  }
  return status;
}
int s21_sum(const unsigned *val1, int sign1, const unsigned *val2, int sign2,
            unsigned *result, int *sign_result, int size) {
  int status = 0;
  if (sign1 == sign2) {
    unsigned long long buf = 0ull;
    for (int i = 0; i < size; i++) {
      buf += val1[i] + val2[i];
      result[i] = (unsigned)buf;
      buf >>= 32;
    }
    *sign_result = sign1;
  } else if (!sign1 > !sign2) {
    unsigned long long buf = 0ull;
    for (int i = 0; i < size; i++) {
      buf = buf + (unsigned long long)val1[i] - (unsigned long long)val2[i];
      result[i] = (unsigned)buf;
      buf >>= 32;
    }
    *sign_result = (buf >> 31);
  } else {
    status = s21_sum(val2, sign2, val1, sign1, result, sign_result, size);
  }
  return status;
}

// int s21_div2(const unsigned *val1, const unsigned *val2, unsigned *result,
//              int size) {
//   unsigned val1_copy[size];
//   for (int i = 0; i < size; i++) {
//     val1_copy[i] = val1[i];
//   }
//   int buf = 0;
//   while (val1[0] || val1[1] || val1[2]) {
//     s21_mul2(val1_copy, val1_copy);
//     if (s21_data_gt(val1_copy, 0, val2, 0, size) ||
//         s21_data_eq(val1_copy, val2, size)) {
//       s21_sum(val1_copy, 0, val2, 1, val1_copy, &buf, size);
//       result[0] |= 1;
//     }
//   }
//   return 0;
// }

int s21_decimal_serialize(s21_decimal value, int fd) {
  if (fd) {
    write(fd, &value, sizeof(s21_decimal));
    return 0;
  } else {
    return 1;
  }
}
int s21_decimal_deserialize(s21_decimal *value, int fd) {

  return (read(fd, value, sizeof(s21_decimal)) != EOF);
}
