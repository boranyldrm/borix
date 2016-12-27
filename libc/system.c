#include "../include/system.h"

//Read a single 8-bit I/O port. 
uint8 inportb (uint16 _port){
    uint8 rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

//Write a single byte to an 8-bit port. 
void outportb (uint16 _port, uint8 _data){
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

