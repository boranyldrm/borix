/*
   Declared new types which will used in other files.
   */

#ifndef TYPES_H
#define TYPES_H

//NULL
#define NULL 0

//8 bits
typedef signed char int8;
typedef unsigned char uint8;

//16 bits
typedef signed short int16;
typedef unsigned short uint16;

//32 bits
typedef signed int int32;
typedef unsigned int uint32;

//64 bits
typedef signed long long int64;
typedef unsigned long long uint64;

//string
typedef char* string;

//boolean
typedef uint8 bool;
#define true 1
#define false 0

//take an 32 bit integer and devide 16 16
#define low_16(address) (uint16)((address) & 0xFFFF)
#define high_16(address) (uint16)(((address) >> 16) & 0xFFFF)

#define SELECTOR_LDT_FIRST 0x28

#endif
