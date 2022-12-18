#include "s21_decimal.h"

// Вспомогательное
int s21_div10(s21_decimal value, s21_decimal *result) {
  unsigned long long buf = 0ull;
  unsigned mod = 0;
  for (int i = 0; i < 4; i++) {
    result->bits[i] = value.bits[i];
  }
  for (int i = 2; i >= 0; i--) {
    buf <<= 32;
    mod = (buf + result->bits[i]) % 10;
    result->bits[i] = (unsigned)((buf + result->bits[i]) / 10);
    buf = mod;
  }

  return 0;
}

void s21_negate_data(unsigned *value, int size) {
  for (int i = 0; i < size; i++)
    value[i] = ~value[i];
  unsigned long long buf = 1ull;
  for (int i = 0; i < size; i++) {
    buf = buf + (unsigned long long)value[i];
    value[i] = (unsigned)buf;
    buf >>= 32;
  }
}
void s21_data_add(unsigned *buf_1, unsigned *buf_2, unsigned *buf_result,
                  int size) {
  unsigned long long buf = 0ull;
  for (int i = 0; i < size; i++) {
    buf += (unsigned long long)buf_1[i] + (unsigned long long)buf_2[i];
    buf_result[i] = (unsigned)buf;
    buf >>= 32;
  }
}
void s21_data_sub(unsigned *buf_1, unsigned *buf_2, unsigned *buf_result,
                  int size) {
  unsigned long long buf = 0ull;
  unsigned inv[size];
  int carry = 1ull;
  for (int i = 0; i < size; i++) {
    inv[i] = buf_2[i];
  }
  s21_negate_data(inv, size);
  for (int i = 0; i < size; i++) {
    buf += (unsigned long long)buf_1[i] + (unsigned long long)inv[i];
    buf_result[i] = (unsigned)buf;
    buf >>= 32;
  }
}
int s21_mod10mem(unsigned *value, int size) {
  unsigned result[size];
  unsigned long long buf = 0ull;
  unsigned long long div_buf = 0ull;
  unsigned mod = 0;
  for (int i = size - 1; i >= 0; i--) {
    mod = (buf + value[i]) % 10;
    div_buf = ((buf + value[i]) / 10);
    result[i] = (unsigned)div_buf;
    buf = mod;
    buf = buf << 32;
  }
  return mod;
}

void s21_div10mem(unsigned *value, unsigned *result, int size) {
  unsigned long long buf = 0ull;
  unsigned long long div_buf = 0ull;
  unsigned mod = 0;
  for (int i = size - 1; i >= 0; i--) {
    mod = (buf + value[i]) % 10;
    div_buf = ((buf + value[i]) / 10);
    result[i] = (unsigned)div_buf;
    buf = mod;
    buf = buf << 32;
  }
}

void s21_set_scale(s21_decimal *dst, int scale) {

  dst->bits[3] |= ((scale & 0xFF) << 16);
}

void s21_div2mem(unsigned *value, int size) {
  unsigned long long buf = 0ull;
  unsigned long long div_buf = 0ull;
  unsigned mod = 0;
  for (int i = size - 1; i >= 0; i--) {
    mod = (buf + value[i]) % 2;
    div_buf = ((buf + value[i]) / 2);
    value[i] = (unsigned)div_buf;
    buf = mod;
    buf = buf << 32;
  }
}

int s21_get_sign(s21_decimal dst) { return dst.bits[3] & 0x80000000 ? 1 : 0; }
int s21_get_scale(s21_decimal value) { return (value.bits[3] >> 16) & 0xff; }
void s21_set_sign(s21_decimal *dst) { dst->bits[3] |= 0x80000000; }

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
    buf += (unsigned long long)(buf_array[i]) * (unsigned long long)integer;
    buf_array[i] = (unsigned)buf;
    buf = (buf >> 32);
  }
  for (int i = 0; i < size_r; i++) {
    result[i] = buf_array[i];
  }
  return status;
}

int s21_mul10mem(unsigned *value, int size) {
  return s21_multiply_by_integer_mem(value, 6, 10, value, 6);
}

void s21_mul_pow10mem(unsigned *value, int pow, int size) {
  for (; pow >= 8; pow -= 8) {
    s21_multiply_by_integer_mem(value, size, 100000000, value, size);
  }
  for (; pow >= 4; pow -= 4) {
    s21_multiply_by_integer_mem(value, size, 10000, value, size);
  }
  for (; pow >= 2; pow -= 2) {
    s21_multiply_by_integer_mem(value, size, 100, value, size);
  }
  for (; pow >= 1; pow -= 1) {
    s21_multiply_by_integer_mem(value, size, 10, value, size);
  }
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

int s21_is_null(s21_decimal value) {
  return (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0);
}