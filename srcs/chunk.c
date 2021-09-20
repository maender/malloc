#include "malloc.h"

t_malloc_chunk *add_chunk(t_page *page, void *start, size_t size)
{
	t_malloc_chunk *chunk;

	if (!page)
		return (NULL);
	chunk = (t_malloc_chunk *)start;
	init_chunk(chunk, page, size);
	return (chunk);
}

t_malloc_chunk *create_chunk(t_page *page, void *start, size_t size)
{
	if (!page)
	{
		page = create_page_from_size(size);
		return (add_chunk(page, PAGE_DATA(page), size));
	}
	return (add_chunk(page, start, size));
}

t_malloc_chunk *search_free_chunk(size_t size)
{
	t_page *page;
	void *space;

	if ((space = free_space_available(size)))
		return ((t_malloc_chunk *)space);
	page = first_page();
	while (page)
	{
		if (page_type_from_page_size(size) == page->type)
		{
			if ((space = space_left(page, size)))
				return (create_chunk(page, space, size));
		}
		page = page->next;
	}
	return (NULL);
}

void init_chunk(t_malloc_chunk *chunk, t_page *page, size_t size)
{
	t_malloc_chunk *ptr;

	ptr = page->chunks;
	page->num_chunks++;
	page->free_size -= size;
	chunk->size = size;
	chunk->fd = NULL;
	chunk->bk = NULL;
	chunk->freed = FALSE;
	if (!ptr)
	{
		page->chunks = chunk;
		chunk->bk = NULL;
		chunk->prev_size = 0;
	}
	else
	{
		while (ptr->fd)
			ptr = ptr->fd;
		ptr->fd = chunk;
		chunk->bk = ptr;
		chunk->prev_size = ptr->size;
	}
}
