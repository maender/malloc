#include "malloc.h"

t_free_list **get_first_free_ptr()
{
	static t_free_list *free_lst = NULL;

	return (&free_lst);
}

t_free_list *first_free_ptr()
{
	return (*get_first_free_ptr());
}

void remove_from_free_list(void *ptr)
{
	t_free_list *lst;

	lst = first_free_ptr();
	while (lst)
	{
		if (ptr == (void *)lst)
		{
			if (lst->prev)
				lst->prev->next = lst->next;
			if (lst->next)
				lst->next->prev = lst->prev;
			lst = lst->next;
			return;
		}
	}
}

void consoliate(t_malloc_chunk *chunk)
{
	if (chunk->bk && chunk->bk->freed)
	{
		chunk->bk->fd = chunk->fd;
		chunk->bk->size += chunk->size + CHUNK_SIZE;
		chunk = chunk->bk;
	}
	if (chunk->fd && chunk->fd->freed)
	{
		remove_from_free_list(CHUNK_DATA(chunk->fd));
		chunk->fd = chunk->fd->fd;
		chunk->size += chunk->fd->size + CHUNK_SIZE;
	}
}

void add_to_free_list(t_malloc_chunk *ptr)
{
	t_free_list *free_list;
	t_free_list *new;

	free_list = first_free_ptr();
	new = (t_free_list *)(CHUNK_DATA(ptr));
	new->next = NULL;
	new->prev = NULL;
	if (!free_list)
		*get_first_free_ptr() = new;
	else
		consoliate(ptr);
}

void *add_chunk_back(t_malloc_chunk *chunk, size_t size)
{
	t_page *page;
	t_page_type type;

	page = first_page();
	if (!page)
		return (NULL);
	type = page_type_from_page_size(size);
	while (page)
	{
		if ((void *)page < (void *)chunk && (void *)chunk < NEXT_PAGE(page))
		{
			if (page->type != type)
				return (NULL);
			chunk->freed = FALSE;
			chunk->size = size;
			return (chunk);
		}
		page = page->next;
	}
	return (NULL);
}

void *space_left_after_free_ptr(t_free_list *free_ptr, size_t size)
{
	t_malloc_chunk *chunk;

	chunk = (t_malloc_chunk *)(free_ptr);
	chunk -= 1;
	if (chunk->size <= size)
		return (add_chunk_back(chunk, size));
	return (NULL);
}

void *free_space_available(size_t size)
{
	t_free_list *free_list;
	void *ptr;

	free_list = first_free_ptr();
	if (!free_list)
		return (NULL);
	while (free_list)
	{
		if ((ptr = space_left_after_free_ptr(free_list, size)))
			return (ptr);
		free_list = free_list->next;
	}
	return (NULL);
}

t_malloc_chunk *chunk_from_addr(void *ptr)
{
	t_page *page;
	t_malloc_chunk *chunk;

	page = first_page();
	if (!page)
		return (NULL);
	ptr -= CHUNK_SIZE;
	while (page)
	{
		if (ptr >= (void *)page && ptr < NEXT_PAGE(page))
		{
			chunk = page->chunks;
			while (chunk)
			{
				if (ptr == (void *)chunk)
					return (chunk);
				chunk = chunk->fd;
			}
		}
		page = page->next;
	}
	return (NULL);
}

void remove_page(t_page *page)
{
	t_page *lst;

	lst = first_page();
	if (page == lst)
		*get_first_page() = page->next;
	else
	{
		while (lst)
		{
			if (page == lst)
			{
				if (lst->prev)
					lst->prev->next = lst->next;
				if (lst->next)
					lst->next->prev = lst->prev;
				break;
			}
			lst = lst->next;
		}
	}
	if (page_type_from_page_size(page->size) == LARGE)
		page->size += CHUNK_SIZE + PAGE_SIZE;
	munmap(page, page->size);
	return;
}

void ft_free(void *ptr)
{
	t_malloc_chunk *curr;
	t_page *page;

	if (!ptr)
		return;
	curr = chunk_from_addr(ptr);
	if (curr->freed)
		return;
	curr->freed = TRUE;
	page = page_from_chunk(curr);
	page->free_size += curr->size;
	// if (page->free_size == page->size)
	// 	remove_page(page);
	// else
	add_to_free_list(curr);
}
