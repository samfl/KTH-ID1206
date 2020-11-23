#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

/* Defining the chunk size */
#define MIN 32
#define ALIGN 8

/* Chunk data structure */
struct chunk
{
  int size;
  struct chunk *next;
};

/* Free list reference */
struct chunk *flist = NULL;

extern int glob_count;

/* return the lenth of the free-list, only for statistics */
static int length_of_flist(void)
{
  int i = 0;
  struct chunk *next  = flist;
  while(next != NULL)
  {
    i++;
    next = next->next;
  }
  return i;
}

/* collect the sizes of all chunks in a buffer */
static void sizes(int *buffer, int max)
{
  struct chunk *next = flist;
  int i = 0;

  while((next != NULL) & (i < max))
  {
    buffer[i] = next->size;
    i++;
    next = next->next;
  }
}

/* Align a chunk size to a multiple of byte size <ALIGN> */
static int align(int size) {
  int rem = size % ALIGN;
  if(rem > 0)
  {
    size +=  (ALIGN - rem);
  }
  return size;
}

/* The free-list is ordered in address order. */
static void insert(struct chunk* cnk)
{
  struct chunk* next = flist;
  struct chunk *prev = NULL;

  /* Go the correct position */
  while(next != NULL)
  {
    if(cnk < next)
    {
      break;
    }
    prev = next;
    next = next->next;
  }

  /* Check if its the start */
  /* If not, put cnk inbetween current prev & next */
  if(prev == NULL)
  {
    flist = cnk;
  } else
  {
    prev->next = cnk;
  }
  cnk->next = next;
}

/* if possible, coalece with other free blocks */
static void coalesce(struct chunk *cnk)
{
  struct chunk* next = flist;
  struct chunk *prev = NULL;

  /* cnk_check is the adjacent block with higher address */
  char *cnk_check = (char*)cnk  + cnk->size + sizeof(struct chunk);

  /* prev_check is the adjacent block of the previous block */
  char *prev_check = NULL;

  /* Go through flist */
  while (next != NULL)
  {
    if(cnk_check == (char*)next)
    {
      if(prev_check == (char*)cnk)
      {
        // coalesce with both chunks
        prev->size += cnk->size + next->size;
        prev->next = next->next;
      } else
      {
          // coallece with next chunk
          cnk->size += next->size;
          cnk->next = next->next;
          if(prev == NULL)
          {
            flist = cnk;
          } else
          {
            prev->next = cnk;
          }
        }
        return;
    }

    if(prev_check == (char*)cnk)
    {
      // coallece with previous chunk
      prev->size += cnk->size;
      prev->next = next;
      return;
    }

    if(next > cnk)
    {
      // regular insert
      cnk->next = next;
      if(prev == NULL)
      {
	      flist = cnk;
      } else
      {
	      prev->next = cnk;
      }
      return;
    }

    prev = next;
    prev_check = ((char*)prev  + prev->size + sizeof(struct chunk));
    next = next->next;
  } /* End of while loop */

  if(prev_check == (char*)cnk)
  {
    // coallece with final chunk
    prev->size += cnk->size;
    prev->next = next;
    return;
  }

  //  insert as final chunk
  cnk->next = next;
  if(prev == NULL)
  {
    flist = cnk;
  } else
  {
    prev->next = cnk;
  }
}


static struct chunk* find(size_t size)
{
  struct chunk *next = flist;
  struct chunk *prev = NULL;
  struct chunk* taken = NULL;

  while(next != NULL)
  {
    if (next->size >= size)
    {
      taken = next;
      if(prev != NULL)
      {
	      prev->next = taken->next;
      } else
      {
	      flist = taken->next;
      }
      break;
    }
    prev = next;
    next = next->next;
  }
  return taken;
}

/* Our 'malloc' */
extern void* balloc(size_t size) {

  if( size == 0 )
  {
    return NULL;
  }

  /* we only handle size of multiples of ALIGN */
  size = align(size);

  struct chunk* taken = find(size);
  struct chunk* next = flist;

  if(taken != NULL)
  {
    printf("\n\t\t\t taken: %d\n", taken->size);

    /* left over is total size of remaining chunk  */
    int leftover = (taken->size - size);
    printf("\t\t\t leftover: %d", leftover);

    if(leftover >= MIN)
    {
      // new is the leftover chunk
      struct chunk *new = (struct chunk*)(((char*)next) + size + sizeof(struct chunk));

      new->size = (leftover - sizeof(struct chunk));
      // leftover is inserted in free list
      insert(new);

      // we're now changing the size of taken
      taken->size = size;
    }
    return  (void*)(next + 1);
  }
  /* use sbrk to allocate more memory */
  void *memory = sbrk(size + sizeof(struct chunk));

  if(memory == (void *)-1)
  {
    return NULL;
  } else
  {
    struct chunk *cnk = (struct chunk*)memory;
    cnk->size = size;
    return  (void*)(cnk + 1);
  }
}

/* Our 'free' */
extern void bree(void *memory)
{
  if(memory != NULL)
  {
    /* we're jumping back one chunk position */
    struct chunk *cnk = (struct chunk*)(((struct chunk*)memory) - 1);
    coalesce(cnk);
  }
  return;
}
