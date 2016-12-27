#ifndef SYSTEM_H
#define SYSTEM_H

#include "types.h"

//Read a single 8-bit I/O port. 
uint8 inportb (uint16 _port);

//Write a single byte to an 8-bit port. 
void outportb (uint16 _port, uint8 _data);

#endif
