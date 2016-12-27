#include "../include/shell.h"

void launchShell() {
    string str = (string) malloc(200);
    do {
        printk("borix::shell:$ ");
        str = readInput();
        if (strEquals(str, "borix")) {
            printk("\nYou are in the right place!!\n");
        }
        else if (strEquals(str, "clear")) {
            clearScreen();
        }
        else {
            printk("\nInvalid command.\n");
            //printk("borix::shell:$");
        }
    } while(!strEquals(str, "exit"));
}
