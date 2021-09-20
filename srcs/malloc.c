
#include "malloc.h"

void *ft_malloc(size_t size)
{
	t_malloc_chunk *new_chunk;

	if (size == 0)
		return (NULL);
	size = align(size);
	new_chunk = search_free_chunk(size);
	if (!new_chunk)
		new_chunk = create_chunk(NULL, NULL, size);
	new_chunk->freed = FALSE;
	return (CHUNK_DATA(new_chunk));
}

void test_malloc()
{
	char *ptr = NULL;
	for (int i = 0; i < 1024; i++)
	{
		ptr = realloc(ptr, 1024);
		ptr[0] = 42;
	}
}

void test_my_malloc()
{
	void*tab[1024];
	char *ptr = NULL;
	for (int i = 0; i < 1024; i++)
	{
		ptr = (char*)ft_malloc(sizeof(char)*1024);
		ptr[0] = 42;
		tab[i] = ptr;
		if (i > 0)
		printf("%d\n", tab);
	}
	printf("%p\n", tab[0]);
	// ft_free(tab[1]);
	ft_realloc(tab[0], sizeof(char)*5000);
	printf("%p\n", tab[0]);
	show_alloc_mem();
}

void __attribute__((destructor)) dtor()
{
	t_page *page;
	t_page *next;

	page = first_page();
	while (page)
	{
		next = page->next;
		munmap(page, page->size);
		page = NULL;
		page = next;
	}
	*get_first_page() = NULL;
}

int main(int ac, char **av)
{
	if (ac > 1)
	{
		if (!ft_strcmp(av[1], "0"))
			test_malloc();
		else
			test_my_malloc();
	}
	else
		test_my_malloc();
}
