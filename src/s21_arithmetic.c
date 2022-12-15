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
  unsigned long long buf = 0ull;
  for (int i = 0; i < 6; i++) {
    buf += (unsigned long long)buf_1[i] + (unsigned long long)buf_2[i];
    buf_result[i] = (unsigned)buf;
    buf >>= 32;
  }
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
    err = 1;
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

void s21_rise_sign(s21_decimal *dst) { dst->bits[3] |= 0x80000000; }

void s21_clean_sign(s21_decimal *value) {
  value->bits[3] &= 0b01111111111111111111111111111111;
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
  unsigned buf_1[6] = {0, 0, 0, 0, 0, 0};
  unsigned buf_2[6] = {0, 0, 0, 0, 0, 0};
  unsigned buf_result[6] = {0, 0, 0, 0, 0, 0};
  int scale = s21_get_scale(value_1) >= s21_get_scale(value_2)
                  ? s21_get_scale(value_1)
                  : s21_get_scale(value_2);
  s21_to_scale(value_1, scale, buf_1, 6);
  s21_to_scale(value_2, scale, buf_2, 6);
  scale = s21_get_scale(value_2) - s21_get_scale(value_1);
  while (s21_is_greater(value_1, value_2)) {
    
  }

  return err;
}

int s21_mul10mem(unsigned *value, int size) {
  int status = 0;
  unsigned long long buf = 0ull;
  for (int i = 0; i < size; i++) {
    buf >>= 32;
    buf = buf + value[i] * 10;
    value[i] = (unsigned)buf;
  }
  return status;
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

// int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
//   int err = 0;
//   unsigned int buf[6] = {0};
//   unsigned int buf2[6] = {0};
//   unsigned long long tmp = 0ull;
//   int scale = s21_get_scale(value_1) + s21_get_scale(value_2);
//   for (int i = 0; i < 3; i++) {
//     for (int j = 0; j < 3; j++) {
//       tmp = value_1.bits[i] * value_2.bits[j];
//       buf2[i + j] = tmp;
//       buf2[i + j + 1] = tmp >> 32;
//       s21_sum(buf, buf2, buf, 6);
//       buf2[i + j] = 0;
//       buf2[i + j + 1] = 0;
//     }
//   }
//   while ((scale > 28 || (buf[3] || buf[4] || buf[5])) && scale >= 0) {
//     s21_div10mem(buf, buf, 6);
//     scale--;
//   }
//   if (buf[3] || buf[4] || buf[5]) {
//     err = 1;
//   } else {
//     for (int i = 0; i < 4; i++) {
//       result->bits[i] = buf[i];
//     }
//     if (s21_get_sign(value_1) == s21_get_sign(value_2))
//       s21_clean_sign(result);
//     else
//       s21_rise_sign(result);
//     s21_set_scale(result, scale);
//   }
//   return err;
// }

// int err = 0;
// s21_decimal res = {0};
// int scale1 = s21_get_scale(value_1);
// int scale2 = s21_get_scale(value_2);
// int scale = scale2 - scale1;
// int sign1 = s21_get_sign(value_1);
// int sign2 = s21_get_sign(value_2);
// value_1.bits[3] = 0;
// value_2.bits[3] = 0;
// while (s21_is_greater(value_1, value_2) && !s21_mul10(&value_2)) scale++;
// while (value_1.bits[0] || value_1.bits[1] || value_1.bits[2]) {
//   err = err || s21_mul2(&res);
//   if (s21_is_greater_or_equal(value_1, value_2)) {
//     s21_sub(value_1, value_2, &value_1);
//     res.bits[0]++;
//   }
//   s21_mul2(&value_1);
// }
// while (scale > 28) {
//   s21_div10(res, &res);
//   scale--;
// }
// while (scale < 0) {
//   err = err || s21_mul10(&res);
//   scale++;
// }
// if (sign1 == sign2)
//   s21_clean_sign(result);
// else
//   s21_rise_sign(result);
// return err;