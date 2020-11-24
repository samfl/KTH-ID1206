/*
 * dlmall.h
 *
 *  Created on: Nov 23, 2020
 *      Author: samf
 */

#ifndef DLMALL_H_
#define DLMALL_H_

#include <stddef.h>

void dree(void* mem);
void* dalloc(size_t req);
void flist_info(void);
int16_t sanity(void);

#endif /* DLMALL_H_ */
