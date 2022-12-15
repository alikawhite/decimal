#include "s21_decimal.h"

#define S21_OVERFLOW 1

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  *dst = s21_zero;
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
int s21_from_float_to_decimal(float src, s21_decimal *dst) {}