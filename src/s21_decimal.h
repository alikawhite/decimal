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
#define MAXLIMIT_ERROR 1 // число слишком велико или равно бесконечности
#define MINLIMIT_ERROR                                                         \
  2 // число слишком мало или равно отрицательной бесконечности
#define DIVBYZERO_ERROR 3 // деление на 0
#define LIM 1e-28
#define CONVERTING_ERROR 1
#define FRACTIONAL 8

#define S21_OK 0

typedef struct {
  unsigned bits[4];
} s21_decimal;

extern const s21_decimal s21_zero;

// Основные функции

// Перевод в decimal и обратно
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);

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

int s21_data_eq(const unsigned *data1, const unsigned *data2, int size);
int s21_data_gt(const unsigned *data1, const unsigned *data2, int size);
int s21_is_null(s21_decimal value);
int s21_inc(s21_decimal *result);
int s21_sum(const unsigned *val1, int sign1, const unsigned *val2, int sign2,
            unsigned *result, int *sign_result, int size);
int s21_decimal_serialize(s21_decimal value, int fd);
int s21_decimal_deserialize(s21_decimal *value, int fd);
int s21_div10(s21_decimal value, s21_decimal *result);
int s21_mod10(s21_decimal value);
int s21_get_scale(s21_decimal value);
void s21_to_scale(s21_decimal value, int scale, unsigned *result, int size);
void s21_negate_data(unsigned *value, int size);
void s21_div2mem(unsigned *value, int size);
int s21_mul2mem(unsigned *value, int size);
void s21_div10mem(unsigned *value, unsigned *result, int size);
void s21_data_add(unsigned *buf_1, unsigned *buf_2, unsigned *buf_result,
                  int size);
void s21_set_scale(s21_decimal *dst, int scale);
int s21_mul10mem(unsigned *value, int size);
int s21_mod10mem(unsigned *value, int size);
void s21_set_sign(s21_decimal *dst);
int s21_get_sign(s21_decimal dst);
int s21_get_exponent(s21_decimal value);
int s21_divide_by_power_of_10(s21_decimal value, int power,
                              s21_decimal *result);
int s21_divide_by_integer(s21_decimal value, int integer, s21_decimal *result);
int s21_modulo_by_integer(s21_decimal value, int integer);
int s21_add_integer(s21_decimal value, int integer, s21_decimal *result);
int s21_multiply_by_integer_mem(unsigned *value, int size_v, int integer,
                                unsigned *result, int size_r);
void s21_data_sub(unsigned *buf_1, unsigned *buf_2, unsigned *buf_result,
                  int size);
void s21_mul_pow10mem(unsigned *value, int pow, int size);

void s21_last_zeroes(s21_decimal *value);
int s21_get_bit(s21_decimal d, int i);
int s21_get_bit_mem(unsigned value, int i);
#endif
