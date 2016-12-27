#include "alloc.h"


void *ksbrk (int n) {
    malloc_header *chunk;
    char *addr;

    if ((kern_heap + (n * PAGESIZE)) > (char *) HEAP_LIMIT)
        return (void *) NO_MEMORY;  // no virtual memory left for kernel heap

    chunk = (malloc_header *) kern_heap;

    // allocation

    for (int i = 0; i < n; ++i) {
        addr = get_page_frame();
        if (addr < 0) {
            return (void *) NO_MEMORY; // no free page frame
        }

        pd0_add_page(kern_heap, addr, 0);

        kern_heap += PAGESIZE;
    }

    chunk->size = (uint32) (PAGESIZE * n);
    chunk->used = UNUSED;
    return chunk;
}

void *malloc (uint32 size) {

    if (size == 0)
        return 0;

    uint32 total_size;
    malloc_header *chunk;

    total_size = sizeof(malloc_header) + size;
    if (total_size < MALLOC_MINSIZE)
        total_size = MALLOC_MINSIZE;

    chunk = (malloc_header *) KERN_HEAP;
    while (chunk->used || chunk->size < total_size) {
        if (chunk->size == 0) {
            asm("hlt");
            return 0;
        }
        chunk += chunk->size;
        if (chunk == (malloc_header *) kern_heap) {
            if ((ksbrk(total_size / PAGESIZE) + 1) < 0) {
                asm("hlt");
                return (void *) NO_MEMORY;   // no memory left
            }
            else if (chunk > (malloc_header *) kern_heap) {
                asm("hlt");
                return 0;
            }
        }
    }

    // free block founded
    if (chunk->size - total_size < MALLOC_MINSIZE)
        chunk->used = USED;
    else {
        malloc_header *tmp;
        tmp = chunk + total_size;
        tmp->size = chunk->size - total_size;
        tmp->used = UNUSED;

        chunk->size = total_size;
        chunk->used = total_size;
    }

    malloc_used += total_size;

    return chunk + sizeof(malloc_header);
}

void free (void *addr) {
    if (addr == 0)
        return;

    malloc_header *chunk;
    chunk = addr - sizeof(malloc_header);
    chunk->used = UNUSED;

    malloc_used -= chunk->size;

    malloc_header *tmp;
    tmp = (chunk + chunk->size);
    while (tmp < (malloc_header *) kern_heap && tmp->used == UNUSED) {
        tmp = (chunk + chunk->size);
        chunk->size += tmp->size;
    }
}
