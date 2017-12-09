/**
 * file rsa.c
 * RSA asymmetric algorithm
 */
#include <stdio.h>
#include <string.h>
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
#if 0
	int_64 tmp = data;
	int_64 i;

	//data的pub_e次幂对pub_n求余数
	for (i = 1; i < pub_e; i++) {
		tmp *= data;
		tmp = tmp % pub_n;
	}

	return tmp;
#else
	/**
	 * 快速幂取模算法
	 */
	int_64 ret = 1;
	data = data % pub_n; //防止data比pub_n大

	while (pub_e != 0) {
		if (pub_e & 1)
			ret = (ret * data) % pub_n;
		pub_e >>= 1;
		data = (data * data) % pub_n;
	}
	return ret;
#endif
}

/**
 * RSA解密
 * pub_n: 私钥n
 * pub_e: 私钥d
 * data: 需要解密的数据
 */
int_64 DecryptRSA(int_64 pri_n, int_64 pri_d, int_64 data)
{
#if 0
	int_64 tmp = data;
	int_64 i;

	for (i = 1; i < pri_d; i++) {
		tmp *= data;
		tmp = tmp % pri_n;
	}

	return tmp;
#else
	/**
	 * 快速幂取模算法
	 */
	int ret = 1;
	data = data % pri_n; //防止data比pri_n大

	while (pri_d != 0) {
		if (pri_d & 1)
			ret = (ret * data) % pri_n;
		pri_d >>= 1;
		data = (data * data) % pri_n;
	}
	return ret;
#endif
}

/************************************************************************************/

/**
 * 16进制字符转为整数
 */
static inline int_64 char_to_num(char ch)
{
	int_64 num = 0;

	if (ch >= '0' && ch <= '9')
		num = ch - '0';
	else if (ch >= 'a' && ch <= 'f')
		num = ch - 'a' + 10;
	else if (ch >= 'A' && ch <= 'F')
		num = ch - 'A' + 10;

	return num;
}

/**
 * 整数转为16进制字符, 字母的用大写字母
 */
static inline char num_to_char(int_64 num)
{
	char ch = '0';

	if (num >= 0 && num <= 9)
		ch = '0' + num;
	else if (num >= 10 && num <= 15)
		ch = 'A' + (num - 10);

	return ch;
}

/**
 * 去掉MD5字符串中的'-'
 */
static inline void md5str2num(char *md5str)
{
	char tmp[MD5_ARR_SIZE];
	char *p = NULL;
	char *q = NULL;

	strcpy(tmp, md5str);

	p = md5str;
	q = tmp;
	while ((*q) != '\0') {
		if ((*q) != '-')
			*p++ = *q;
		q++;
	}
	*p = '\0';
}

/**
 * 加上MD5字符串中的'-'
 */
static inline void md5num2str(char *md5str)
{
	char tmp[MD5_ARR_SIZE];
	char *p = NULL;
	int i;

	strcpy(tmp, md5str);

	p = tmp;
	i = 0;
	while ((*p) != '\0') {
		//在8,13,18,23的位置加'-'
		if (i == 8 || i == 13 || i == 18 || i == 23)
			md5str[i++] = '-';
		else
			md5str[i++] = *p++;
	}
	md5str[i] = '\0';
}

/**
 * 加密MD5字符串
 * (pub_n, pub_e) RSA公钥
 * md5            需要加密的MD5字符串
 * rsa_str        RSA加密后的密文
 */
const char *EncryptRSA_MD5(int_64 pub_n, int_64 pub_e, const char md5[], char rsa_str[])
{
	int i, j;
	int size = strlen(md5) + 1;
	char md5_str[size];

	strcpy(md5_str, md5);

	//去掉MD5字符串中的'-'
	md5str2num(md5_str);

	//将MD5字符串拆分成8组
	int_64 md5_arr[8];
	for (i = 0; i < 8; i++) {
		md5_arr[i] = (char_to_num(md5_str[4*i]) << 12) | (char_to_num(md5_str[4*i+1]) << 8) |
				(char_to_num(md5_str[4*i+2]) << 4) | char_to_num(md5_str[4*i+3]);
	}

	//对md5_arr进行RSA加密
	int_64 rsa_arr[8];
	for (i = 0; i < 8; i++) {
		rsa_arr[i] = EncryptRSA(pub_n, pub_e, md5_arr[i]);
	}

	memset(rsa_str, 0, MD5_RSA_SIZE);
	//将rsa_arr转化成对应的字符
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++)
			rsa_str[8 * i + (7-j)] = num_to_char((rsa_arr[i] >> (4*j)) & 0xf);
	}
	rsa_str[MD5_RSA_SIZE-1] = '\0';

	return rsa_str;
}

/**
 * 解密MD5字符串
 * (pri_n, pri_d) RSA私钥
 * rsa_str        RSA加密的密文
 * md5_str        解密后的MD5字符串
 */
const char *DecryptRSA_MD5(int_64 pri_n, int_64 pri_d, const char rsa_str[], char md5_str[])
{
	int i, j;
	int_64 num_arr[8];
	int_64 rsa_arr[8];

	for (i = 0; i < 8; i++) {
		num_arr[i] = 0x0;
		for (j = 0; j < 8; j++)
			num_arr[i] |= char_to_num(rsa_str[8 * i + (7-j)]) << (4 * j);
	}

	//RSA解密
	for (i = 0; i < 8; i++) {
		rsa_arr[i] = DecryptRSA(pri_n, pri_d, num_arr[i]);
	}

	memset(md5_str, 0, MD5_ARR_SIZE);
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 4; j++)
			md5_str[4 * i + (3-j)] = num_to_char(rsa_arr[i] >> (4*j) & 0xf);
	}
	md5_str[MD5_NUM_SIZE] = '\0';

	//加上MD5字符串中的'-'
	md5num2str(md5_str);

	return md5_str;
}

