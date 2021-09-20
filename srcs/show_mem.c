#include "malloc.h"

void show_alloc_mem()
{
	t_page *page;
	size_t tiny_mem = 0;
	size_t small_mem = 0;
	size_t large_mem = 0;
	size_t tiny = 0;
	size_t small = 0;
	size_t large = 0;

	page = first_page();
	if (!page)
	{
		ft_printf("0B allocated.\n");
		return;
	}
	while (page)
	{
		t_page_type type = page_type_from_page_size(page->size);
		size_t page_size = 0;
		t_malloc_chunk *chunk = page->chunks;
		while (chunk)
		{
			if (!chunk->freed)
				page_size += chunk->size;
			chunk = chunk->fd;
		}
		switch (type)
		{
		case TINY:
			tiny_mem += page_size;
			tiny++;
			break;
		case SMALL:
			small_mem += page_size;
			small++;
			break;
		default:
			large_mem += page_size;
			large++;
			break;
		}
		page = page->next;
	}
	ft_printf("TINY : %zuB\n", tiny_mem);
	ft_printf("SMALL : %zuB\n", small_mem);
	ft_printf("LARGE : %zuB\n", large_mem);
	ft_printf("Number of pages : %zu, %zu, %zu\n", tiny, small, large);
}

void show_mem_addr()
{
	t_page *page = first_page();
	int i = 1;

	if (!page)
		return;
	while (page)
	{
		ft_printf("\npage %d, size : %zu\n", i++, page->size);
		if (page->prev)
			ft_printf("after prev : %d\n", page > page->prev);
		ft_printf("addr : %p, next page : %p, first chunk : %p\n", page, NEXT_PAGE(page), PAGE_DATA(page));
		if (page->chunks)
		{
			ft_printf("chunks : ");
			t_malloc_chunk *chunk = page->chunks;
			int i = 0;
			while (chunk)
			{
				ft_printf("%s%p", i++ != 0 ? ", " : "", (void *)chunk);
				chunk = chunk->fd;
			}
			ft_printf("\n");
		}
		page = page->next;
	}
}