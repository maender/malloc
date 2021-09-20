#include "malloc.h"

void init_page(t_page *page, size_t size)
{
	page->type = page_type_from_page_size(size);
	page->size = page_size_from_size(size);
	page->free_size = page->size;
	page->num_chunks = 0;
	page->next = NULL;
	page->prev = NULL;
	page->chunks = NULL;
}

void add_page(t_page *page)
{
	t_page *first;

	first = first_page();
	if (!first)
		*get_first_page() = page;
	else
	{
		while (first->next)
			first = first->next;
		first->next = page;
		page->prev = first;
	}
}

t_page *create_page_from_size(size_t size)
{
	t_page *page;

	page = alloc_page(size);
	if (page)
	{
		init_page(page, size);
		add_page(page);
	}
	return (page);
}

size_t align_page(size_t n)
{
	return (n + PAGE_SZ - 1) & ~(PAGE_SZ - 1);
}

t_page *alloc_page(size_t size)
{
	t_page *page;
	void *ptr;
	t_page_type type;

	type = page_type_from_page_size(size);
	size = page_size_from_size(size);
	if (type == LARGE)
		size += CHUNK_SIZE + PAGE_SIZE;
	page = first_page();
	while (page && page->next)
		page = page->next;
	// *size = align_page(*size);
	ptr = mmap(page ? page : NULL, size, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
	return ((t_page *)ptr);
}