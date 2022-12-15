#include "s21_decimal.h"

int s21_divide_by_integer(s21_decimal value, int integer, s21_decimal *result) {
  unsigned long long buf = 0ull;
  unsigned long long div_buf = 0ull;
  unsigned mod = 0;
  for (int i = 2; i >= 0; i--) {

    mod = (buf + value.bits[i]) % integer;
    div_buf = (buf + value.bits[i]) / integer;
    result->bits[i] = (unsigned)div_buf;
    buf = mod;
    buf = buf << 32;
  }
  return 0;
}

int s21_modulo_by_integer(s21_decimal value, int integer) {
  unsigned long long buf = 0ull;
  unsigned long long div_buf = 0ull;
  unsigned long long mod = 0;
  for (int i = 2; i >= 0; i--) {

    mod = (buf + value.bits[i]) % integer;
    div_buf = (buf + value.bits[i]) / integer;
    buf = mod << 32;
  }
  return mod;
}

int s21_add_integer(s21_decimal value, int integer, s21_decimal *result) {
  int status = S21_OK;
  unsigned long long buf = 0ull + integer;
  for (int i = 0; i < 3; i++) {
    buf += value.bits[i];
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

int s21_divide_by_power_of_10(s21_decimal value, int power,
                              s21_decimal *result) {
  int status = S21_OK;
  for (int i = 0; i < power; i++) {
    status = s21_divide_by_integer(value, 10, result);
    value = *result;
  }

  return status;
}
int s21_get_exponent(s21_decimal value) {
  int status = (value.bits[3] & 0x00FF0000) >> 16;

  return status;
}

int s21_decimal_deserialize(s21_decimal *value, int fd) {
  return read(fd, value->bits, sizeof(s21_decimal));
}
int s21_decimal_serialize(s21_decimal value, int fd) {
  return write(fd, value.bits, sizeof(s21_decimal));
}

// ---------------------------------------------------

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
    if (flag) {
      s21_add_integer(*result, 1, result);
    }
  }

  return 0;
}
int s21_floor(s21_decimal value, s21_decimal *result) {
  int status = 0;
  if (value.bits[3] & 0x80000000) {
    status = s21_truncate(value, result);
    s21_add_integer(*result, 1, result);
  } else {
    status = s21_truncate(value, result);
  }

  return status;
}

// Вспомогательные функции
