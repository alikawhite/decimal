#include "s21_decimal.h"

int s21_get_exp(float src) {
  unsigned int bits = *((unsigned int *)&src);
  int res = 0;
  bits <<= 1;
  res = res | (bits >> 24);
  return res - 127;
}

void s21_set_sign(s21_decimal *dst) { dst->bits[3] |= 0x80000000; }

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
        numb[j - 1] = '\0';
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
    char sign = flts[8];
    sscanf(numb, "%de%c%d", &_mantissa, &sign, &_exp10);
    sscanf(flts, "%d.%de%c%d", &tmp, &tmp, &numb[0], &_exp10);
    dst->bits[0] = _mantissa;
    dst->bits[3] = dst->bits[3] & 0x80000000 | ((_exp10 & 0xff) << 16);
  }
  return result;
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
