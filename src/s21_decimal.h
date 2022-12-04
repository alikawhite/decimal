#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <stdio.h>
#include <math.h>

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
int setSign(s21_decimal *dst);
int getBit(s21_decimal d, int i);
int setBit(s21_decimal *dst, int i);
int setScale(s21_decimal* dst, int scale);
int getScale(s21_decimal src);
int getExp(float src);

void big_decimal (s21_decimal value, s21_big_decimal *result);

#endif

