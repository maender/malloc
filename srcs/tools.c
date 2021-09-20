#include "malloc.h"

void malloc_init(int *hasbeeninit)
{
	if (*hasbeeninit >= 0)
		return;
	*hasbeeninit = 0;
}

// t_mallinfo mallinfo_init(int *hasbeeninit)
// {
// 	t_mallinfo m;

// 	if (*hasbeeninit < 0)
// 		malloc_init(hasbeeninit);
// 	memset(&m, 0, sizeof(m));
// 	return (m);
// }

size_t align(size_t n)
{
	return (n + sizeof(void *) - 1) & ~(sizeof(void *) - 1);
}
