//
// Created by boran on 30.09.2016.
//

#include "array_functions.h"


void fill_arr(uint16 arr[], uint16 n) {
    for (uint16 i = 0; i < n; i++) {
        printk(toString(i + 1)); //user can be non-programmer (count from 1)
        printk("th: ");
        arr[i] = (uint16) parseInt(readInput());
        printk("\n");
    }
}

void print_arr(uint16 arr[], uint16 n) {
    for (uint16 i = 0; i < n; ++i) {
        printk(toString(arr[i]));
        printk("  ");
    }
    printk("\n");
}

uint32 sum_arr(uint16 arr[], uint16 n) {
    uint32 sum = 0;
    for (uint16 i = 0; i < n; ++i) {
        sum += arr[i];
    }
    return sum;
}