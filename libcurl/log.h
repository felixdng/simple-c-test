#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>

#undef __DEBUG
#define __DEBUG
#ifdef __DEBUG
#define LOG_DEBUG(format, args...) \
	fprintf(stdout, "[DEBUG] [func %s()] : "format, __func__, ##args)
#else
#define LOG_DEBUG(format, args...)
#endif

#define LOG_INFO(format, args...) \
	fprintf(stdout, "[INFO] [func %s()] : "format, __func__, ##args)

#define LOG_ERR(format, args...) \
	fprintf(stderr, "[ERROR] [func %s()] : "format, __func__, ##args)

#endif
