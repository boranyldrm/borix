#include "../../include/interrupts/idt.h"

void set_idt_gate(uint32 n, uint32 handler) {
    idt[n].low_offset = low_16(handler);
    idt[n].selector = KERNEL_CS;
    idt[n].always0 = 0;
    idt[n].flag = 0x8E;
    idt[n].high_offset = high_16(handler);
}

void set_idt() {
    idt_reg.base = (uint32) &idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    __asm__ __volatile__ ("lidtl (%0)" : : "r" (&idt_reg));
}
