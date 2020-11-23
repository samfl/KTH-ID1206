/*
 * dmalloc.c
 *
 *  Created on: Nov 23, 2020
 *      Author: samf
 */
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <errno.h>

#define TRUE 1
#define FALSE 0
#define HEAD (sizeof(struct head))
#define MIN(size) (((size)>(8))?(size):(8))
#define LIMIT(size) (MIN(0) + HEAD + size)
#define MAGIC(memory) ((struct head*)memory - 1)
#define HIDE(block) (void*)((struct head*)block + 1)
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
		flist = NULL;
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
		if (curr->size >= (size + HEAD))
		{
			if (curr->size >= LIMIT(size))
			{
				return split(curr, size);
			}
			return curr;
		}

		curr = curr->next;
	}
	return NULL;
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
		struct head* aft = after(taken);
		aft->bfree = FALSE;

		return (void*) (((char*)taken) + HEAD);
	}
}

void dree(void* mem)
{
	if (mem != NULL)
	{
		struct head* block = (struct head*) (((char*) mem) - HEAD);
		struct head* aft = after(block);
		block->free = TRUE;
		aft->bfree = TRUE;
		insert(block);
	}

	return;
}



