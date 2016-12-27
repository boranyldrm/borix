#ifndef UTIL_H
#define UTIL_H

#include "types.h"
#include "string.h"
#include "../mm/alloc.h"

//copy bytes from source to dest
void memcpy(char *source, char *dest, int nbytes);

//set a value to dest
void memory_set(char *dest, uint8 value, uint32 length);

//converts an integer to string (int to ascii) itoa
string toString(int n);

//converts a string to integer (ascii to int) atoi
int parseInt(string str);

#endif
