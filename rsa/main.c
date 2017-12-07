#include<stdio.h>  
#include<stdlib.h>  

#include "rsa.h"


int main(int argc, char **argv)
{
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

	return 0;
}

