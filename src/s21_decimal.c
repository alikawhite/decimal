#include "s21_decimal.h"

// убрать отсюда нахуй эту константу, чтобы не доебались

const s21_decimal s21_zero = {0};

union test {
  float asFloat;
  long int asInteger;
};

void setSign(s21_decimal *dst) { dst->bits[3] |= 0x80000000; }

int getBit(s21_decimal d, int i) {
  unsigned int mask = 1u << (i % 32);
  int r = 0;
  if (i <= 95 && i >= 0) {
    r = d.bits[i / 32] & mask;
  }
  return r;
}

int getExp(float src) {
  unsigned int bits = *((unsigned int *)&src);
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

void setScale(s21_decimal *dst, int scale) {
  unsigned int mask = 0;
  mask = mask | scale;
  dst->bits[3] = dst->bits[3] | (mask << 16);
}

int getScale(s21_decimal src) {
  src.bits[3] <<= 8;
  unsigned int res = src.bits[3] >> 24;
  return res;
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
  for (int i = 0; i < 7; i++) {
    printf("%d\t", result->bits[i]);
  }
  printf("\n");
}

int big_getScale(s21_big_decimal src) {
  src.bits[6] <<= 8;
  unsigned int res = src.bits[6] >> 24;
  return res;
}

void to_one_scale(s21_decimal *value1, s21_decimal *value2,
                  s21_big_decimal *first, s21_big_decimal *second) {
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
  s21_big_decimal tmp_result = {0};
  int sign = big_getSign(*dst);
  int scale = big_getScale(*dst);
  tmp_result = *dst;
  for (int i = 0; i < value; i++) {
    big_mul10(&tmp_result);
    *dst = tmp_result;
    to_zero(&tmp_result);
  }
  *result = *dst;
  big_cleanScale(result);
  big_setScale(result, scale + value);
  if (sign)
    big_setSign(result);
}

void scale_down(s21_big_decimal *dst, int value, s21_big_decimal *result) {
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

void big_setScale(s21_big_decimal *dst, int scale) {
  unsigned int mask = 0;
  mask = mask | scale;
  dst->bits[6] = dst->bits[6] | (mask << 16);
}

void big_cleanScale(s21_big_decimal *dst) {
  unsigned int mask = 0x10000000;
  dst->bits[6] = dst->bits[6] & mask;
}

void to_zero(s21_big_decimal *dst) {
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

void big_cleanBit(s21_big_decimal *dst, int i) {
  unsigned int mask = 1;
  int value = i / 32;
  dst->bits[value] = dst->bits[value] & (~(mask << (i - (32 * value))));
}

void big_add(s21_big_decimal a, s21_big_decimal b, s21_big_decimal *result) {
  int tmp = 0;
  s21_big_decimal zero = {0};
  *result = zero;
  // for (int i = 0; i < 7; i++) {
  //   printf("\n%d", a.bits[i]);
  // }
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
  if (dst.bits[value] && (mask << (i - (32 * value)))) {
    res = 1;
  }
  return res;
}

int big_setBit(s21_big_decimal *dst, int i) {
  int err = 0;
  unsigned int mask = 1;
  int value = i / 32;
  dst->bits[value] = dst->bits[value] | (mask << (i - (32 * value)));
  return err;
}

int big_getSign(s21_big_decimal dst) {
  return dst.bits[7] & 0x80000000 ? 1 : 0;
}

int comparison(s21_big_decimal value_1, s21_big_decimal value_2) {
  int result = 0;
  int flag = 0;
  for (int i = 192; (i >= 0) && (!flag); i--) {
    if (value_1.bits[i] > value_2.bits[i]) {
      flag = 1;
      result = 1;
    } else if (value_1.bits[i] < value_2.bits[i]) {
      flag = 1;
      result = 0;
    }
  }
  return result;
}

// сравнение модуля!!!!!
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int err = 0;
  s21_big_decimal val1 = {0};
  s21_big_decimal val2 = {0};
  s21_big_decimal *tmp = &val1;
  to_one_scale(&value_1, &value_2, &val1, &val2);
  // int scale = big_getScale(val1);
  if ((getSign(value_1) && getSign(value_2)) ||
      (!getSign(value_1) && !getSign(value_2))) { // + + + & - + -
    big_add(val1, val2, tmp);
  } else if (getSign(value_1) && !getSign(value_2)) { // - + +
    big_cleanSign(&val1);
    big_cleanSign(&val2);
    if (comparison(val1, val2)) {
      big_sub(val1, val2, tmp);
      big_setSign(tmp);
    } else {
      big_sub(val2, val1, tmp);
    }
  } else { // + + -
    big_cleanSign(&val1);
    big_cleanSign(&val2);
    if (comparison(val1, val2)) {
      big_sub(val1, val2, tmp);
    } else {
      big_sub(val2, val1, tmp);
      big_setSign(tmp);
    }
  }
  // for (int i = 0; i < 7; i++) {
  //   printf("%d\n", tmp->bits[i]);
  // }
  big_to_dec(*tmp, result);
  return err;
}

void big_sub(s21_big_decimal first, s21_big_decimal second,
             s21_big_decimal *result) {
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

void big_inverce(s21_big_decimal b, s21_big_decimal *d) {
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

int big_to_dec(s21_big_decimal dst, s21_decimal *result) {
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
  if ((!getSign(value_1) && getSign(value_2)) ||
      (getSign(value_1) && !getSign(value_2))) { // + - - & - - +
    big_add(val1, val2, tmp);
  } else if (getSign(value_1) && getSign(value_2)) { // - - -
    big_cleanSign(&val1);
    big_cleanSign(&val2);
    if (comparison(val1, val2)) {
      big_sub(val1, val2, tmp);
      big_setSign(tmp);
    } else {
      big_sub(val2, val1, tmp);
    }
  } else { // + - +
    if (comparison(val1, val2)) {
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

int shift(s21_decimal *dst, int opt) {
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

int cleanBit(s21_decimal *dst, int i) {
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

void cleanSign(s21_decimal *value) {
  value->bits[3] &= 0b01111111111111111111111111111111;
}

void big_cleanSign(s21_big_decimal *value) {
  value->bits[7] &= 0b01111111111111111111111111111111;
}

int s21_mul10(s21_decimal *value) {
  int status = 0;
  s21_decimal result = {0};
  unsigned long long buf = 0ull;
  for (int i = 0; i < 3; i++) {
    buf >>= 32;
    buf = buf + result.bits[i] * 10;
    result.bits[i] = (unsigned)buf;
  }
  status = buf >> 32;
  if (!status) {
    for (int i = 0; i < 3; i++) {
      value->bits[i] = result.bits[i];
    }
  }
  return status;
}

int big_mul10(s21_big_decimal *value) {
  int status = 0;
  s21_big_decimal result = {0};
  unsigned long long buf = 0ull;
  for (int i = 0; i < 6; i++) {
    buf >>= 32;
    buf = buf + result.bits[i] * 10;
    result.bits[i] = (unsigned)buf;
  }
  status = buf >> 32;
  if (!status) {
    for (int i = 0; i < 3; i++) {
      value->bits[i] = result.bits[i];
    }
  }
  return status;
}

// Сравнения

// int s21_from_decimal_to_float(s21_decimal src, float *dst) {
//   int scale = s21_get_scale(src);
//   while (!(src.bits[2] == 0 && src.bits[1] == 0 && src.bits[0] > 100000000))
//   {
//     s21_div10(src, &src);
//     scale--;
//   }
//   if (src.bits[0] <= 10000000) {
//     src.bits[0] += 5;
//     s21_div10(src, &src);
//     scale--;
//   }
//   char buf[32] = {0};
//   sprintf(buf, "%c%de%d", (src.bits[3] & 0x80000000 ? '-' : '+'),
//   src.bits[0],
//           -scale);
//   sscanf(buf, "%e", dst);
//   return 0;
// }

// Вспомогательное

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

int s21_div10(s21_decimal value, s21_decimal *result) {
  int integer = 10;
  unsigned long long buf = 0ull;
  unsigned long long div_buf = 0ull;
  unsigned mod = 0;
  for (int i = 2; i >= 0; i--) {

    mod = (buf + value.bits[i]) % integer;
    div_buf = (buf + value.bits[i]) / integer;
    result->bits[i] = (unsigned)div_buf;
    buf = mod;
    buf = buf << 32;
  }
  return 0;
}

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

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = S21_OK;
  // for (int i = 0; i < 4; i++) {
  //   result->bits[i] = 0;
  // }
  // // Check if the numbers are negative
  // int value_1_negative = value_1.bits[3] & 0x80000000;
  // int value_2_negative = value_2.bits[3] & 0x80000000;
  // for (int i = 0; i < 4; i++) {
  //   for (int j = 0; j < 4; j++) {
  //     if (i + j < 4) {
  //       result->bits[i + j] += value_1.bits[i] * value_2.bits[j];
  //     }
  //   }
  // }

  return status;
}
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = S21_OK;

  return status;
}
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = S21_OK;

  return status;
}