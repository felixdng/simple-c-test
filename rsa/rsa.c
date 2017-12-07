/**
 * file rsa.c
 * RSA asymmetric algorithm
 */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "rsa.h"

static int_64 prime_p = 987127;
static int_64 prime_q = 1549;
static int_64 number = 1529059723; //prime_p * prime_q
static int_64 Ln; //(prime_p - 1) * (prime_q - 1)
static int_64 e = 65557;
static int_64 d = 58832029;

/**
 * 判断一个整数是否是素数
 * 返回: 1-是, 0-不是
 */
int is_prime(int_64 num)
{
	if (2 == num || 3 == num)
		return 1;

	// 不在6的倍数两侧的一定不是素数
	if ((num % 6) != 1 && (num % 6) != 5)
		return 0;

	// 判断在6的倍数两侧的是否是素数
	int_64 i;
	int_64 tmp = sqrtl(num);
	for (i = 5; i <= tmp; i += 6) {
		if ((num % i) == 0 || (num % (i + 2) == 0))
			return 0;
	}

	return 1;
}

#if 0
/**
 * 判断一个64位的整数的二进制位个数
 */
int_64 binary_bits_64(int_64 num)
{
	int_64 tmp = num;
	int_64 count = 0;

	if (num == 0)
		return 1;

	while (tmp) {
		tmp = tmp / 2;
		count++;
	}

	return count;
}
#endif

/**
 * 扩展欧几里得算法: 求解二元一次方程
 */
int_64 exgcd(int_64 a, int_64 b, int_64 *x, int_64 *y)
{
	int_64 d;

	if (0 == a && 0 == b)
		return -1;

	if (0 == b) {
		*x = 1;
		*y = 0;
		return a;
	}

	d = exgcd(b, a % b, y, x);
	*y -= a / b * (*x);

	return d;
}

/**
 * 计算模反元素d
 */
int_64 mod_reverse(int_64 a, int_64 b)
{
	int_64 x, y, d;

	d = exgcd(a, b, &x, &y);
	if (1 == d) {
		if (x % b <= 0)
			return (x % b) + b;
		else
			return x % b;
	} else {
		return -1;
	}
}

#if 1
void rsa_init(int_64 p, int_64 q)
{
	//寻找p最近的素数
	while (1) {
		if (is_prime(p))
			break;
		p++;
	}

	//确保p和q不相等
	if (p == q)
		q++;

	//寻找q最近的素数
	while (1) {
		if (is_prime(q))
			break;
		q++;
	}

	prime_p = p;
	prime_q = q;

	//计算p和q的乘积
	number = prime_p * prime_q;

	//计算n的欧拉函数
	Ln = (prime_p - 1) * (prime_q - 1);

	//随机生成一个整数e, 满足1 < e < Ln, e与Ln互质(e为质数，且Ln不是e的倍数)
	while (1) {
		srand((unsigned)time(NULL));
		int_64 tmp = 1 + rand() % (Ln > 70000 ? 70000 : Ln);
		while (1) {
			if (is_prime(tmp) && (Ln % tmp != 0))
				break;
			tmp++;
		}

		if (tmp > 1 && tmp < Ln) {
			e = tmp;
			break;
		}
	}

	//计算模反元素d
#if 1
	d = mod_reverse(e, Ln);
#else
	d = 1;
	while(((e * d) % Ln) != 1)
		d++;
#endif
	printf("n: %lld\n", number);
	printf("e: %lld\n", e);
	printf("d: %lld\n", d);
}
#endif

/**
 * 获取公钥
 */
void GetPublicKey(int_64 *pub_n, int_64 *pub_e)
{
	*pub_n = number;
	*pub_e = e;
}

/**
 * 获取私钥
 */
void GetPrivateKey(int_64 *pri_n, int_64 *pri_d)
{
	*pri_n = number;
	*pri_d = d;
}

/**
 * RSA加密
 * pub_n: 公钥n
 * pub_e: 公钥e
 * data: 需要加密的数据
 */
int_64 EncryptRSA(int_64 pub_n, int_64 pub_e, int_64 data)
{
	int_64 tmp = data;
	int_64 i;

	//确保data < pub_n
	if (data >= pub_n)
		return -1;

	for (i = 1; i < pub_e; i++) {
		tmp *= data;
		tmp = tmp % pub_n;
	}

	return tmp;
}

/**
 * RSA解密
 * pub_n: 私钥n
 * pub_e: 私钥d
 * data: 需要解密的数据
 */
int_64 DecryptRSA(int_64 pri_n, int_64 pri_d, int_64 data)
{
	int_64 tmp = data;
	int_64 i;

	for (i = 1; i < pri_d; i++) {
		tmp *= data;
		tmp = tmp % pri_n;
	}

	return tmp;
}

