//
// Created by boran on 07.11.2016.
//

#include "vmm.h"
#include "../include/screen.h"
#include "alloc.h"

char *kern_heap;
list kern_free_vm;
uint32 *pd0 = (uint32 *) KERN_PDIR;
char *page0 = 0;    //Kernel page 0
char *page1 = (char *) KERN_PG_1;   // Kernel page 1 (4mb)
char *end_page1 = (char *) KERN_PG_1_LIM;

uint8 mem_bitmap[RAM_MAXPAGE / 8];
uint32 malloc_used = 0;

char *get_page_frame (void) {
    int page = -1;
    for (int byte = 0; byte < RAM_MAXPAGE / 8; byte++) {
        if (mem_bitmap[byte] != 0xFF)
            for (int bit = 0; bit < 8; bit++) {
                if (!(mem_bitmap[byte] & (1 << bit))) {
                    page = 8 * byte + bit;
                    set_page_frame_used(page);
                    return (char *) (page * PAGESIZE);
                }
            }
    }
    return (char *) -1;
}

page *get_page_from_heap (void) {
    page *pg;
    vm_area *area;
    char *v_ad, *p_ad;

    p_ad = get_page_frame();
    if (p_ad < 0)
        printk("NO PAGE FRAME AVAILABLE!!! \n");

    if (list_is_empty(&kern_free_vm))
        printk("NO MEMORY LEFT IN PAGE HEAP!!! \n");

    area = list_first_entry(&kern_free_vm, vm_area, vm_list);
    v_ad = area->vm_start;

    area->vm_start += PAGESIZE;
    if (area->vm_start == area->vm_end) {
        list_del(&area->vm_list);
        free(area);
    }

    pd0_add_page(v_ad, p_ad, 0);

    pg = malloc(sizeof(page));
    pg->v_addr = v_ad;
    pg->p_addr = p_ad;
    pg->p_list.next = NULL;
    pg->p_list.prev = NULL;

    return pg;
}

int release_page_from_heap (char *v_addr) {
    vm_area *next_ar, *prev_ar, *new_ar;
    char *p_addr;

    p_addr = get_p_addr(v_addr);
    if (p_addr)
        release_page_frame(p_addr);
    else {
        printk("NO PAGE FRAME ASSOCIATED!!! \n");
        return -1;
    }

    pd_remove_page(v_addr);
    list_for_each_entry(next_ar, &kern_free_vm, vm_list) {
        if (next_ar->vm_start > v_addr)
            break;
    }

    prev_ar = list_entry(next_ar->vm_list.prev, vm_area, vm_list);

    if (prev_ar->vm_end == v_addr) {
        prev_ar->vm_end += PAGESIZE;
        if (prev_ar->vm_end == next_ar->vm_start) {
            prev_ar->vm_end = next_ar->vm_end;
            list_del(&next_ar->vm_list);
            free(next_ar);
        }
    }
    else if (next_ar->vm_start == (v_addr + PAGESIZE) )
        next_ar->vm_start = v_addr;
    else if (next_ar->vm_start > v_addr + PAGESIZE) {
        new_ar = malloc(sizeof(vm_area));
        new_ar->vm_start = v_addr;
        new_ar->vm_end = v_addr + PAGESIZE;
        list_add(&new_ar->vm_list, &prev_ar->vm_list);
    }
    else {
        printk("SYSTEM HALTED!! \n");
        asm("hlt");
    }
    return 0;
}

void mem_init (uint32 mem) {
    int pg, pg_lim;
    vm_area *p;
    vm_area *pm;

    pg_lim = (mem * 1024) / PAGESIZE;

    for (pg = 0; pg < pg_lim / 8; pg++)
        mem_bitmap[pg] = 0;
    for (pg = pg_lim / 8; pg < RAM_MAXPAGE / 8; pg++)
        mem_bitmap[pg] = 0xFF;

    for (pg = PAGE(0x0); pg < ( PAGE( (uint32)end_page1) ); pg++) {
        set_page_frame_used(pg);
    }

    pd0[0] = ((uint32)page0 | (PG_PRESENT | PG_WRITE | PG_4MB));
    pd0[1] = ((uint32)page1 | (PG_PRESENT | PG_WRITE | PG_4MB));
    for (uint32 i = 2; i < 1023; i++)
        pd0[i] = ((uint32) page1 + PAGESIZE * i) | (PG_PRESENT | PG_WRITE);
    pd0[1023] = ((uint32) pd0 | (PG_PRESENT | PG_WRITE));

    //thanks to internet
    asm("	mov %0, %%eax \n \
			mov %%eax, %%cr3 \n \
			mov %%cr4, %%eax \n \
			or %2, %%eax \n \
			mov %%eax, %%cr4 \n \
			mov %%cr0, %%eax \n \
			or %1, %%eax \n \
			mov %%eax, %%cr0"::"m"(pd0), "i"(PAGING_FLAG), "i"(PSE_FLAG));

    kern_heap = (char *) KERN_HEAP;
    ksbrk(1);

    p = malloc(sizeof(vm_area));
    p->vm_start = (char *) KERN_PG_HEAP;
    p->vm_end = (char *) KERN_PG_HEAP_LIM;

    LIST_HEAD_INIT(*&kern_free_vm);
    list_add(&p->vm_list, &kern_free_vm);
}

page_dir *pd_create (void) {
    page_dir *pd;
    uint32 *pdir;

    pd = malloc(sizeof(page_dir));
    pd->base = get_page_from_heap();

    pdir = (uint32 *) pd->base->v_addr;
    for (int i = 0; i < 256; i++)
        pdir[i] = pd0[i];
    for (int j = 256; j < 1023; ++j)
        pdir[j] = 0;

    pdir[1023] = ((uint32)pd->base->p_addr | (PG_PRESENT | PG_WRITE));

    LIST_HEAD_INIT(*&pd->pd_list);
    return pd;
}

void pd_destroy (page_dir *pd) {
    page *pg;
    list *p, *n;

    list_for_each_safe(p, n, &pd->pd_list) {
        pg = list_entry(p, page, p_list);
        release_page_from_heap(pg->v_addr);
        list_del(p);
        free(pg);
    }

    release_page_from_heap(pd->base->v_addr);
    free(pd);
}

page_dir *pd_copy (page_dir *pd_in) {
    page_dir *pd;
    uint32 *pdir;

    pd = malloc(sizeof(page_dir));
    pd->base = get_page_from_heap();

    pdir = (uint32 *) pd->base->v_addr;
    for (int i = 0; i < 256; i++)
        pdir[i] = pd0[i];

    for (int i = 256; i < 1023; i++)
        pdir[i] = 0;

    pdir[1023] = ((uint32) pd->base->p_addr | (PG_PRESENT | PG_WRITE));

    LIST_HEAD_INIT(*&pd->pd_list);

    return pd;
}

bool pd0_add_page (char *v_addr, char *p_addr, int flag) {
    if (v_addr > (char *) USER_OFFSET)
        return false;   // not in kernel

    uint32 *pg = (uint32 *) (0xFFFFF000 | (((uint32) v_addr & 0xFFC00000) >> 20));
    if ((*pg & PG_PRESENT) == 0)
        return false;   // error

    pg = (uint32 *) (0xFFC00000 | (((uint32) v_addr & 0xFFFFF000) >> 10));
    *pg = ((uint32) p_addr) | (PG_PRESENT | PG_WRITE | flag);
    set_page_frame_used(p_addr);
    return true;
}

int pd_add_page (char *v_addr, char *p_addr, int flag, page_dir *pd) {
    uint32 *pdi = (uint32 *) (0xFFFFF000 | (((uint32) v_addr & 0xFFC00000) >> 20));

    if ((*pdi & PG_PRESENT) == 0) {
        page *pg = get_page_from_heap();

        uint32 *pgt = (uint32 *) pg->v_addr;
        for (int i = 0; i < 1024; ++i)
            pgt[i] = NULL;

        *pdi = (uint32) pg->p_addr | (PG_PRESENT | PG_WRITE | flag);

        if (pd)
            list_add(&pg->p_list, &pd->pd_list);
    }

    uint32 *pte;
    pte = (uint32 *) (0xFFC00000 | (((uint32) v_addr & 0xFFFFF000) >> 10));
    *pte = ((uint32) p_addr) | (PG_PRESENT | PG_WRITE | flag);

    return 0;
}

int pd_remove_page (char *v_addr) {
    uint32 *pte;

    if (get_p_addr(v_addr)) {
        pte = (uint32 *) (0xFFC00000 | (((uint32) v_addr & 0xFFFFF000) >> 10));
        *pte = (*pte & (~PG_PRESENT));

        asm("invlpg %0"::"m"(v_addr));
    }

    return 0;
}

char *get_p_addr (char *v_addr) {
    uint32 *pde;		/* adresse virtuelle de l'entree du repertoire de pages */
    uint32 *pte;		/* adresse virtuelle de l'entree de la table de pages */

    pde = (uint32 *) (0xFFFFF000 | (((uint32) v_addr & 0xFFC00000) >> 20));
    if ((*pde & PG_PRESENT)) {
        pte = (uint32 *) (0xFFC00000 | (((uint32) v_addr & 0xFFFFF000) >> 10));
        if ((*pte & PG_PRESENT))
            return (char *) ((*pte & 0xFFFFF000) + (VADDR_PG_OFFSET((uint32) v_addr)));
    }

    return 0;
}

void vmm_init (uint32 hsize) {
    mem_init(hsize);
}

void vmm_kmap(uint32 phy, uint32 virt) {
    pd0_add_page((char *) phy, (char *) virt, PG_USER);
}
