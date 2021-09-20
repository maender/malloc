#include "malloc.h"

t_bool resize_chunk(t_malloc_chunk *chunk, size_t size)
{
	t_page *page;
	void *start;
	void *end;

	page = page_from_chunk(chunk);
	if (!page || page->free_size < size)
		return (FALSE);
	start = CHUNK_DATA(chunk);
	if (!chunk->fd)
		end = NEXT_PAGE(page);
	else
		end = NEXT_CHUNK(chunk);
	if ((size_t)(end - start) >= size)
	{
		page->free_size -= (size - chunk->size);
		chunk->size = size;
		printf("test\n");
		return (TRUE);
	}
	return (FALSE);
}

void *ft_realloc(void *ptr, size_t size)
{
	void *out;
	t_malloc_chunk *curr;

	if (size == 0)
	{
		ft_free(ptr);
		return (NULL);
	}
	if (!ptr)
		return (ft_malloc(size));
	curr = chunk_from_addr(ptr);
	if (resize_chunk(curr, size))
		return (CHUNK_DATA(curr));
	out = ft_malloc(size);
	ft_memcpy(out, ptr, size);
	ft_free(ptr);
	return (out);
}