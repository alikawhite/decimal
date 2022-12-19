#include "s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  for (int i = 0; i < 4; i++) {
    result->bits[i] = value.bits[i];
  }
  result->bits[3] =
      (value.bits[3] & 0x7fffffff) | ((~value.bits[3]) & 0x80000000);
  return 0;
}
int s21_truncate(s21_decimal value, s21_decimal *result) {
  for (int i = 0; i < 4; i++) {
    result->bits[i] = value.bits[i];
  }
  int scale = s21_get_exponent(value);
  result->bits[3] = value.bits[3] & 0x80000000;
  s21_divide_by_power_of_10(*result, scale, result);

  return 0;
}
int s21_round(s21_decimal value, s21_decimal *result) {
  for (int i = 0; i < 4; i++) {
    result->bits[i] = value.bits[i];
  }
  int scale = s21_get_exponent(value);
  if (scale) {
    result->bits[3] = value.bits[3] & 0x80000000;
    for (; scale > 1; scale--) {
      s21_divide_by_integer(*result, 10, result);
    }
    int flag = s21_modulo_by_integer(*result, 10) >= 5;

    int status = s21_truncate(value, result);
    if (flag && !status) {
      s21_add_integer(*result, 1, result);
    }
  }

  return 0;
}
int s21_floor(s21_decimal value, s21_decimal *result) {
  int status = 0;
  if (value.bits[3] & 0x80000000) {
    status = s21_truncate(value, result);
    if ((value.bits[0] != 0 || value.bits[1] != 0 || value.bits[2] != 0) &&
        s21_get_scale(value)) {
      s21_add_integer(*result, 1, result);
    }
  } else {
    status = s21_truncate(value, result);
  }

  return status;
}
