#ifndef __RSA_H__
#define __RSA_H__

//MD5字符串数组的大小
#define MD5_ARR_SIZE 37

//MD5字符串中整数的个数
#define MD5_NUM_SIZE 32

//MD5由RSA加密后字符串的大小
#define MD5_RSA_SIZE 65

typedef unsigned char      uint_8;
typedef unsigned int       uint_32;
typedef unsigned long long uint_64;
typedef long long          int_64;

extern void rsa_init(int_64 p, int_64 q);
extern void GetPublicKey(int_64 *pub_n, int_64 *pub_e);
extern void GetPrivateKey(int_64 *pri_n, int_64 *pri_d);
extern int_64 EncryptRSA(int_64 pub_n, int_64 pub_e, int_64 data);
extern int_64 DecryptRSA(int_64 pri_n, int_64 pri_d, int_64 data);

extern const char *EncryptRSA_MD5(int_64 pub_n, int_64 pub_e, const char md5[], char rsa_str[]);
extern const char *DecryptRSA_MD5(int_64 pri_n, int_64 pri_d, const char rsa_str[], char md5_str[]);

#endif
