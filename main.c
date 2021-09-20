#include "malloc.h"
#include <stdio.h>

int main()
{
	char *ptr;
	for (int i = 0; i < 1024; i++)
	{
		ptr = malloc(1024);
		ptr[0] = 42;
		show_alloc_mem();
	}
}