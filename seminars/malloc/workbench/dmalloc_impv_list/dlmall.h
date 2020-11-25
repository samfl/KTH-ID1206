#ifndef DLMALL_H_
#define DLMALL_H_

#include <stddef.h>
#include <stdint.h>

void dree(void* mem);
void* dalloc(size_t req);
void flist_info(void);
uint16_t sanity(void);

#endif /* DLMALL_H_ */
