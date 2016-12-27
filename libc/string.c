#include "../include/string.h"

uint16 strLength(const string str) {
    string s;
    for (s = str; *s; ++s);
    return (uint16) (s - str);
}

bool strEquals(string s1, string s2) {
    if (s1[0] == '\0' && s2[0] != '\0')
        return false;
    uint8 i = 0;
    while (s1[i] != '\0') {
        if (s2[i] == '\0' || s1[i] != s2[i])
            return false;
        i++;
    }
    return (bool) (s2[i] == '\0');
}

void strReverse(string s) {
    int length = strLength(s);
    char tmp;

    for (int i = 0, j = length - 1; i < j; i++, j++) {
        tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
    }
}

void strCopy(string dest, string src) {
   string s = src;
    while (*s) {
        *dest = *s;
        dest++;
        s++;
    }
}

void strReplace (string str, char ch1, char ch2) {
    if (str != NULL) {
        while (*str) {
            if (*str == ch1)
                *str = ch2;
            str++;
        }
    }
}
