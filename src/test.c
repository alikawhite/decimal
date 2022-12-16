#include "s21_decimal.h"

int main() {
  s21_decimal a;
  float b = 345.1111112;
  s21_from_float_to_decimal(b, &a);
  return 0;
}

// for (int i = 0; i < 4; i++) {
//   printf("|%d|", dst->bits[i]);
// }

// if (sign_int) {
// for (long unsigned int i = 0; i < strlen(flts); i++) {
//   flts[i] = flts_tmp[i + 1];
// }
//   setSign(dst);
// } else {
//   strcpy(flts_tmp, flts);
// }

// int sign_int = 0;
// if (src < 0) sign_int = 1;

// printf("\n");
// for (long unsigned int i = 0; i < strlen(flts); i++) {
//   printf("%c ", flts[i]);
// }

// char str[64];
// int n = 0;
// int flag = 0;
// int m = 0;
// for (unsigned long int i = 0; i < strlen(flts); i++) {
//   if (flts[i] != 46 && !flag) {
//     str[m] = flts[i];
//     m++;
//   } else {
//     flag = 1;
//   }
//   if (flts[i] != 46 && flag) {
//     fract[n] = flts[i];
//     n++;
//   }
// }

// int s21_from_float_to_decimal(float src, s21_decimal *dst) {
//   int result = 0;
//   for (int i = 0; i < 4; i++) {
//     dst->bits[i] = 0;
//   }
//   if ((src != src) | (fabsf(src) > 7.922816e28) | (fabsf(src) == INFINITY) |
//       (fabsf(src) < 1e-28)) {
//     if (fabsf(src) != 0) {
//       result = 1;
//     }
//   } else {
//     char flts[64];
//     sprintf(flts, "%.6e", src);
//     int sign_int = 1;
//     if (src > 0) sign_int = 0;

//     if (sign_int) {
//       for (long unsigned int i = 0; flts[i] != '\0'; i++) {
//         flts[i] = flts[i + 1];
//       }
//       setSign(dst);
//     }
//     char numb[64];
//     int j = 0;
//     for (unsigned long int i = 0; i < strlen(flts); i++) {
//       if (flts[i] == 'e') {
//         numb[j] = '\0';
//         break;
//       }
//       if (flts[i] != 46) {
//         numb[j] = flts[i];
//         j++;
//       }
//     }
//     int _exp10 = 0;
//     int _mantissa = 0;
//     int tmp = 0;
//     char sign = flts[8];

//     sscanf(numb, "%de%c%d", &_mantissa, &sign, &_exp10);
//     sscanf(flts, "%d.%de%c%d", &tmp, &tmp, &numb[0], &_exp10);
//     dst->bits[0] = _mantissa;
//     setScale(dst, _exp10);

//   }
//   return result;
// }