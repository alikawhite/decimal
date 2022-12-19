#include "s21_decimal.h"

int main() {
  s21_decimal first = {
      {2147483647, 2147483647, 2147483647, 0b10000000000001110000000000000000}};
  s21_decimal second = {
      {2147483646, 2147483647, 2147483647, 0b10000000000001110000000000000000}};
  int err = s21_is_less(first, second);
  int error = 1;
  if (err == error)
    printf("ok");
  else
    printf("no");
  return 0;
}