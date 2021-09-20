#include "malloc.h"

t_page **get_first_page()
{
	static t_page *first = NULL;

	return (&first);
}

t_page *first_page()
{
	return (*get_first_page());
}

size_t page_size_from_size(size_t size)
{
	if (size <= TINY_ALLOC_SIZE)
		return TINY_PAGE_SIZE;
	else if (size <= SMALL_ALLOC_SIZE)
		return SMALL_PAGE_SIZE;
	else
		return size;
}

t_page_type page_type_from_page_size(size_t size)
{
	if (size <= TINY_PAGE_SIZE)
		return TINY;
	else if (size <= SMALL_PAGE_SIZE)
		return SMALL;
	else
		return LARGE;
}

t_bool in_page(t_malloc_chunk *chunk, t_page *page)
{
	return ((void *)chunk > (void *)page && (void *)chunk < NEXT_PAGE(page));
}

t_page *page_from_chunk(t_malloc_chunk *chunk)
{
	t_page *page;

	page = first_page();
	if (!page)
		return (NULL);
	while (page)
	{
		if (page->num_chunks > 0 && in_page(chunk, page))
			return (page);
		page = page->next;
	}
	return (NULL);
}

void *space_left(t_page *page, size_t size)
{
	void *start;
	void *end;
	t_malloc_chunk *chunk;

	end = NEXT_PAGE(page);
	chunk = page->chunks;
	if (!chunk)
	{
		start = PAGE_DATA(page);
		if ((size_t)(end - start) >= size + CHUNK_SIZE)
			return start;
	}
	else
	{
		while (chunk->fd)
			chunk = chunk->fd;
		start = NEXT_CHUNK(chunk);
		if ((size_t)(end - start) >= size + CHUNK_SIZE)
			return (start);
	}
	return (NULL);
}
