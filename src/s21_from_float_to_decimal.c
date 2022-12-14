#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "s21_decimal.h"

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
    flts[1] = '\0';
    int last = 0;
    for (int i = 2; flts[i] != '\0'; i++) {
      flts[i - 1] = flts[i];
      last = i;
    }
    flts[last] = '\0';
    int _exp10 = 0;
    int _mantissa = 0;
    char sign = flts[8];

    sscanf(flts, "%de%c%d", &_mantissa, &sign, &_exp10);
    printf("%d\n", _exp10);
    if (sign == '-') {
      _exp10 = -_exp10;
    }
    dst->bits[0] = _mantissa;
    _exp10 -= 6;

    while (_exp10 > 0) {
      s21_mul10(dst);
      _exp10--;
    }
    if (_exp10 < 0) {
      setScale(dst, -_exp10);
    }
  }
  return result;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int div_num = 0;
  while ((src.bits[2]) || (src.bits[1]) || (src.bits[0] >= 100000000)) {
    s21_divide_by_power_of_10(src, 1, &src);
    div_num++;
  }

  if (src.bits[0] >= 10000000) {
    if (src.bits[0] % 10 >= 5) {
      src.bits[0] += 5;
    }
    s21_divide_by_power_of_10(src, 1, &src);
    div_num++;
  }
  int _exp10 = div_num - getScale(src);
  char flts[64];

  if (src.bits[3] >> 31) {
    sprintf(flts, "-%de%d", src.bits[0], _exp10);
  } else {
    sprintf(flts, "%de%d", src.bits[0], _exp10);
  }
  sscanf(flts, "%e", dst);
  return 0;
}

int s21_multiply_by_integer(s21_decimal value, int integer,
                            s21_decimal *result) {
  int status = 0;
  unsigned long long buf = 0ull;
  for (int i = 0; i < 3; i++) {
    buf += value.bits[i] * integer;
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
