#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  *dst = s21_zero;
  dst->bits[3] = src & 0x80000000;
  dst->bits[0] = (abs(src));
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