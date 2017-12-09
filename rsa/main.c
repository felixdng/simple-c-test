#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "rsa.h"

static clock_t start = 0;
static clock_t finish = 0;
static double runtimes = 0.0;

int main(int argc, char **argv)
{
#if 0
	int_64 tmp, num;

	if (argc != 4) {
		printf("Usage: %s <num_p> <num_q> <num>\n", argv[0]);
		return -1;
	}

	num = atoll(argv[3]);
	printf("原始数据: %lld\n", num);

	//输入两个整数进行初始化
	rsa_init(atoll(argv[1]), atoll(argv[2]));

	//RSA加密
	{
		int_64 key_n, key_e;
		GetPublicKey(&key_n, &key_e); //获取公钥
		if (num >= key_n) {
			printf("num不能大于公钥key_n\n");
			return -1;
		}
		printf("\n正在加密...\n");
		tmp = EncryptRSA(key_n, key_e, num); //用公钥进行加密
		printf("加密后的数据: %lld\n", tmp);
	}

	//RSA解密
	{
		int_64 key_n, key_d;
		GetPrivateKey(&key_n, &key_d); //获取私钥
		printf("\n正在解密...\n");
		num = DecryptRSA(key_n, key_d, tmp);
		printf("解密后的数据 : %lld\n", num);
	}
#endif

#if 0
	int_64 tmp, num;

	if (argc != 2) {
		printf("Usage: %s <num>\n", argv[0]);
		return -1;
	}

	num = atoll(argv[1]);
	printf("原始数据: %lld\n", num);

	//RSA加密
	{
		int_64 key_n, key_e;
		GetPublicKey(&key_n, &key_e); //获取公钥
		if (num >= key_n) {
			printf("num不能大于公钥key_n\n");
			return -1;
		}
		printf("\n正在加密...\n");
		tmp = EncryptRSA(key_n, key_e, num); //用公钥进行加密
		printf("加密后的数据: %lld\n", tmp);
	}

	//RSA解密
	{
		int_64 key_n, key_d;
		GetPrivateKey(&key_n, &key_d); //获取私钥
		printf("\n正在解密...\n");
		num = DecryptRSA(key_n, key_d, tmp);
		printf("解密后的数据 : %lld\n", num);
	}
#endif


#if 1
	int_64 key_n, key_e, key_d;
	char md5[] = "d6173946-1cb5-46ac-bf29-19349f0c44d5";
	printf("原始数据: %s\n", md5);

	GetPublicKey(&key_n, &key_e); //获取公钥
	char rsa_str[MD5_RSA_SIZE];
	start = clock();
	EncryptRSA_MD5(key_n, key_e, md5, rsa_str);
	finish = clock();
	runtimes = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("start: %ld, finish: %ld, 加密时间: %lf 秒\n", start, finish, runtimes);
	//printf("加密后的数据: %s\n", rsa_str);

	GetPrivateKey(&key_n, &key_d); //获取私钥
	char md5_str[MD5_ARR_SIZE];
	start = clock();
	DecryptRSA_MD5(key_n, key_d, rsa_str, md5_str);
	finish = clock();
	runtimes = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("start: %ld, finish: %ld, 解密时间: %lf 秒\n", start, finish, runtimes);
	printf("解密后的数据 : %s\n", md5_str);
#endif

	return 0;
}

