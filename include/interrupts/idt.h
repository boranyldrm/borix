#ifndef IDT_H
#define IDT_H

/*
   Interrupt Descriptor Table  
   */

#include "../types.h"

//Kernel segment
#define KERNEL_CS 0x08

//defining each element of the idt
typedef struct {
    uint16 low_offset; //"types.h" low_16
    uint16 selector;
    uint16 always0;
    uint8 flag;
    uint16 high_offset; //"types.h" high_16
} __attribute__((packed)) idt_gate_t;

typedef struct {
    uint16 limit; //size of idt array
    uint32 base; //address of idt array
} __attribute__((packed)) idt_register_t;

#define IDT_ENTRIES 256
idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

//set and define the content of idt array
void set_idt_gate(uint32 n, uint32 handler);

//after defining all elements of array
void set_idt();

#endif
