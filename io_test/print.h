#ifndef __PRINT_H__
#define __PRINT_H__

#include <stdio.h>

#define USER_DEBUG	0x00000001
#define USER_INFO	0X00000002
#define USER_ERR	0x00000004

#define USER_PRINT(flag, format, args...) \
	do { \
		if (USER_DEBUG & flag) \
			fprintf(stdout, "[%s] [debug] : "format"\n", __func__, ##args); \
		else if (USER_INFO & flag) \
			fprintf(stdout, "[%s] [info] : "format"\n", __func__, ##args); \
		else if (USER_ERR & flag) \
			fprintf(stderr, "[%s] [err] : "format"\n", __func__, ##args); \
	} while(0)

#endif /* __PRINT_H__ */
