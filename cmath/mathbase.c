/**
 * file: mathbase.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mathbase.h"

//最大位数
#define NUM_MAX 1024

/**
 * 反转字符数组
 */
void reverse_arr(int_8 str[], int_32 len)
{
	int_32 i, j;
	int_8 tmp;

	i = 0;
	j = len - 1;
	while (i < j) {
		tmp = str[i];
		str[i] = str[j];
		str[j] = tmp;
		i++;
		j--;
	}
}

/**
 * 将ASCII码转为对应的数值
 */
void convert_int(int_8 str[], int_32 len)
{
	int_32 i;

	for (i = 0; i < len; i++) {
		str[i] = str[i] - '0';
	}
}

/**
 * 将数值转为对应的ASCII码
 */
void convert_ascii(int_8 str[], int_32 len)
{
	int_32 i;

	for (i = 0; i < len; i++) {
		str[i] = str[i] + '0';
	}
}

/**
 * 比较两个整数大小
 * str_1 > str_2: 返回1
 * str_1 = str_2: 返回0
 * str_1 < str_2: 返回-1
 */
int_32 compare_arr(const int_8 str_1[], const int_8 str_2[])
{
	int_32 len_1 = strlen(str_1);
	int_32 len_2 = strlen(str_2);

	if (len_1 > len_2)
		return 1;
	else if (len_1 < len_2)
		return -1;

	int_32 i;
	for (i = 0; i < len_1; i++) {
		if (str_1[i] > str_2[i])
			return 1;
		else if (str_1[i] < str_2[i])
			return -1;
	}

	return 0;
}

/**
 * 加法运算
 * str_1,str_2相加的两个数(以字符串形式表示)
 * 返回计算结果, 该指针要用free释放
 */
int_8 *plus(const int_8 str_1[], const int_8 str_2[])
{
	int_32 i;
	int_32 len_1 = strlen(str_1);
	int_32 len_2 = strlen(str_2);

	//两数相加, 和的最大位数是较大的加数的位数+1
	int_32 len = len_1 > len_2 ? (len_1 + 2) : (len_2 + 2);

	int_8 *p1 = (int_8 *)malloc(len*sizeof(int_8));
	if (!p1)
		return NULL;

	int_8 *p2 = (int_8 *)malloc(len*sizeof(int_8));
	if (!p2) {
		free(p1);
		return NULL;
	}

	int_8 *rest = (int_8 *)malloc(len*sizeof(int_8));
	if (!rest) {
		free(p1);
		free(p2);
		return NULL;
	}

	//为方便计算, 将个位数放在数组第0位
	for (i = 0; i < len_1; i++)
		p1[i] = str_1[len_1-1-i];
	for (i = 0; i < len_2; i++)
		p2[i] = str_2[len_2-1-i];

	//ASCII码转化为对应的数值
	convert_int(p1, len_1);
	convert_int(p2, len_2);

	//不够位数的填充0
	for (i = len_1; i < len; i++)
		p1[i] = 0;
	for (i = len_2; i < len; i++)
		p2[i] = 0;

	//开始运算
	memset(rest, 0, len);
	for (i = 0; i < len - 2; i++) {
		//每位相加并保存到rest
		rest[i] += p1[i] + p2[i];

		//大于9则进位
		if (rest[i] > 9) {
			rest[i] -= 10;
			rest[i+1]++;
		}
	}

	//去除无效的0
	i = len - 1;
	while ((i >= 0) && (0 == rest[i])) {
		i--;
	}

	reverse_arr(rest, i + 1);   //反转数组
	convert_ascii(rest, i + 1); //转为字符
	rest[i+1] = '\0';
	free(p1);
	free(p2);
	return rest;
}

/**
 * 减法运算
 * str_1,str_2 相减的两个数(以字符串形式表示)
 * 返回两个数的差, 该指针要用free释放
 */
int_8 *minus(const int_8 str_1[], const int_8 str_2[])
{
	int_32 i;
	int_32 len_1 = strlen(str_1);
	int_32 len_2 = strlen(str_2);

	//两数相减, 差的最大位数等于较大数的位数
	int_32 len = len_1 > len_2 ? (len_1 + 1) : (len_2 + 1);

	int_8 *p1 = (int_8 *)malloc(len*sizeof(int_8));
	if (!p1)
		return NULL;

	int_8 *p2 = (int_8 *)malloc(len*sizeof(int_8));
	if (!p2) {
		free(p1);
		return NULL;
	}

	int_8 *rest = (int_8 *)malloc(len*sizeof(int_8));
	if (!rest) {
		free(p1);
		free(p2);
		return NULL;
	}

	/**
	 * 较大数保存在p1, 较小数保存在p2
	 * 为方便计算, 将个位数放在数组第0位
	 */
	if (len_1 >= len_2) {
		for (i = 0; i < len_1; i++)
			p1[i] = str_1[len_1-1-i];
		for (i = 0; i < len_2; i++)
			p2[i] = str_2[len_2-1-i];
	} else {
		for (i = 0; i < len_2; i++)
			p1[i] = str_2[len_2-1-i];
		for (i = 0; i < len_1; i++)
			p2[i] = str_1[len_1-1-i];
		int_32 tmp = len_1;
		len_1 = len_2;
		len_2 = tmp;
	}

	//ASCII码转化为对应的数值
	convert_int(p1, len_1);
	convert_int(p2, len_2);

	//不够位数的填充0
	for (i = len_1; i < len; i++)
		p1[i] = 0;
	for (i = len_2; i < len; i++)
		p2[i] = 0;

	//开始运算
	memset(rest, 0, len);
	for (i = 0; i < len - 1; i++) {
		//不够减就向高位借位
		if (p1[i] < p2[i]) {
			rest[i] = p1[i] + 10 - p2[i];
			p1[i+1]--;
		} else {
			rest[i] = p1[i] - p2[i];
		}
	}

	//去除无效的0
	i = len - 1;
	while ((i >= 0) && (0 == rest[i])) {
		i--;
	}

	reverse_arr(rest, i + 1);   //反转数组
	convert_ascii(rest, i + 1); //转为字符
	rest[i+1] = '\0';
	free(p1);
	free(p2);
	return rest;
}

