#ifndef __RSA_H__
#define __RSA_H__

typedef unsigned char      uint_8;
typedef unsigned int       uint_32;
typedef unsigned long long uint_64;
typedef long long          int_64;

extern void rsa_init(int_64 p, int_64 q);
extern void GetPublicKey(int_64 *pub_n, int_64 *pub_e);
extern void GetPrivateKey(int_64 *pri_n, int_64 *pri_d);
extern int_64 EncryptRSA(int_64 pub_n, int_64 pub_e, int_64 data);
extern int_64 DecryptRSA(int_64 pri_n, int_64 pri_d, int_64 data);

#endif
