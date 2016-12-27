#ifndef MEMORY_H
#define MEMORY_H

#include "../include/types.h"
#include "vmm.h"
#include "page_vars.h"

#define PAGESIZE 4096
#define HEAP_LIMIT	0x40000000

#define NO_MEMORY -1
#define USED 1
#define UNUSED 0

void *ksbrk (int n);
void *malloc (uint32 size);
void  free (void *addr);

typedef struct MALLOC_HEADER {
    uint32 size:31;
    uint32 used:1;
} malloc_header;


#endif
