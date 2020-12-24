#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <errno.h>

#define TRUE 1
#define FALSE 0
#define HEAD (sizeof(struct head))
#define TAKEN (sizeof(struct taken))
#define MIN(size) (((size)>(8))?(size):(8))
#define LIMIT(size) (MIN(0) + HEAD + size)
#define MAGIC(memory) ((struct taken*)memory - TAKEN)
#define HIDE(block) (void*)((struct head*)block + HEAD)
#define ALIGN 8
#define ARENA (64*1024)

struct head
{
	uint16_t bfree;
	uint16_t bsize;
	uint16_t free;
	uint16_t size;

	struct head* next;
	struct head* prev;
};

struct taken
{
	uint16_t bfree;
	uint16_t bsize;
	uint16_t free;
	uint16_t size;
};

struct head* arena = NULL;
struct head* flist;

struct head* after(struct head* block)
{
	char* ptr = (char*) block + HEAD + block->size;
	return (struct head*) (ptr);
}

struct head* before(struct head* block)
{
	char* ptr = (char*) block - HEAD - block->bsize;
	return (struct head*) (ptr);
}

struct head* split(struct head* block, int size)
{
	int rsize = block->size - (size + HEAD);
	block->size	= rsize;
	struct head *splt = after(block);
	splt->bsize = block->size;
	splt->bfree = block->free;
	splt->size = size;
	splt->free = block->free;

	struct head *aft = after(splt);
	aft->bsize = size;
	return splt;
}

struct head* new(void)
{
	if (arena != NULL)
	{
		printf("One arena already allocated\n");
		return NULL;
	}

	struct head* new = mmap(NULL, ARENA, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if (new == MAP_FAILED)
	{
		printf("mmap failed	: error %d\n", errno);
		return NULL;
	}

	uint32_t size = ARENA - 2*HEAD;

	new->bfree = FALSE;
	new->bsize = 0;
	new->free = TRUE;
	new->size = size;

	struct head* sentinel = after(new);

	sentinel->bfree = TRUE;
	sentinel->bsize = size;
	sentinel->free = FALSE;
	sentinel->size = 0;

	arena = (struct head*) new;
	return new;
}

void detach(struct head* block)
{
	if (block->next != NULL)
	{
		block->next->prev = block->prev;
	}

	if (block->prev != NULL)
	{
		block->prev->next = block->next;
	} else
	{
		block->next->prev = NULL;
		flist = block->next;
	}
}

void insert(struct head* block)
{
	block->next = flist;
	block->prev = NULL;

	if (flist != NULL)
	{
		flist->prev = block;
	}

	flist = block;
}

int adjust(size_t size)
{
	int rem = size % ALIGN;

	if(rem > 0)
	{
		size = size + (ALIGN - rem);
	}
	return size;
}

struct head* find(int size)
{
	if (arena == NULL)
	{
		flist = new();
	}

	struct head* curr = flist;
	while(curr != NULL)
	{
		if (curr->size >= (size + TAKEN))
		{
			if (curr->size >= LIMIT(size))
			{
				return split(curr, size);
			}
			detach(curr);
			return curr;
		}

		curr = curr->next;
	}
	return NULL;
}

int len_of_flist(void)
{
	struct head* curr = flist;
	int len = 0;
	while(curr != NULL)
	{
		++len;
		curr = curr->next;
	}
	return len;
}

void blocksize_of_flist(void)
{
	struct head* curr = flist;
	while(curr != NULL)
	{
		printf("current size: %d\n", curr->size);
		curr = curr->next;
	}
}

int sanity(void)
{

	/* Check free list */
	struct head* curr = flist;
	struct head* prev = NULL;

	while(curr != NULL)
	{
		/* Check if free-status is correct */
		if (curr->free == FALSE)
		{
			return 1;
		}

		/* Check if size is larger or equal to 8 */
		if (curr->size < MIN(0))
		{
			return 1;
		}

		/* Check if free in adjacent blocks are valid */
		if (curr->free != before(curr)->bfree)
		{
			return 1;
		}

		/* Check if size in adjacent blocks are valid */
		if (curr->size != before(curr)->bsize)
		{
			return 1;
		}

		prev = curr;
		curr = curr->next;
	}

	/* Check arena */
	/* Check if we have an arena */
	if (!arena)
	{
		return 1;
	} else
	{
		prev = arena;
		curr = after(arena);

		while (curr->size != 0)
		{
			if(prev->free != curr->bfree)
			{
				return 1;
			}
			if(prev->size != curr->bsize)
			{
				return 1;
			}
			if(curr->size % ALIGN != 0)
			{
				return 1;
			}
			if(curr->size >= MIN(0))
			{
				return 1;
			}
			prev = curr;
			curr = after(curr);
		}
	}
}

struct head* merge(struct head* block)
{
	struct head* aft = after(block);
	struct head* bef = before(block);

	if (block->bfree)
	{
		/*unlink block bef. */
		detach(bef);

		/* calc and set tot size of merged blocks*/
		bef->size = bef->size + block->size + HEAD;

		/* upd- the block after the merged blocks */
		aft->bsize = bef->size;

		/* cont with the merged blocks*/
		block = bef;
	}

	if (aft->free)
	{
		/* unlink the block */
		detach(aft);

		/* calc and set the totatl size of merged blocks */
		block->size = block->size + aft->size + HEAD;

		/* upd. the block after the merged block */
		after(aft)->bsize = block->size;
	}

	insert(block);
	return block;
}

void* dalloc(size_t req)
{
	if (req <= 0)
	{
		return NULL;
	}

	int size = adjust(req);
	struct head* taken = find(size);

	if (taken == NULL)
	{
		return NULL;
	} else
	{
		taken->free = FALSE;
		after(taken)->bfree = FALSE;

		return (void*) HIDE(taken);
	}
}

void dree(void* mem)
{
	if (mem != NULL)
	{
		struct head* block = MAGIC(mem);
		struct head* aft = after(block);
		block->free = TRUE;
		aft->bfree = TRUE;
		merge(block);
	}

	return;
}



