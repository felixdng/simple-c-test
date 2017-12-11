#include <stdio.h>
#include <stddef.h>
#include <string.h>

/**
 * offsetof(type, member) : 结构体type的成员member的偏移量
 * container_of(ptr, type, member) : 已知结构体type的成员member的指针为ptr, 求结构体type的起始地址
 */
#ifndef container_of
#if 1
#define container_of(ptr, type, member) \
	((type *)((char *)ptr - offsetof(type, member)))
#else
#define container_of(ptr, type, member) ({ \
	const typeof( ((type *)0)->member ) *__mptr = (ptr); \
	(type *)( (char *)__mptr - offsetof(type,member) );})
#endif
#endif

struct data_1 {
	int a;
	char b[30];
};

struct data_2 {
	int a;
	struct data_1 b;
};

int main(int argc, char **argv)
{
	struct data_2 data;
	data.a = 100;
	data.b.a = 200;
	strcpy(data.b.b, "hall");

	struct data_1 *p = &data.b;
	struct data_2 *q = container_of(p, struct data_2, b);
	printf("%d, %d, %s\n", q->a, q->b.a, q->b.b);

	return 0;
}
