#ifndef STRING_H
#define STRING_H

#include "types.h"

uint16 strLength (string str);

bool strEquals (string s1, string s2);

void strReverse (string s);

void strCopy (string dest, string src);

void strReplace (string str, char ch1, char ch2);

#endif
