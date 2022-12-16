#include "s21_decimal.h"

int s21_mod10(s21_decimal value) {
  unsigned long long buf = 0ull;
  unsigned long long div_buf = 0ull;
  unsigned mod = 0;
  for (int i = 2; i >= 0; i--) {
    mod = (buf + value.bits[i]) % 10;
    div_buf = ((buf + value.bits[i]) / 10);
    value.bits[i] = (unsigned)div_buf;
    buf = mod;
    buf = buf << 32;
  }
  return mod;
}
int s21_get_sign(s21_decimal dst) { return dst.bits[3] & 0x80000000 ? 1 : 0; }

void s21_to_zero(s21_big_decimal *dst) {
  for (int i = 0; i < 7; i++) {
    dst->bits[i] = 0;
  }
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int err = 0;
  int sign = 0;
  unsigned buf_1[6] = {0, 0, 0, 0, 0, 0};
  unsigned buf_2[6] = {0, 0, 0, 0, 0, 0};
  unsigned buf_result[6] = {0, 0, 0, 0, 0, 0};
  for (int i = 0; i < 3; i++) {
    buf_1[i] = value_1.bits[i];
    buf_2[i] = value_2.bits[i];
  }
  int scale = s21_get_scale(value_1) >= s21_get_scale(value_2)
                  ? s21_get_scale(value_1)
                  : s21_get_scale(value_2);
  s21_to_scale(value_1, scale, buf_1, 6);
  s21_to_scale(value_2, scale, buf_2, 6);
  if (s21_get_sign(value_1) == s21_get_sign(value_2)) {
    sign = s21_get_sign(value_1);
  } else {
    if (s21_data_gt(buf_1, buf_2, 6)) {
      s21_negate_data(buf_2, 6);
      sign = s21_get_sign(value_1);
    } else {
      s21_negate_data(buf_1, 6);
      sign = s21_get_sign(value_2);
    }
  }
  int last_digit = 0;
  s21_data_add(buf_1, buf_2, buf_result, 6);
  while ((scale && (buf_result[5] || buf_result[4] || buf_result[3])) ||
         scale > 28) {
    last_digit = s21_mod10mem(buf_result, 6);
    s21_div10mem(buf_result, buf_result, 6);
    scale--;
  }
  if (last_digit >= 5) {
    unsigned carry[6] = {1, 0, 0, 0, 0, 0};
    s21_data_add(buf_result, carry, buf_result, 6);
  }
  if (!(buf_result[5] || buf_result[4] || buf_result[3])) {
    for (int i = 0; i < 3; i++) {
      result->bits[i] = buf_result[i];
    }
    result->bits[3] = sign << 31;
    s21_set_scale(result, scale);
  } else {
    err = 1;
  }
  return err;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_negate(value_2, &value_2);
  return s21_add(value_1, value_2, result);
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

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int err = 0;
  int sign = s21_get_sign(value_1) == s21_get_sign(value_2) ? 0 : 1;
  int scale = s21_get_scale(value_1) + s21_get_scale(value_2);

  unsigned buf_1[6] = {
      value_1.bits[0], value_1.bits[1], value_1.bits[2], 0, 0, 0};
  unsigned buf_2[6] = {
      value_2.bits[0], value_2.bits[1], value_2.bits[2], 0, 0, 0};
  unsigned buf_result[6] = {0, 0, 0, 0, 0, 0};
  while (buf_1[0] || buf_1[1] || buf_1[2]) {
    if (buf_1[0] & 1) {
      s21_data_add(buf_result, buf_2, buf_result, 6);
    }
    s21_div2mem(buf_1, 6);
    s21_data_add(buf_2, buf_2, buf_2, 6);
  }
  int last_digit = 0;
  while ((scale && (buf_result[5] || buf_result[4] || buf_result[3])) ||
         scale > 28) {
    last_digit = s21_mod10mem(buf_result, 6);
    s21_div10mem(buf_result, buf_result, 6);
    scale--;
  }
  if (last_digit >= 5) {
    unsigned carry[6] = {1, 0, 0, 0, 0, 0};
    s21_data_add(buf_result, carry, buf_result, 6);
  }
  if (!(buf_result[5] || buf_result[4] || buf_result[3])) {
    for (int i = 0; i < 3; i++) {
      result->bits[i] = buf_result[i];
    }
    result->bits[3] = sign << 31;
    s21_set_scale(result, scale);
  } else {
    err = 1;
  }
  return err;
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

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {}

void s21_rise_sign(s21_decimal *dst) { dst->bits[3] |= 0x80000000; }

void s21_clean_sign(s21_decimal *value) {
  value->bits[3] &= 0b01111111111111111111111111111111;
}

void s21_set_scale(s21_decimal *dst, int scale) {

  dst->bits[3] = (dst->bits[3] & 0x80000000) | ((scale & 0xFF) << 16);
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int err = 0;
  if (value_2.bits[0] == 0 && value_2.bits[1] == 0 && value_2.bits[2] == 0) {
    err = 1;
  } else {
    int sign = s21_get_sign(value_1) == s21_get_sign(value_2) ? 0 : 1;
    int scale = s21_get_scale(value_2) - s21_get_scale(value_1);
    // -----------------------------------------------------------------------
    unsigned buf_1[6] = {
        value_1.bits[0], value_1.bits[1], value_1.bits[2], 0, 0, 0};
    unsigned buf_2[6] = {
        0, 0, 0, value_2.bits[0], value_2.bits[1], value_2.bits[2]};
    // unsigned buf_2_inv[6] = {0, 0, 0, 0, 0, 0};
    unsigned buf_result[6] = {0, 0, 0, 0, 0, 0};
    while ((buf_1[0] || buf_1[1] || buf_1[2]) &&
           (buf_2[0] || buf_2[1] || buf_2[2])) {
      s21_data_add(buf_result, buf_result, buf_result, 6);
      if (s21_data_gt(buf_1, buf_2, 6) || s21_data_eq(buf_1, buf_2, 6)) {

        s21_negate_data(buf_2, 6);
        s21_data_add(buf_1, buf_2, buf_1, 6);
        s21_negate_data(buf_2, 6);
        buf_result[0]++;
      }
      s21_div2mem(buf_2, 6);
    }
    // -----------------------------------------------------------------------

    int last_digit = 0;
    while ((scale && (buf_result[5] || buf_result[4] || buf_result[3])) ||
           scale > 28) {
      last_digit = s21_mod10mem(buf_result, 6);
      s21_div10mem(buf_result, buf_result, 6);
      scale--;
    }
    if (last_digit >= 5) {
      unsigned carry[6] = {1, 0, 0, 0, 0, 0};
      s21_data_add(buf_result, carry, buf_result, 6);
    }

    if (!(buf_result[5] || buf_result[4] || buf_result[3])) {
      for (int i = 0; i < 3; i++) {
        result->bits[i] = buf_result[i];
      }
      result->bits[3] = sign << 31;
      s21_set_scale(result, scale);
    } else {
      err = 1;
    }
    return err;
  }
}

int s21_mul2mem(unsigned *value, int size) {
  int status = 0;
  unsigned long long buf = 0ull;
  for (int i = 0; i < size; i++) {
    buf >>= 32;
    buf = buf + (unsigned long long)value[i] * 2;
    value[i] = (unsigned)buf;
  }
  return status;
}

int s21_mul10mem(unsigned *value, int size) {
  int status = 0;
  unsigned long long buf = 0ull;
  for (int i = 0; i < size; i++) {
    buf >>= 32;
    buf = buf + (unsigned long long)value[i] * 10;
    value[i] = (unsigned)buf;
  }
  return status;
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

int s21_mul10(s21_decimal *value) {
  int status = 0;
  s21_decimal result = {0};
  unsigned long long buf = 0ull;
  for (int i = 0; i < 3; i++) {
    buf >>= 32;
    buf = buf + result.bits[i] * 10;
    result.bits[i] = (unsigned)buf;
  }
  status = buf >> 32;
  if (!status) {
    for (int i = 0; i < 3; i++) {
      value->bits[i] = result.bits[i];
    }
  }
  return status;
}

int s21_mul2(s21_decimal *value) {
  int status = 0;
  s21_decimal result = {0};
  unsigned long long buf = 0ull;
  for (int i = 0; i < 3; i++) {
    buf >>= 32;
    buf = buf + result.bits[i] * 2;
    result.bits[i] = (unsigned)buf;
  }
  status = buf >> 32;
  if (!status) {
    for (int i = 0; i < 3; i++) {
      value->bits[i] = result.bits[i];
    }
  }
  return status;
}
