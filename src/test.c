#include "s21_decimal.h"

const s21_decimal s21_zero = {0};

int s21_from_float_to_decimal(float src, s21_decimal *dst){
    *dst = s21_zero;
    int result = 0;
    if(isinf(src) || isnan(src)) result = 1;// is src infinity
    if((0 < src) && (src < 1e-28)) result = 1;
    if(result == 0){
        int n = intpart(src);
        while (n <= 29)
        {
            
        }
        
    }
    int intermidNumber = 0;
    int scale = 0;
    if(src < 0) {
        
    }
    return 0;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int dec1_bit;
    int dec2_bit;
    int sub_result_bit = 0;
    s21_decimal_init(result);
    for(int i = 0; i < 96; i++) {
        dec1_bit = s21_get_bit(&value_1, i);
        dec2_bit = s21_get_bit(&value_2, i);
        s21_set_bit(result, i, dec1_bit^dec2_bit^sub_result_bit);
        sub_result_bit = (dec1_bit && dec2_bit) || (dec1_bit && sub_result_bit)
                        || (dec2_bit && sub_result_bit);
    }
    return sub_result_bit;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int dec1_bit;
    int dec2_bit;
    int sub_result_bit = 0;
    s21_decimal_init(result);
    for(int i = 0; i < 96; i++) {
        dec1_bit = s21_get_bit(&value_1, i);
        dec2_bit = s21_get_bit(&value_2, i);
        s21_set_bit(result, i, dec1_bit^dec2_bit);
        if(!dec1_bit && dec2_bit) {
            int k = i;
            while(s21_get_bit(&value_1, k) != 1) {
                s21_set_bit(&value_1, k, 1);
                k++;
            }
            s21_set_bit(&value_1, k, 0);
        }
    }
    return sub_result_bit;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    s21_decimal_init(result);
    s21_decimal sub_result;
    for(int i = 0; i < 96; i++) {
        s21_decimal_init(&sub_result);
        if(s21_get_bit(&value_2, i) == 1) {
            sub_result = value_1;
            int j = 0;
            while (j < i) {
                s21_shift(&sub_result);
                j++;
            }
        }
        s21_add(sub_result, *result, result);
    }
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    s21_decimal_init(result);
    *result = value_1;
    int count = 0;
    while(1) {
        s21_sub(*result, value_2, result);
        count++;
        if(s21_is_greater(*result, value_2)) {
            break;
        }
    }
    s21_from_int_to_decimal(count, result);
    return 0;
}

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    s21_div(value_1, value_2, result);
    long long int mod = 0;
    s21_from_decimal_to_int(*result, &mod);
    return mod;
}

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
    int sign = 0;
    for(int i = 95; i >= 0; i--) {
        if(s21_get_bit(&value_1, i)^s21_get_bit(&value_2, i)) {
            sign = s21_get_bit(&value_1, i);
            break;
        }
    }
    return sign;
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
    int sign = 0;
    if(!(s21_is_greater(value_1, value_2))) {
        for(int i = 95; i >= 0; i--) {
            if(s21_get_bit(&value_1, i)^s21_get_bit(&value_2, i)) {
                sign = s21_get_bit(&value_1, i);
            } else if(!(s21_get_bit(&value_1, i)^s21_get_bit(&value_2, i))) {
                sign = !(s21_get_bit(&value_1, i));
            }
        }
    }
    return sign;
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
    int sign = 0;
    for(int i = 95; i >= 0; i--) {
        if(s21_get_bit(&value_1, i)^s21_get_bit(&value_2, i)) {
            sign = !(s21_is_less(value_1, value_2));
        }
    }
    return sign;
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
    int sign = 0;
    if(!(s21_is_less(value_1, value_2))) {
        for(int i = 95; i >= 0; i--) {
            if(s21_get_bit(&value_1, i)^s21_get_bit(&value_2, i)) {
                sign = 1;
            } else {
                sign = s21_is_equal(value_1, value_2);
            }
            }
        }
    return sign;
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
    int sign = 1;
    for(int i = 95; i >= 0; i--) {
        if((s21_get_bit(&value_1, i) != s21_get_bit(&value_2, i))) {
            sign = 0;
            break;
        }
    }
    return sign;
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
    int sign = s21_is_equal(value_1, value_2);
    return !sign;
}

int s21_from_int_to_decimal(long long int src, s21_decimal *dst) {
    s21_decimal_init(dst);
    int sign = 0;
    if (src < 0) {
        src = -src;
        sign = 1;
    }
    s21_decimal_sign(dst, sign);
    int count = 0;
    for (int i = 0; i < 96; i++) {
        if (src / (long long int)(pow(2, i)) < 2) {
            count = i;
            break;
        }
    }
    for (count; count >= 0; count--){
        if (src - (long long int)(pow(2, count)) >= 0) {
            s21_set_bit(dst, count, 1);
            src -= (long long int)(pow(2, count));
        } else {
            s21_set_bit(dst, count, 0);
        }
    }
    return 0;
}

int s21_from_decimal_to_int(s21_decimal src, long long int *dst) {
    *dst = 0;
    for (int i = 95; i >= 0; i--) {
        long long int sub_res = 0;
        if(s21_get_bit(&src, i)) {
            sub_res = 1;
            int j = 0;
            while(j < i) {
                sub_res *= 2;
                j++;
            }
        }
        *dst += sub_res;
    }
    return *dst;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
    s21_decimal_init(dst);
    int sign = 0;
    if (src < 0) {
        src = -src;
        sign = 1;
    }
    s21_decimal_sign(dst, sign);
    int rank = -100;
    while (src / pow(2, rank) > 2) {
        rank++;
    }
    int scale = 127 + rank;
    int bit = 7;
    for (int i = 23; i > 15; i--) {
        if (scale - pow(2, bit) >= 0) {
            dst->bits[3] = dst->bits[3] | (1 << i);
            scale -= pow(2, bit);
            bit--;
        } else {
            dst->bits[3] = dst->bits[3] & ~(1 << i);
            bit--;
        }
    }
    for (int i = 95; i > 0; i--) {
        if (src - pow(2, rank) > 0) {
            s21_set_bit(dst, i, 1);
            src -= pow(2, rank);
            rank--;
        } else {
            s21_set_bit(dst, i, 0);
            rank--;
        }
    }
    s21_shift(dst);
    return 0;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
    int scale = s21_get_scale(src) - 127;
    printf("%d\n", scale);
    s21_decimal new_src;
    s21_decimal_init(&new_src);
    new_src = src;
    s21_reverse_shift(&new_src);
    s21_set_bit(&new_src, 95, 1);
    for (int i = 95; i >= 0; i--) {
        *dst += s21_get_bit(&new_src, i) * pow(2, scale);
        scale--;
    }
    return 0;
}

void s21_set_bit(s21_decimal *dst, int bit, int bit_status) {
    if (bit_status == 0) {
        dst->bits[bit/32] = dst->bits[bit/32] & ~(1 << bit % 32);
    } else {
        dst->bits[bit/32] = dst->bits[bit/32] | (1 << bit % 32);
    }
}

int s21_get_bit(s21_decimal *dst, int bit) {
    int result_bit;
    if ((dst->bits[bit/32] & (1 << bit % 32)) == 0) {
        result_bit = 0;
    } else {
        result_bit = 1;
    }
    return result_bit;
}

// int s21_inverse_bit(s21_decimal *dst, int bit) {
//     int result_bit;
//     if((dst->bits[bit/32] & (1 << bit % 32)) == 0) {
//         result_bit = 1;
//     } else {
//         result_bit = 0;
//     }
//     return result_bit;
// }

int s21_get_scale(s21_decimal src) {
    int scale = 0;
    int degree = 0;
    for (int shift = 16; shift < 24; shift++) {
        int bit = !!(src.bits[3] & (1 << shift));
        if (bit != 0) {
            scale += bit * pow(2, degree);
        }
        degree++;
    }
    return scale;
}

void s21_decimal_init(s21_decimal *dst) {
    for (int i = 0; i < 128; i++) {
        s21_set_bit(dst, i, 0);
    }
}

void s21_decimal_sign(s21_decimal *dst, int sign_status) {
    if (sign_status == 0) {
        dst->bits[3] = dst->bits[3] & ~(1 << 31);
    } else {
        dst->bits[3] = dst->bits[3] | (1 << 31);
    }
}

void s21_shift(s21_decimal *value) {
    for (int i = 95; i > 0; i--) {
        if (s21_get_bit(value, i-1) == 1) {
            s21_set_bit(value, i, 1);
        } else {
            s21_set_bit(value, i, 0);
        }
    }
    s21_set_bit(value, 0, 0);
}

void s21_reverse_shift(s21_decimal *value) {
    for (int i = 0; i < 95; i++) {
        if (s21_get_bit(value, i+1) == 1) {
            s21_set_bit(value, i, 1);
        } else {
            s21_set_bit(value, i, 0);
        }
    }
    s21_set_bit(value, 95, 0);
}
