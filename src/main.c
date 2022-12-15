#include "s21_decimal.h"

int main() {

  int fd = open("s21_mul.input", O_RDONLY);

  int output = open("s21_mul.output", O_WRONLY | O_CREAT | O_TRUNC,
                    S_IRWXG | S_IRWXU | S_IRWXO);

  s21_decimal a, b, et, res;

  while (s21_decimal_deserialize(&a, fd) && s21_decimal_deserialize(&b, fd) &&
         s21_decimal_deserialize(&et, fd)) {
    s21_mul(a, b, &res);

    s21_decimal_serialize(res, output);
  }

  close(fd);

  fd = open("s21_div.input", O_RDONLY);

  output = open("s21_div.output", O_WRONLY | O_CREAT | O_TRUNC,
                S_IRWXG | S_IRWXU | S_IRWXO);

  while (s21_decimal_deserialize(&a, fd) && s21_decimal_deserialize(&b, fd) &&
         s21_decimal_deserialize(&et, fd)) {
    s21_div(a, b, &res);

    s21_decimal_serialize(res, output);
  }

  close(fd);

  return 0;
}