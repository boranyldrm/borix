//
// Created by boran on 25.09.2016.
//
/*
 * Global Desriptor Table
 */

#ifndef BORIX_GDT_H
#define BORIX_GDT_H

#include "types.h"

// Defines a GDT entry.
struct gdt_entry {
    uint16 limit_low;
    uint16 base_low;
    uint8 base_middle;
    uint8 access;
    uint8 detail;
    uint8 base_high;
} __attribute__((packed));


/* Special pointer which includes the limit: The max bytes
*  taken up by the GDT, minus 1. */
struct gdt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

/* Our GDT, with 3 entries, and finally our special GDT pointer */
struct gdt_entry gdt[3];
struct gdt_ptr gdtPtr;

/* This is a function in gdt.asm. We use this to properly
*  reload the new segment registers */
extern void gdt_flush();

#endif //BORIX_GDT_H
