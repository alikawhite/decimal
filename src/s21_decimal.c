#include "s21_decimal.h"

// убрать отсюда нахуй эту константу, чтобы не доебались

const s21_decimal s21_zero = {0};

union test {
  float asFloat;
  long int asInteger;
};

int getBit(s21_decimal d, int i) {
  unsigned int mask = 1u << (i % 32);
  int r = 0;
  if (i <= 95 && i >= 0) {
    r = d.bits[i / 32] & mask;
  }
  return r;
}

int getExp(float src) {
  unsigned int bits = *((unsigned int *)&src);
  int res = 0;
  bits <<= 1;
  res = res | (bits >> 24);
  return res - 127;
}

int setBit(s21_decimal *dst, int i) {
  int err = 0;
  unsigned int mask = 1;
  if (i >= 0 && i <= 31) {
    dst->bits[0] = dst->bits[0] | (mask << i);
  } else if (i >= 32 && i <= 63) {
    dst->bits[1] = dst->bits[1] | (mask << (i - 32));
  } else if (i >= 64 && i <= 95) {
    dst->bits[2] = dst->bits[2] | (mask << (i - 64));
  } else {
    err = -1;
  }
  return err;
}

void setScale(s21_decimal *dst, int scale) {
  unsigned int mask = 0;
  mask = mask | scale;
  dst->bits[3] = dst->bits[3] | (mask << 16);
}

int getScale(s21_decimal src) {
  src.bits[3] <<= 8;
  unsigned int res = src.bits[3] >> 24;
  return res;
}


int shift(s21_decimal *dst, int opt) {
  if (opt == 1) {
    for (int i = 96; i != 0; i--) {
      if (getBit(*dst, i - 1)) {
        setBit(dst, i);
      } else {
        cleanBit(dst, i);
      }
    }
    cleanBit(dst, 0);
  } else if (opt == -1) {
    for (int i = 0; i < 96; i++) {
      if (getBit(*dst, i + 1)) {
        setBit(dst, i);
      } else {
        cleanBit(dst, i);
      }
    }
    cleanBit(dst, 95);
  }
  return 1;
}

int cleanBit(s21_decimal *dst, int i) {
  int err = 1;
  unsigned int mask = 1;
  if (i >= 0 && i <= 31) {
    dst->bits[0] = dst->bits[0] & (~(mask << i));
  } else if (i >= 32 && i <= 63) {
    dst->bits[1] = dst->bits[1] & (~(mask << (i - 32)));
  } else if (i >= 64 && i <= 95) {
    dst->bits[2] = dst->bits[2] & (~(mask << (i - 64)));
  } else {
    err = -1;
  }
  return err;
}

void cleanSign(s21_decimal *value) {
  value->bits[3] &= 0b01111111111111111111111111111111;
}

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

int s21_inc(s21_decimal *result) {
  unsigned long long buf = 0ull;
  for (int i = 0; i < 3; i++) {
    buf += result->bits[i] + 1;
    result->bits[i] = (unsigned)buf;
    buf >>= 32;
  }
  return 0;
}

int s21_sum(const unsigned *val1, int sign1, const unsigned *val2, int sign2,
            unsigned *result, int *sign_result, int size) {
  int status = 0;
  if (sign1 == sign2) {
    unsigned long long buf = 0ull;
    for (int i = 0; i < size; i++) {
      buf += val1[i] + val2[i];
      result[i] = (unsigned)buf;
      buf >>= 32;
    }
    *sign_result = sign1;
  } else if (!sign1 > !sign2) {
    unsigned long long buf = 0ull;
    for (int i = 0; i < size; i++) {
      buf = buf + (unsigned long long)val1[i] - (unsigned long long)val2[i];
      result[i] = (unsigned)buf;
      buf >>= 32;
    }
    *sign_result = (buf >> 31);
  } else {
    status = s21_sum(val2, sign2, val1, sign1, result, sign_result, size);
  }
  return status;
}

