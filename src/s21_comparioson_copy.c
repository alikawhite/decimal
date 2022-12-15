#include "s21_decimal.h"

int s21_multiply_by_integer_mem(unsigned *value, int size_v, int integer,
                                unsigned *result, int size_r);

int getSign(s21_decimal dst) { return dst.bits[3] & 0x80000000 ? 1 : 0; }
int s21_get_scale(s21_decimal value) { return (value.bits[3] >> 16) & 0xff; }
int s21_is_null(s21_decimal value) {
  return (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0);
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  int result = 0;
  if (s21_is_null(value_1) && s21_is_null(value_1)) {
    result = 0;
  } else {
    if (getSign(value_1) > getSign(value_2)) {
      result = 0;
    } else if (getSign(value_1) < getSign(value_2)) {
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
      if (getSign(value_1) && result) result = !result;
    }
  }
  return result;
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  int result = 0;
  if (s21_is_null(value_1) && s21_is_null(value_2)) {
    result = 1;
  } else {
    if (getSign(value_1) != getSign(value_2)) {
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

int s21_data_eq(const unsigned *data1, const unsigned *data2, int size) {
  int flag = 1;
  for (int i = size - 1; i >= 0; i--) {
    flag = flag && (data1[i] == data2[i]);
  }
  return flag;
}

int s21_data_gt(const unsigned *data1, const unsigned *data2, int size) {
  int result = 0;
  int flag = 0;
  for (int i = size - 1; (i >= 0) && (!flag); i--) {
    if (data1[i] > data2[i]) {
      flag = 1;
      result = 1;
    } else if (data1[i] < data2[i]) {
      flag = 1;
      result = 0;
    }
  }
  return result;
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

int s21_multiply_by_integer_mem(unsigned *value, int size_v, int integer,
                                unsigned *result, int size_r) {
  unsigned buf_array[size_r];
  for (int i = 0; i < size_r; i++) {
    buf_array[i] = 0;
  }
  for (int i = 0; i < size_v; i++) {
    buf_array[i] = value[i];
  }
  int status = 0;
  unsigned long long buf = 0ull;
  for (int i = 0; i < size_r; i++) {
    buf += (0ull + result[i]) * integer;
    buf_array[i] = (unsigned)buf;
    buf = (buf >> 32);
  }
  for (int i = 0; i < size_r; i++) {
    result[i] = buf_array[i];
  }
  return status;
}

void s21_to_scale(s21_decimal value, int scale, unsigned *result, int size) {
  for (int i = 0; i < size; i++) {
    result[i] = 0;
  }
  for (int i = 0; i < 3; i++) {
    result[i] = value.bits[i];
  }
  for (int i = s21_get_scale(value); i < scale; i++) {
    s21_multiply_by_integer_mem(result, 6, 10, result, 6);
  }
}
