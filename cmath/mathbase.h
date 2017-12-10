#ifndef __MATH_BASE_H
#define __MATH_BASE_H

typedef unsigned char uint_8;
typedef unsigned int uint_32;
typedef char int_8;
typedef int int_32;

extern void reverse_arr(int_8 str[], int_32 len);
extern int_32 compare_arr(const int_8 str_1[], const int_8 str_2[]);
extern int_8 *plus(const int_8 str_1[], const int_8 str_2[]);
extern int_8 *minus(const int_8 str_1[], const int_8 str_2[]);

#endif
