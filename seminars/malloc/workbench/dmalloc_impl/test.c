#include "dlmall.h"

int main(void)
{

	unsigned int* t1 = (unsigned int*) dalloc(sizeof(int));
	unsigned int* t2 = (unsigned int*) dalloc(sizeof(int) * 2);
	unsigned int* t3 = (unsigned int*) dalloc(sizeof(int) * 3);
	unsigned int* t4 = (unsigned int*) dalloc(sizeof(int) * 4);

	dree(t1);
	dree(t2);
	dree(t3);
	dree(t4);

	return 0;
}
