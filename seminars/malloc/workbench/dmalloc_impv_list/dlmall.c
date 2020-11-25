/**************************************************************************
 * Includes
 *************************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <errno.h>

/**************************************************************************
 * Defines
 *************************************************************************/
#define TRUE 1
#define FALSE 0
#define HEAD (sizeof(struct head))
#define MIN(size) (((size)>(8))?(size):(8))
#define LIMIT(size) (MIN(0) + HEAD + size)
#define MAGIC(memory) ((struct head*)memory - 1)
#define HIDE(block) (void*)((struct head*)block + 1)
#define ALIGN 8
#define ARENA (64*1024)

/**************************************************************************
 * Declaration / Definitions
 *************************************************************************/
/* header (4 * 2) + (2 * 8) = 24 bytes */
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
struct head* flist[16];

/**************************************************************************
 * Debugging / Learning Helper Procedures
 *************************************************************************/
void flist_info(void)
{
	uint16_t len_tot = 0;
	uint16_t len_lst = 0;
	printf("************* Flist-info START *************\n ");
	for (uint16_t i = 0; i < sizeof(flist)/sizeof(flist[0]); i++)
	{
		struct head* curr = *(flist + i);
		while(curr != NULL)
		{
			++len_lst;
			printf("\n");
			printf("block %d: %d byte ", len_lst, curr->size);
			curr = curr->next;
		}
		printf("\t (flist[%d]: %d st) \n", i, len_lst);
		len_tot += len_lst;
		len_lst = 0;
	}
	printf("flist-length tot: %d \n", len_tot);
}

/**************************************************************************
 * Dalloc / Dree Helper Procedures
 *************************************************************************/
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

uint16_t get_flist_index(uint16_t size)
{
	if (size > 128){ return 15; }
	if (size < 16) { return 0;  }
	return (size / 8) - 2;
}

uint16_t adjust(size_t size)
{
	uint16_t rem = size % ALIGN;
	if(rem > 0) { size = size + (ALIGN - rem); }
	return size;
}

struct head* new(void)
{
	if (arena != NULL)
	{
		printf("One arena already allocated\n");
		return NULL;
	}
	if (new == MAP_FAILED)
	{
		printf("mmap failed	: error %d\n", errno);
		return NULL;
	}

	struct head* new = mmap(NULL, ARENA, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	struct head* sentinel = after(new);
	uint32_t size = ARENA - 2*HEAD;

	new->bfree = FALSE;
	new->bsize = 0;
	new->free = TRUE;
	new->size = size;
	sentinel->bfree = TRUE;
	sentinel->bsize = size;
	sentinel->free = FALSE;
	sentinel->size = 0;
	arena = (struct head*) new;
	return new;
}

void detach(struct head* block)
{
	if (block->next != NULL) { block->next->prev = block->prev; }

	if (block->prev != NULL) { block->prev->next = block->next; }
	else
	{
		if (block->next == NULL) { flist[get_flist_index(block->size)] = NULL; }
		else
		{
			block->next->prev = NULL;
			flist[get_flist_index(block->size)] = block->next;
		}
	}
}

void insert(struct head* block)
{
	struct head* curr_list = *(flist + get_flist_index(block->size));
	block->next = curr_list;
	block->prev = NULL;

	if (curr_list != NULL)
	{
		curr_list->prev = block;
	}
	*(flist + get_flist_index(block->size)) = block;
}

struct head* split(struct head* block, uint16_t size)
{
	uint16_t rsize = (block->size) - (size + HEAD);
	uint16_t new_index = get_flist_index(rsize);
	uint16_t old_index = get_flist_index(block->size);

	if (new_index != old_index)
	{
		detach(block);
		block->size	= rsize;
		insert(block);
	} else

	{
		block->size	= rsize;
	}

	struct head *splt = after(block);
	splt->bsize = block->size;
	splt->bfree = block->free;
	splt->size = size;
	splt->free = block->free;

	struct head *aft = after(splt);
	aft->bsize = size;
	return splt;
}

struct head* find(uint16_t size)
{
	if (arena == NULL)
	{
		for(uint16_t i = 0; i < sizeof(flist)/sizeof(flist[0]); i++)
		{
			flist[i] = NULL;
		}
		flist[15] = new();
	}
	uint16_t index = get_flist_index(size);
	for (uint16_t i = index; i < sizeof(flist)/sizeof(flist[0]); i++)
	{
		struct head* curr = flist[i];

		while(curr != NULL)
		{
			if (curr->size >= (size + HEAD))
			{
				if (curr->size >= LIMIT(size)) { return split(curr, size); }
				detach(curr);
				return curr;
			}
			curr = curr->next;
		}
	}
	return NULL;
}

struct head* merge(struct head* block)
{
	struct head* aft = after(block);
	struct head* bef = before(block);

	if (bef->free)
	{
		detach(bef);
		bef->size = bef->size + block->size + HEAD;
		aft->bsize = bef->size;
		block = bef;
	}
	if (aft->free)
	{
		detach(aft);
		block->size = block->size + aft->size + HEAD;
		after(aft)->bsize = block->size;
	}
	insert(block);
	return block;
}

/**************************************************************************
 * Verification Procedure
 *************************************************************************/
uint16_t sanity(void)
{
	for (uint16_t i = 0; i < sizeof(flist)/sizeof(flist[0]); i++)
	{
		/* Checking the Free-list */
		struct head* curr = flist[i];
		struct head* prev = NULL;

		while(curr != NULL)
		{
			if (curr->free == FALSE) { return 1; } 				 /* Free status */
			if (curr->size < MIN(0)) { return 2; } 				 /* Minimum size */
			//if (curr->free != before(curr)->bfree) { return 3; } /* Adjacent blocks (free) */
			//if (curr->size != before(curr)->bsize) { return 4; } /* Adjacent blocks (size) */
			prev = curr;
			curr = curr->next;
		}

		/* Checking the Arena */
		if (!arena) { return 5; }
		else
		{
			prev = arena;
			curr = after(arena);

			while (curr->size != 0)
			{
				if(prev->free != curr->bfree) { return 6; }
				if(prev->size != curr->bsize) { return 7; }
				if(curr->size % ALIGN != 0) { return 8; }
				if(curr->size < MIN(0)) { return 9; }
				prev = curr;
				curr = after(curr);
			}
		}
	}
	return 0;
}

/**************************************************************************
 * Main Procedures
 **************************************************************************/
void* dalloc(size_t req)
{
	if (req <= 0) { return NULL; }

	uint16_t size = adjust(req);
	struct head* taken = find(size);

	if (taken == NULL) { return NULL; }
	else
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
		merge(block);
	}
	return;
}



