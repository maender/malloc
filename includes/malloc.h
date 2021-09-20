#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include "libft.h"

#define SIZE_SZ (sizeof(size_t))
#define MALLOC_ALIGN (2 * SIZE_SZ)
#define NFASTBINS (64)
#define MAX_FAST_SIZE (80 * SIZE_SZ / 4)
#define PAGE_SZ getpagesize()
#define TINY_ALLOC_SIZE (size_t)128
#define SMALL_ALLOC_SIZE (size_t)1024
#define TINY_PAGE_SIZE (size_t)(PAGE_SZ * 4)
#define SMALL_PAGE_SIZE (size_t)(PAGE_SZ * 32)
#define PAGE_SIZE sizeof(t_page)
#define CHUNK_SIZE sizeof(t_malloc_chunk)
#define PAGE_DATA(page) (void *)(page + 1)
#define NEXT_PAGE(page) PAGE_DATA(page) + page->size
#define CHUNK_DATA(chunk) (void *)(chunk + 1)
#define NEXT_CHUNK(chunk) CHUNK_DATA(chunk) + chunk->size
#define FALSE (0)
#define TRUE (!FALSE)

typedef short t_bool;

typedef struct s_malloc_chunk t_malloc_chunk;
typedef struct s_malloc_state t_malloc_state;
typedef struct s_malloc_chunk *t_fastbin;

typedef enum e_page_type
{
	TINY,
	SMALL,
	LARGE
} t_page_type;

// typedef struct s_mallinfo
// {
// 	int arena;	  /* non-mmapped space allocated from system */
// 	int ordblks;  /* number of free chunks */
// 	int smblks;	  /* number of fastbin blocks */
// 	int hblks;	  /* number of mmapped regions */
// 	int hblkhd;	  /* space in mmapped regions */
// 	int usmblks;  /* maximum total allocated space */
// 	int fsmblks;  /* space available in freed fastbin blocks */
// 	int uordblks; /* total allocated space */
// 	int fordblks; /* total free space */
// 	int keepcost; /* top-most, releasable (via malloc_trim) space */
// } t_mallinfo;

typedef struct s_malloc_chunk
{
	t_malloc_chunk *fd;
	t_malloc_chunk *bk;
	size_t prev_size;
	size_t size;
	t_bool freed;
} t_malloc_chunk;

typedef struct s_page
{
	struct s_page *next;
	struct s_page *prev;
	t_malloc_chunk *chunks;
	size_t size;
	size_t free_size;
	size_t num_chunks;
	t_page_type type;
} t_page;

// typedef struct s_malloc_state
// {
// 	int mutex;
// 	int flags;
// 	t_fastbin fastbins[NFASTBINS];
// } t_malloc_state;

typedef struct s_free_list
{
	struct s_free_list *prev;
	struct s_free_list *next;
} t_free_list;

void *ft_malloc(size_t size);
t_malloc_chunk *search_free_chunk(size_t size);
t_malloc_chunk *create_chunk(t_page *page, void *start, size_t size);
t_malloc_chunk *add_chunk(t_page *page, void *start, size_t size);
void init_chunk(t_malloc_chunk *chunk, t_page *page, size_t size);
void *space_left(t_page *page, size_t size);
t_page *create_page_from_size(size_t size);
void add_page(t_page *page);
void init_page(t_page *page, size_t size);
t_page *alloc_page(size_t size);
t_page_type page_type_from_page_size(size_t size);
size_t page_size_from_size(size_t size);
t_page *first_page();
t_page **get_first_page();
size_t align(size_t n);
size_t align_page(size_t n);
// t_mallinfo mallinfo_init(int *hasbeeninit);
void malloc_init(int *hasbeeninit);
void show_alloc_mem();
void ft_free(void *ptr);
t_free_list **get_first_free_ptr();
t_free_list *first_free_ptr();
void *free_space_available(size_t size);
void show_mem_addr();
t_page *page_from_chunk(t_malloc_chunk *chunk);
void *ft_realloc(void *ptr, size_t size);
t_malloc_chunk *chunk_from_addr(void *ptr);
