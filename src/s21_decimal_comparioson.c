#include "s21_decimal.h"

int main() {
  s21_decimal src1, src2;
  int origin;
  // src1 = -9878798789.5867800;
  // src2 = -9878798789.58678;
  src1.bits[0] = 0b11001110100110111101100110011000;
  src1.bits[1] = 0b00000001010111101111011100100110;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b10000000000001110000000000000000;
  src2.bits[0] = 0b10110101010001000001111001010110;
  src2.bits[1] = 0b00000000000000111000001001111000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000001010000000000000000;
  // unsigned die = {0};

  // die = src1.bits[0];
  // for (int i = 0; i < 4; i++) {
  //   printf("%x88  ", die[i]);
  // }
  s21_last_zeroes(&src1);
  int result = s21_is_greater(src1, src2);
  origin = 0;
  if (origin == result)
    printf("ok");
  else
    printf("no");
  return 0;
}

// int s21_get_bit(s21_decimal d, int i) {
//   unsigned int mask = 1u << (i % 32);
//   int r = 0;
//   if (i <= 95 && i >= 0) {
//     r = d.bits[i / 32] & mask;
//   }
//   return r;
// }

// int s21_get_bit_mem(unsigned int value, int i) {
//   unsigned int mask = 1u << (i % 32);
//   int r = 0;
//     r = value[0] & mask;
//   }
//   return r;
// }

void s21_last_zeroes(s21_decimal *value) {
  // int digit = 0;
  unsigned buf[3 n] = {0, 0, 0};
  for (int i = 0; i < 3; i++) {
    buf[i] = value->bits[i];
  }
  while ((buf[0] << 31) != 1) {
    s21_mul10mem(buf, 3);
  }
  // return digit;
}

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
      if (s21_get_sign(value_1))
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
