//
// Created by boran on 07.11.2016.
//

#ifndef BORIX_VMM_H
#define BORIX_VMM_H


#include "../include/list.h"
#include "page_vars.h"


typedef struct PAGE {
    char *v_addr;
    char *p_addr;
    list p_list;
} page;

typedef struct PAGE_DIR {
    page *base;
    list pd_list;
} page_dir;

typedef struct VM_AREA {
    char *vm_start;
    char *vm_end;
    list vm_list;
} vm_area;

typedef struct PAGE_DIR proc_mem;

char *kern_heap;

list kern_free_vm;

uint32 *pd0;

uint8 mem_bitmap[RAM_MAXPAGE / 8];

uint32 malloc_used;

#define set_page_frame_used(page)	\
    mem_bitmap[((uint32) page)/8] |= (1 << (((uint32) page)%8))

#define release_page_frame(p_addr)	\
    mem_bitmap[((uint32) p_addr/PAGESIZE)/8] &= ~(1 << (((uint32) p_addr/PAGESIZE)%8))


char *get_page_frame (void);

page *get_page_from_heap (void);
int release_page_from_heap (char *v_addr);

void mem_init (uint32 mem);

page_dir *pd_create (void);
void pd_destroy (page_dir *pd);
page_dir *pd_copy (page_dir *pd_in);

bool pd0_add_page (char *v_addr, char *p_addr, int flag);

int pd_add_page (char *v_addr, char *p_addr, int flag, page_dir *pd);

int pd_remove_page (char *v_addr);

char *get_p_addr (char *v_addr);


// functions for VMM is below

void vmm_init (uint32 hsize);

proc_mem *vmm_cratePM ();

void vmm_switchPM (proc_mem *ad);

void vmm_map (proc_mem *ad, uint32 phy, uint32 addr);

void vmm_kmap (uint32 phy, uint32 virt);

#endif //BORIX_VMM_H
