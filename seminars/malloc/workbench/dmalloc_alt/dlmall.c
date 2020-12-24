#define _GNU_SOURCE
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <errno.h>

typedef unsigned short uint16_t;
typedef struct head
{
    uint16_t bfree;
    uint16_t bsize;
    uint16_t free;
    uint16_t size;
    struct head* next;
    struct head* prev;
} head_t;

typedef struct
{
    uint16_t bfree;
    uint16_t bsize;
    uint16_t free;
    uint16_t size;
} taken_t;

#define TRUE 1
#define FALSE 0
#define HEAD (sizeof(taken_t))
#define MIN(size) (((size) > (16)) ? (size) : (16))
#define LIMIT(size) ( MIN(0) + HEAD + size )

/* memory - [sizeof(head_t)] */
#define MAGIC(memory) ((taken_t*) memory - 1)

/* memory + [sizeof(head_t)] */
#define HIDE(block) (void*) ((taken_t*) block + 1)
#define ALIGN 8
#define ARENA (64 * 1024)

head_t* flist = NULL;
head_t* arena = NULL;

void flist_info(void)
{
	head_t* curr = flist;
	uint16_t len_tot = 0u;
	uint16_t size_tot = 0u;

    while(curr != NULL)
    {
        ++len_tot;
        size_tot += curr->size;
        curr = curr->next;
    }

    printf("Blocks in free list (Amount): %d\n", len_tot);
    printf("Block size (Average): %d\n", len_tot/size_tot);
    return;
}

/* Return pointer to the block after (lower address) */
head_t* after(head_t* block)
{
	head_t* after = (head_t*) ((char*) block) + block->size + HEAD;
    return after;
}

/* Return pointer to the block before (higher address) */
head_t* before(head_t* block)
{
	head_t* before = (head_t*) ((char*) block) - block->bsize - HEAD;
    return before;
}

head_t* split(head_t* block, uint16_t size)
{
	uint16_t rsize = block->size - size - HEAD;
    block->size =  size;

    head_t* splt = after(block);
    splt->bsize = block->size;
    splt->bfree = block->free;
    splt->size = rsize;
    splt->free = TRUE;

    head_t* aft = after(splt);
    aft->bsize = splt->size;
    aft->bfree = splt->free;

    return splt;
}

head_t* new(void)
{
    if(arena != NULL)
    {
        printf("One arena already allocated \n");
        return NULL;
    }

    head_t* new = mmap(NULL, ARENA, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if(new == MAP_FAILED)
    {
        printf("mmap failed: error %d\n", errno);
        return NULL;
    }

    /* Mark start and end of an arena */
    uint16_t size = ARENA - 2 * HEAD;
    new->bfree = FALSE;
    new->bsize = 0;
    new->free = TRUE;
    new->size = size;
    flist = new;

    head_t* sentinel = after(new);
    sentinel->bfree = TRUE;
    sentinel->bsize = size;
    sentinel->free = FALSE;
    sentinel->size = 0;

    return new;
}

void detach(head_t* block)
{
    if(block->next != NULL)
    {
        block->next->prev = block->prev;
    }

    if(block->prev != NULL)
    {
        block->prev->next = block->next;
    }
    else
    {
        flist = block->next;
    }
    return;
}

/* Insert a block at the first position in the free list */
void insert(head_t* block)
{
    block->next = flist;
    block->prev = NULL;

    if(flist != NULL)
    {
        flist->prev = block;
    }

    flist = block;
    return;
}

uint16_t adjust(size_t request)
{
	uint16_t size = MIN(request);
    while(size%ALIGN != 0){
        size++;
    }
    return size;
}

head_t* find(uint16_t size)
{
	head_t* current = flist;

    while(current->size < size)
    {
        current = current->next;
        if(current == NULL)
        {
            return NULL;
        }
    }

    if(current->size >= LIMIT(size))
    {
    	head_t* splt = split(current, size);
        insert(splt);
        current->free = FALSE;
        after(current)->bfree = FALSE;
        detach(current);
    } else
    {
        current->free = FALSE;
        after(current)->bfree = FALSE;
        detach(current);
    }

    return current;
}

head_t* merge(head_t* block)
{
	head_t* aft = after(block);
	head_t* bef = before(block);

    if(bef->free)
    {
        detach(bef);
        uint16_t size = bef->size + block->size + HEAD;
        bef->size = size;
        aft->bsize = size;
        aft->bfree = TRUE;
        block = bef;
    }

    if(aft->free)
    {
        detach(aft);
        uint16_t size = block->size + aft->size + HEAD;
        block->size = size;
        head_t* aft2 = after(aft);
        aft2->bsize = size;
        aft2->bfree = TRUE;
    }

    return block;
}

void* dalloc(size_t request)
{
    if(arena == NULL)
    {
        arena = new();
        flist = arena;
    }

    if(request <= 0)
    {
        return NULL;
    }

    uint16_t size = adjust(request);
    head_t* taken = find(size);

    if(taken == NULL)
    {
        return NULL;
    }
    else
    {
        return HIDE(taken);
    }
}

void dree(void* memory)
{
    if(memory != NULL)
    {
    	head_t* block = (head_t*) MAGIC(memory);
        block = merge(block);

        head_t* aft = after(block);
        block->free = TRUE;
        aft->bfree = TRUE;

        insert(block);
    }

    return;
}
