//
// Created by boran on 30.09.2016.
//

#include "../h/sum.h"

void sum() {
    printk("\nHow many numbers: ");
    uint16 num = (uint16) parseInt(readInput());
    printk("\n");
    uint16 arr[num];
    fill_arr(arr, num);
    uint32 sum = sum_arr(arr, num);
    printk("Sum: ");
    printk(toString(sum));
    printk("\n");
}