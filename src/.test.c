/*
 * DO NOT EDIT THIS FILE. Generated by checkmk.
 * Edit the original source file "./tests/s21_arifmetics.check" instead.
 */

#include <check.h>

#line 1 "./tests/s21_arifmetics.check"
#include <check.h>

#include "s21_decimal.h"

START_TEST(s21_add_test)
{
#line 6

int fd = open("s21_add.input", O_RDONLY);

#ifdef DECIMALIZATOR
int output = open("s21_add.output", O_WRONLY | O_CREAT | O_TRUNC,
                  S_IRWXG | S_IRWXU | S_IRWXO);
#endif

s21_decimal a, b, et, res;

while (s21_decimal_deserialize(&a, fd) && s21_decimal_deserialize(&b, fd) &&
       s21_decimal_deserialize(&et, fd)) {
  s21_add(a, b, &res);
#ifndef DECIMALIZATOR
  ck_assert(s21_is_equal(et, res));
#else
  s21_decimal_serialize(res, output);
#endif
}

close(fd);

}
END_TEST

START_TEST(s21_sub_test)
{
#line 29

#ifdef DECIMALIZATOR
int output = open("s21_sub.output", O_WRONLY | O_CREAT | O_TRUNC,
                  S_IRWXG | S_IRWXU | S_IRWXO);
#endif

int fd = open("s21_sub.input", O_RDONLY);
s21_decimal a, b, et, res;

while (s21_decimal_deserialize(&a, fd) && s21_decimal_deserialize(&b, fd) &&
       s21_decimal_deserialize(&et, fd)) {
  s21_sub(a, b, &res);
#ifndef DECIMALIZATOR
  ck_assert(s21_is_equal(et, res));
#else
  s21_decimal_serialize(res, output);
#endif
}

close(fd);

}
END_TEST

START_TEST(s21_mul_test)
{
#line 51

int fd = open("s21_mul.input", O_RDONLY);

#ifdef DECIMALIZATOR
int output = open("s21_mul.output", O_WRONLY | O_CREAT | O_TRUNC,
                  S_IRWXG | S_IRWXU | S_IRWXO);
#endif

s21_decimal a, b, et, res;

while (s21_decimal_deserialize(&a, fd) && s21_decimal_deserialize(&b, fd) &&
       s21_decimal_deserialize(&et, fd)) {
  s21_mul(a, b, &res);
#ifndef DECIMALIZATOR
  ck_assert(s21_is_equal(et, res));
#else
  s21_decimal_serialize(res, output);
#endif
}

close(fd);
}
END_TEST

START_TEST(s21_div_test)
{
#line 73

int fd = open("s21_div.input", O_RDONLY);

#ifdef DECIMALIZATOR
int output = open("s21_div.output", O_WRONLY | O_CREAT | O_TRUNC,
                  S_IRWXG | S_IRWXU | S_IRWXO);
#endif

s21_decimal a, b, et, res;

while (s21_decimal_deserialize(&a, fd) && s21_decimal_deserialize(&b, fd) &&
       s21_decimal_deserialize(&et, fd)) {
  s21_div(a, b, &res);
#ifndef DECIMALIZATOR
  ck_assert(s21_is_equal(et, res));
#else
  s21_decimal_serialize(res, output);
#endif
}

close(fd);
}
END_TEST

START_TEST(s21_mod_test)
{
#line 95

int fd = open("s21_mod.input", O_RDONLY);

#ifdef DECIMALIZATOR
int output = open("s21_mod.output", O_WRONLY | O_CREAT | O_TRUNC,
                  S_IRWXG | S_IRWXU | S_IRWXO);
#endif

s21_decimal a, b, et, res;

while (s21_decimal_deserialize(&a, fd) && s21_decimal_deserialize(&b, fd) &&
       s21_decimal_deserialize(&et, fd)) {
  s21_mod(a, b, &res);
#ifndef DECIMALIZATOR
  ck_assert(s21_is_equal(et, res));
#else
  s21_decimal_serialize(res, output);
#endif
}

close(fd);
#include <check.h>

#include "s21_decimal.h"

}
END_TEST

START_TEST(s21_is_equal_test)
{
#line 6
#ifdef DECIMALIZATOR
int output = open("s21_is_equal.output", O_WRONLY | O_CREAT | O_TRUNC,
                  S_IRWXG | S_IRWXU | S_IRWXO);
#endif
int fd = open("s21_is_equal.input", O_RDONLY);
s21_decimal a, b;
int res;

while (s21_decimal_deserialize(&a, fd) && s21_decimal_deserialize(&b, fd) &&
       (read(fd, &res, sizeof(int)) > 0)) {
#ifndef DECIMALIZATOR
  ck_assert(s21_is_equal(a, b) == res);
#else
  int res = s21_is_equal(a, b);
  write(output, &res, sizeof(res));
#endif
}

close(fd);

}
END_TEST

START_TEST(s21_is_greater_test)
{
#line 27
#ifdef DECIMALIZATOR
int output = open("s21_is_greater.output", O_WRONLY | O_CREAT | O_TRUNC,
                  S_IRWXG | S_IRWXU | S_IRWXO);
#endif
int fd = open("s21_is_greater.input", O_RDONLY);
s21_decimal a, b;
int res;

while (s21_decimal_deserialize(&a, fd) && s21_decimal_deserialize(&b, fd) &&
       (read(fd, &res, sizeof(int)) > 0)) {
#ifndef DECIMALIZATOR
  ck_assert(s21_is_greater(a, b) == res);
#else
  int res = s21_is_greater(a, b);
  write(output, &res, sizeof(res));
#endif
}

close(fd);

// -----------------------------------------------------------------------

}
END_TEST

START_TEST(s21_is_less_test)
{
#line 50
#ifdef DECIMALIZATOR
int output = open("s21_is_less.output", O_WRONLY | O_CREAT | O_TRUNC,
                  S_IRWXG | S_IRWXU | S_IRWXO);
#endif
int fd = open("s21_is_less.input", O_RDONLY);
s21_decimal a, b;
int res;

while (s21_decimal_deserialize(&a, fd) && s21_decimal_deserialize(&b, fd) &&
       (read(fd, &res, sizeof(int)) > 0)) {
#ifndef DECIMALIZATOR
  ck_assert(s21_is_less(a, b) == res);
#else
  int res = s21_is_less(a, b);
  write(output, &res, sizeof(res));
#endif
}

close(fd);

}
END_TEST

START_TEST(s21_is_not_equal_test)
{
#line 71
#ifdef DECIMALIZATOR
int output = open("s21_is_not_equal.output", O_WRONLY | O_CREAT | O_TRUNC,
                  S_IRWXG | S_IRWXU | S_IRWXO);
#endif
int fd = open("s21_is_not_equal.input", O_RDONLY);
s21_decimal a, b;
int res;

while (s21_decimal_deserialize(&a, fd) && s21_decimal_deserialize(&b, fd) &&
       (read(fd, &res, sizeof(int)) > 0)) {
#ifndef DECIMALIZATOR
  ck_assert(s21_is_not_equal(a, b) == res);
#else
  int res = s21_is_not_equal(a, b);
  write(output, &res, sizeof(res));
#endif
}

close(fd);

}
END_TEST

START_TEST(s21_is_greater_or_equal_test)
{
#line 92
#ifdef DECIMALIZATOR
int output = open("s21_is_greater_or_equal.output",
                  O_WRONLY | O_CREAT | O_TRUNC, S_IRWXG | S_IRWXU | S_IRWXO);
#endif
int fd = open("s21_is_greater_or_equal.input", O_RDONLY);
s21_decimal a, b;
int res;

while (s21_decimal_deserialize(&a, fd) && s21_decimal_deserialize(&b, fd) &&
       (read(fd, &res, sizeof(int)) > 0)) {
#ifndef DECIMALIZATOR
  ck_assert(s21_is_greater_or_equal(a, b) == res);
#else
  int res = s21_is_greater_or_equal(a, b);
  write(output, &res, sizeof(res));
#endif
}

close(fd);

}
END_TEST

START_TEST(s21_is_less_or_equal_test)
{
#line 113
#ifdef DECIMALIZATOR
int output = open("s21_is_less_or_equal.output", O_WRONLY | O_CREAT | O_TRUNC,
                  S_IRWXG | S_IRWXU | S_IRWXO);
#endif
int fd = open("s21_is_less_or_equal.input", O_RDONLY);
s21_decimal a, b;
int res;

while (s21_decimal_deserialize(&a, fd) && s21_decimal_deserialize(&b, fd) &&
       (read(fd, &res, sizeof(int)) > 0)) {
#ifndef DECIMALIZATOR
  ck_assert(s21_is_less_or_equal(a, b) == res);
#else
  int res = s21_is_less_or_equal(a, b);
  write(output, &res, sizeof(res));
#endif
}

close(fd);
#include "s21_decimal.h"

}
END_TEST

START_TEST(s21_from_decimal_to_int_test)
{
#line 4
#ifdef DECIMALIZATOR
int output = open("s21_from_decimal_to_int.output",
                  O_WRONLY | O_CREAT | O_TRUNC, S_IRWXG | S_IRWXU | S_IRWXO);
#endif
int fd = open("s21_from_decimal_to_int.input", O_RDONLY);
s21_decimal a;
int res, res2;
int status = 0;
while (s21_decimal_deserialize(&a, fd) && (read(fd, &res, sizeof(res)) > 0)) {
  status = s21_from_decimal_to_int(a, &res2);
#ifndef DECIMALIZATOR

  ck_assert(res2 == res);
#else
  write(output, &status, sizeof(status));
  write(output, &res2, sizeof(res2));
#endif
}

close(fd);

}
END_TEST

START_TEST(s21_from_int_to_decimal_test)
{
#line 26
#ifdef DECIMALIZATOR
int output = open("s21_from_int_to_decimal.output",
                  O_WRONLY | O_CREAT | O_TRUNC, S_IRWXG | S_IRWXU | S_IRWXO);
#endif
int fd = open("s21_from_int_to_decimal.input", O_RDONLY);
s21_decimal a;
s21_decimal b;
int res;

while ((read(fd, &res, sizeof(int)) > 0) && s21_decimal_deserialize(&a, fd)) {
  s21_from_int_to_decimal(res, &b);
#ifndef DECIMALIZATOR

  ck_assert(s21_is_equal(a, b));
#else
  write(output, &b, sizeof(b));
#endif
}

close(fd);

}
END_TEST

START_TEST(s21_from_decimal_to_float_test)
{
#line 48
#ifdef DECIMALIZATOR
int output = open("s21_from_decimal_to_float.output",
                  O_WRONLY | O_CREAT | O_TRUNC, S_IRWXG | S_IRWXU | S_IRWXO);
#endif
int fd = open("s21_from_decimal_to_float.input", O_RDONLY);
s21_decimal a;
float res, res2;
int status = 0;
while (s21_decimal_deserialize(&a, fd) && (read(fd, &res, sizeof(res)) > 0)) {
  status = s21_from_decimal_to_float(a, &res2);
#ifndef DECIMALIZATOR

  ck_assert(res2 == res);
#else
  write(output, &status, sizeof(status));
  write(output, &res2, sizeof(res2));
#endif
}

close(fd);

}
END_TEST

START_TEST(s21_from_float_to_decimal_test)
{
#line 70
#ifdef DECIMALIZATOR
int output = open("s21_from_float_to_decimal.output",
                  O_WRONLY | O_CREAT | O_TRUNC, S_IRWXG | S_IRWXU | S_IRWXO);
#endif
int fd = open("s21_from_float_to_decimal.input", O_RDONLY);
s21_decimal a;
s21_decimal b;
float_t res;

while ((read(fd, &res, sizeof(res)) > 0) && s21_decimal_deserialize(&a, fd)) {
  s21_from_float_to_decimal(res, &b);
#ifndef DECIMALIZATOR

  ck_assert(s21_data_eq(a.bits, b.bits, 4));
#else
  s21_decimal_serialize(b, output);
#endif
}

close(fd);
#include <check.h>

#include "s21_decimal.h"

}
END_TEST

START_TEST(s21_truncate_test)
{
#line 6
int fd = open("s21_truncate.input", O_RDONLY);
#ifdef DECIMALIZATOR
int output = open("s21_truncate.output", O_WRONLY | O_CREAT | O_TRUNC,
                  S_IRWXG | S_IRWXU | S_IRWXO);
#endif

s21_decimal a, et, res;
while (s21_decimal_deserialize(&a, fd) && s21_decimal_deserialize(&et, fd)) {
  s21_truncate(a, &res);
#ifndef DECIMALIZATOR
  ck_assert((s21_is_equal(et, res)));
#else
  s21_decimal_serialize(res, output);
#endif
}

close(fd);

}
END_TEST

START_TEST(s21_negate_test)
{
#line 25
int fd = open("s21_negate.input", O_RDONLY);
#ifdef DECIMALIZATOR
int output = open("s21_negate.output", O_WRONLY | O_CREAT | O_TRUNC,
                  S_IRWXG | S_IRWXU | S_IRWXO);
#endif

s21_decimal a, et, res;
while (s21_decimal_deserialize(&a, fd) && s21_decimal_deserialize(&et, fd)) {
  s21_negate(a, &res);
#ifndef DECIMALIZATOR
  ck_assert((s21_is_equal(et, res)));
#else
  s21_decimal_serialize(res, output);
#endif
}

close(fd);

}
END_TEST

START_TEST(s21_floor_test)
{
#line 44
int fd = open("s21_floor.input", O_RDONLY);
#ifdef DECIMALIZATOR
int output = open("s21_floor.output", O_WRONLY | O_CREAT | O_TRUNC | O_TRUNC,
                  S_IRWXG | S_IRWXU | S_IRWXO);
#endif

s21_decimal a, et, res;
while (s21_decimal_deserialize(&a, fd) && s21_decimal_deserialize(&et, fd)) {
  s21_floor(a, &res);
#ifndef DECIMALIZATOR
  ck_assert((s21_is_equal(et, res)));
#else
  s21_decimal_serialize(res, output);
#endif
}

close(fd);

}
END_TEST

START_TEST(s21_round_test)
{
#line 63
int fd = open("s21_round.input", O_RDONLY);
#ifdef DECIMALIZATOR
int output = open("s21_round.output", O_WRONLY | O_CREAT | O_TRUNC | O_TRUNC,
                  S_IRWXG | S_IRWXU | S_IRWXO);
#endif

s21_decimal a, et, res;
while (s21_decimal_deserialize(&a, fd) && s21_decimal_deserialize(&et, fd)) {
  s21_round(a, &res);
#ifndef DECIMALIZATOR
  ck_assert((s21_is_equal(et, res)));
#else
  s21_decimal_serialize(res, output);
#endif
}

close(fd);
}
END_TEST

int main(void)
{
    Suite *s1 = suite_create("Core");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc1_1);
    tcase_add_test(tc1_1, s21_add_test);
    tcase_add_test(tc1_1, s21_sub_test);
    tcase_add_test(tc1_1, s21_mul_test);
    tcase_add_test(tc1_1, s21_div_test);
    tcase_add_test(tc1_1, s21_mod_test);
    tcase_add_test(tc1_1, s21_is_equal_test);
    tcase_add_test(tc1_1, s21_is_greater_test);
    tcase_add_test(tc1_1, s21_is_less_test);
    tcase_add_test(tc1_1, s21_is_not_equal_test);
    tcase_add_test(tc1_1, s21_is_greater_or_equal_test);
    tcase_add_test(tc1_1, s21_is_less_or_equal_test);
    tcase_add_test(tc1_1, s21_from_decimal_to_int_test);
    tcase_add_test(tc1_1, s21_from_int_to_decimal_test);
    tcase_add_test(tc1_1, s21_from_decimal_to_float_test);
    tcase_add_test(tc1_1, s21_from_float_to_decimal_test);
    tcase_add_test(tc1_1, s21_truncate_test);
    tcase_add_test(tc1_1, s21_negate_test);
    tcase_add_test(tc1_1, s21_floor_test);
    tcase_add_test(tc1_1, s21_round_test);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
