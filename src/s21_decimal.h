#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <fcntl.h>
#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define is_nan(x) __builtin_isnan(x)
#define MAXLIMIT_ERROR 1  // число слишком велико или равно бесконечности
#define MINLIMIT_ERROR 2  // число слишком мало или равно отрицательной бесконечности
#define DIVBYZERO_ERROR 3  // деление на 0
#define LIM 1e-28
#define CONVERTING_ERROR 1
#define FRACTIONAL 8

typedef struct{
    int bits[4];
} s21_decimal;

typedef struct {
  unsigned int bits[7];
} s21_big_decimal;

extern const s21_decimal s21_zero;

// Основные функции


// Перевод в decimal и обратно
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// Арифметические операторы

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// Операторы сравнения

int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);
int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal, s21_decimal);
int s21_is_equal(s21_decimal, s21_decimal);
int s21_is_not_equal(s21_decimal, s21_decimal);

// Дополнительные функции
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

/* Дополнительные функции - тут наши функции,
которые могут понадобиться в процессе работы */

int s21_init_func(s21_decimal *dst);
int getSign(s21_decimal dst);
void setSign(s21_decimal *dst);
int getBit(s21_decimal d, int i);
int setBit(s21_decimal *dst, int i);
void setScale(s21_decimal* dst, int scale);
int getScale(s21_decimal src);
int getExp(float src);
int cleanBit(s21_decimal* dst, int i);
int shift(s21_decimal* d, int opt);


int big_to_dec(s21_big_decimal big_d, s21_decimal* d);

s21_decimal divTen(s21_decimal src);
s21_big_decimal div_big_end(s21_big_decimal value, s21_big_decimal divider, s21_big_decimal *reminder);

void big_decimal(s21_decimal value, s21_big_decimal *result);
int big_getScale(s21_big_decimal src);
int big_setBit(s21_big_decimal* dst, int i);
int big_getBit(s21_big_decimal dst, int i);
int big_getSign(s21_big_decimal dst);
void big_cleanBit(s21_big_decimal* dst, int i);
void big_cleanScale(s21_big_decimal* dst);
void big_setSign(s21_big_decimal *dst);
void big_setScale(s21_big_decimal* dst, int scale);
int big_not_null(s21_big_decimal dst);
void big_inverce(s21_big_decimal b, s21_big_decimal* d);

void to_zero(s21_big_decimal* dst);
void big_shift(s21_big_decimal *dst, int opt);
void big_add(s21_big_decimal a, s21_big_decimal b, s21_big_decimal *result);
int big_sub(s21_big_decimal first, s21_big_decimal second, s21_big_decimal* result);
void to_one_scale(s21_decimal *value1, s21_decimal *value2, s21_big_decimal* first, s21_big_decimal *second);
void scale_up(s21_big_decimal *dst, int value, s21_big_decimal *result);
void big_mult(s21_big_decimal a, s21_big_decimal b, s21_big_decimal *result);
void big_add(s21_big_decimal a, s21_big_decimal b, s21_big_decimal *result);


int s21_data_eq(const unsigned *data1, const unsigned *data2, int size);
int s21_data_gt(const unsigned *data1, int sign1, const unsigned *data2,
                int sign2, int size);
int s21_is_null(s21_decimal value);
int s21_inc(s21_decimal *result);
int s21_sum(const unsigned *val1, int sign1, const unsigned *val2, int sign2,
            unsigned *result, int *sign_result, int size);
// int s21_div2(const unsigned *val1, const unsigned *val2, unsigned *result,
            //  int size);
int s21_decimal_serialize(s21_decimal value, int fd);
int s21_decimal_deserialize(s21_decimal *value, int fd);
int s21_div10(s21_decimal value, s21_decimal *result);
int s21_mul10(s21_decimal value, s21_decimal *result);
int s21_mod10(s21_decimal value);
int s21_get_scale(s21_decimal value);

#endif

