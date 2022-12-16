#include "s21_decimal.h"

int s21_get_exp(float src) {
  unsigned int bits = *((unsigned int *)&src);
  int res = 0;
  bits <<= 1;
  res = res | (bits >> 24);
  return res - 127;
}

void s21_set_scale(s21_decimal *dst, int scale) {
  unsigned int mask = 0;
  mask = mask | scale;
  dst->bits[3] = dst->bits[3] | (mask << 16);
}

void s21_set_sign(s21_decimal *dst) { dst->bits[3] |= 0x80000000; }

int s21_get_sign(s21_decimal dst) { return dst.bits[3] & 0x80000000 ? 1 : 0; }

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
    if (src > 0) sign_int = 0;

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
    sscanf(flts, "%d.%de%c%d", &tmp, &tmp, &numb[0], &_exp10);
    dst->bits[0] = (_mantissa);
    s21_set_scale(dst, 6 - _exp10);
  }
  return result;
}
