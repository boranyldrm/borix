//
// Created by boran on 30.09.2016.
//

#include "../h/echo.h"

void echo() {
    printk("\n");
    string str = readInput();
    printk("\n");
    printk(str);
    printk("\n");
}