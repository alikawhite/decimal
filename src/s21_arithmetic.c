#include "s21_decimal.h"

int s21_get_scale(s21_decimal value) { return (value.bits[3] >> 16) & 0xff; }

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
  s21_data_add(buf_1, buf_2, buf_result, 6);
  while (scale && (buf_result[5] || buf_result[4] || buf_result[3])) {
    s21_div10mem(result, result, 6);
    scale--;
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
  for (int i = 0; i < size; i++) value[i] = ~value[i];
  unsigned long long buf = 1ull;
  for (int i = 0; i < size; i++) {
    buf += value[i];
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
  int sign = 0;
  if (s21_get_sign(value_1) == s21_get_sign(value_2)) {
    sign = 0;
  } else {
    sign = 1;
  }
  int scale = s21_get_scale(value_1) + s21_get_scale(value_2);
  unsigned buf_result[6] = {0, 0, 0, 0, 0, 0};
  unsigned buf_add[6] = {0, 0, 0, 0, 0, 0};
  unsigned long long buf = 0ull;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; i < 3; j++) {
      buf = (unsigned long long)value_1.bits[i] *
            (unsigned long long)value_2.bits[j];
      buf_add[i + j] = (unsigned)buf;
      buf >>= 32;
      buf_add[i + j + 1] = (unsigned)buf;
      buf = 0ull;
      for (int k = 0; k < 6; k++) {
        buf +=
            (unsigned long long)buf_result[k] + (unsigned long long)buf_add[k];
        buf_result[k] = (unsigned)buf;
        buf >>= 32;
      }
      buf = 0ull;
      buf_add[i + j] = 0;
      buf_add[i + j + 1] = 0;
    }
  }
  while (scale && (buf_result[5] || buf_result[4] || buf_result[3])) {
    s21_div10mem(result, result, 6);
    scale--;
  }
  if ((!(buf_result[5] || buf_result[4] || buf_result[3]) && (scale <= 28))) {
    for (int i = 0; i < 3; i++) {
      result->bits[i] = buf_result[i];
    }
    result->bits[3] = sign << 31;
    s21_set_scale(result, scale);
  } else {
    err = 1 + sign;
  }
  return err;
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
  unsigned int mask = 0;
  mask = mask | scale;
  dst->bits[3] = dst->bits[3] | (mask << 16);
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int err = 0;
  int sign = 0;
  if (s21_get_sign(value_1) == s21_get_sign(value_2)) {
    sign = 0;
  } else {
    sign = 1;
  }
  int pow_2 = 0;
  unsigned buf_1[6] = {0, 0, 0, 0, 0, 0};
  unsigned buf_2[6] = {0, 0, 0, 0, 0, 0};
  unsigned buf_result[6] = {0, 0, 0, 0, 0, 0};
  int scale = s21_get_scale(value_1) >= s21_get_scale(value_2)
                  ? s21_get_scale(value_1)
                  : s21_get_scale(value_2);
  s21_to_scale(value_1, scale, buf_1, 6);
  s21_to_scale(value_2, scale, buf_2, 6);
  scale = s21_get_scale(value_2) - s21_get_scale(value_1);
  while (s21_data_gt(buf_1, buf_2, 6)) {
    s21_mul10mem(&buf_2, 6);
    scale++;
  }
  while (s21_data_gt(buf_2, buf_1, 6)) {
    s21_div2mem(&buf_1, 6);
    pow_2++;
  }
  unsigned buf_2_inv[6] = {0, 0, 0, 0, 0, 0};
  s21_negate_data(buf_2_inv, 6);
  while (buf_1[0] || buf_1[1] || buf_1[2] || buf_1[3] || buf_1[4] || buf_1[5]) {
    s21_mul2mem(buf_result, 6);
    if (s21_data_gt(buf_1, buf_2, 6) || s21_data_eq(buf_1, buf_2, 6)) {
      s21_data_add(buf_1, buf_2_inv, buf_1, 6);
    }
  }
  while (pow_2--) {
    s21_div2mem(&buf_result, 6);
  }
  while (scale++ < 0) {
    s21_mul10mem(buf_result, 6);
  }
  while (scale && (buf_result[5] || buf_result[4] || buf_result[3])) {
    s21_div10mem(result, result, 6);
    scale--;
  }
  if ((!(buf_result[5] || buf_result[4] || buf_result[3]) && (scale <= 28))) {
    for (int i = 0; i < 3; i++) {
      result->bits[i] = buf_result[i];
    }
    result->bits[3] = sign << 31;
    s21_set_scale(result, scale);
  } else {
    err = 1 + sign;
  }
  return err;
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
