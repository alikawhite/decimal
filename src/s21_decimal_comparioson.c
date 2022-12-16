#include "s21_decimal.h"

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  int result = 0;
  if (s21_is_null(value_1) && s21_is_null(value_1)) {
    result = 0;
  } else {
    if (s21_get_sign(value_1) > s21_get_sign(value_2)) {
      result = 0;
    } else if (s21_get_sign(value_1) < s21_get_sign(value_2)) {
      result = 1;
    } else {
      unsigned buf1[6] = {0, 0, 0, 0, 0, 0};
      unsigned buf2[6] = {0, 0, 0, 0, 0, 0};
      int scale = s21_get_scale(value_1) >= s21_get_scale(value_2)
                      ? s21_get_scale(value_1)
                      : s21_get_scale(value_2);
      s21_to_scale(value_1, scale, buf1, 6);
      s21_to_scale(value_2, scale, buf2, 6);
      result = s21_data_gt(buf1, buf2, 6);
      if (s21_get_sign(value_1) && result)
        result = !result;
    }
  }
  return result;
}
int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  int result = 0;
  if (s21_is_null(value_1) && s21_is_null(value_2)) {
    result = 1;
  } else {
    if (s21_get_sign(value_1) != s21_get_sign(value_2)) {
      result = 0;
    } else {
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
