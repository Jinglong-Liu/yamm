#ifndef __YAMM_H__
#define __YAMM_H__

#define MAX_MEMORY_BLOCKS (32)

int yammInit(int size);

void* yammAlloc(int size);

int yammFree(void* ptr);

int yammDestroy();

int yammDump();

#endif
