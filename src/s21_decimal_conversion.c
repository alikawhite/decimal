#include "s21_decimal.h"

#define S21_OVERFLOW 1

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  for (int i = 0; i < 4; i++)
    dst->bits[i] = 0;
  dst->bits[3] = src & 0x80000000;

  dst->bits[0] = (abs(src));
  return 0;
}
int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int status = S21_OK;
  *dst = 0;

  s21_truncate(src, &src);
  if (src.bits[2] || src.bits[1] || src.bits[0] > 0x7fffffff) {
    status = S21_OVERFLOW;
  }
  *dst = (src.bits[3] & 0x80000000) ? -(src.bits[0] & 0x7fffffff)
                                    : src.bits[0] & 0x7fffffff;

  return status;
}
int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int scale = s21_get_scale(src);
  while (!(src.bits[2] == 0 && src.bits[1] == 0 && src.bits[0] < 100000000)) {
    s21_div10(src, &src);
    scale--;
  }
  if (src.bits[0] >= 10000000) {
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
int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int result = 0;
  for (int i = 0; i < 4; i++) {
    dst->bits[i] = 0;
  }
  if ((src != src) | (fabsf(src) > 7.922816e28) | (fabsf(src) == INFINITY) |
      (fabsf(src) < 1e-28)) {
    if (fabsf(src) != 0) {
      result = 1;
    }
  } else {
    char flts[64];
    sprintf(flts, "%.6e", src);
    int sign_int = 1;
    if (src > 0)
      sign_int = 0;

    if (sign_int) {
      for (long unsigned int i = 0; flts[i] != '\0'; i++) {
        flts[i] = flts[i + 1];
      }
      s21_set_sign(dst);
    }
    char numb[64];
    int j = 0;
    for (unsigned long int i = 0; i < strlen(flts); i++) {
      if (flts[i] == 'e') {
        numb[j] = '\0';
        break;
      }
      if (flts[i] != 46) {
        numb[j] = flts[i];
        j++;
      }
    }
    int _exp10 = 0;
    int _mantissa = 0;
    int tmp = 0;
    sscanf(numb, "%d", &_mantissa);
    sscanf(flts, "%d.%de%d", &tmp, &tmp, &_exp10);
    dst->bits[0] = (_mantissa);
    int last_digit = 0;
    while (_exp10 > 28) {
      last_digit = s21_mod10mem(dst->bits, 3);
      s21_div10mem(dst->bits, dst->bits, 3);
      _exp10--;
    }
    if (last_digit >= 5) {
      unsigned carry[3] = {1, 0, 0};
      s21_data_add(dst->bits, carry, dst->bits, 3);
    }
    while (_exp10 < 0) {
      last_digit = s21_mod10mem(dst->bits, 3);
      s21_mul10mem(dst->bits, 3);
      _exp10++;
    }
    s21_set_scale(dst, 6 - _exp10);
  }
  return result;
}
