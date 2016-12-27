#include "../include/util.h"

#define DIGITS 19

void memcpy(char *source, char *dest, int nbytes) {
    for (int i = 0; i < nbytes; i++)
        dest[i] = source[i];
}

void memory_set(char *dest, uint8 value, uint32 length) {
    char *tmp = (char *) dest;
    while (length != 0) {
        *tmp++ = value;
        length--;
    }
}

string toString(int n) {
    static char buf[DIGITS + 2]; // 2 is stand for - and \0
    char *tmp = buf + DIGITS + 1;
    if (n >= 0) {
        do {
            *--tmp = (char) ('0' + (n % 10));
            n /= 10;
        } while (n != 0);
        return tmp;
    }
    else {
        do {
            *--tmp = (char) ('0' - (n % 10));
            n /= 10;
        } while (n != 0);
        *--tmp = '-';
    }
    return tmp;
}

int parseInt(string str) {

    if (str[0] == '\0')
        return 0;

    uint32 parsed = 0;
    uint8 sign = 1;
    uint8 i = 0;

    if (str[0] == '-') {
        sign = (uint8) -1;
        i++;
    }
    while (str[i] != '\0') {
        // If string contain character it will terminate
        if (str[i] <= '0' || str[i] >= '9')
            return 0;
        parsed = parsed * 10 + str[i] - '0';
        i++;
    }
    return sign * parsed;
}



