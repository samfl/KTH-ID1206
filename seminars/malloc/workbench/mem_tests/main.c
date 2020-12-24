#include <stdio.h>

typedef unsigned short uint16_t;

struct head
{
	uint16_t free;
	uint16_t size;
	uint16_t bfree;
	uint16_t bsize;

	struct head* prev;
	struct head* next;
};

int main(void)
{
	void* mem = 0x55555557a000;
	void* ptr_a = (void*) mem + 1;
	void* ptr_b = (void*) mem - 1;

	printf("old address: %p \n ", mem);
	printf("new address (after): %p \n ", ptr_a);
	printf("new address (before): %p \n ", ptr_b);
}
