#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  *dst = s21_zero;
  dst->bits[3] = src & 0x80000000;

  dst->bits[0] = (abs(src));
  return 0;
}