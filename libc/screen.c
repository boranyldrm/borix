#include "../include/screen.h"

int cursorX = 0;
int cursorY = 0;

int color = 0x0F; //stores current color of terminal

//screen width, height and depth
const uint8 width = 80, height = 25, depth = 2;

void clearLine(uint8 from, uint8 to) {
    uint16 i = width * from * depth;
    string video = (string)0xb8000;
    while (i < (width * (to + 1) * depth)) {
            video[(i / 2) * 2 + 1] = (char) color;
            video[(i /2) * 2] = 0;
            i++;
    }
}

//update cursor position to x and y
void updateCursor(int x, int y) {
    cursorX = x;
    cursorY = y;

    unsigned position = (unsigned int) (cursorX + cursorY * width);

    outportb(0x3D4, 14);    //select cursor location
    outportb(0x3D5, (uint8) (position >> 8)); //send the high byte across the bus
    outportb(0x3D4, 15);    //select send low byte
    outportb(0x3D5, (uint8) position);  //send the low byte of the cursor location
}

void clearScreen() {
    clearLine(0, height - 1);
    updateCursor(0, 0);
}

//scroll the screen lineNumber up
void scrollUp(uint8 lineNumber) {
    string video = (string)0xb8000;
    uint16 i = 0;
    clearLine(0, (uint8) (lineNumber - 1));
    while (i < (width * (height - 1) * depth)) {
        video[i] = video[i + width * depth * lineNumber];
        i++;
    }
    clearLine((uint8) (height - 1 - lineNumber), height - 1);

    if ((cursorY - lineNumber) < 0){
        updateCursor(0, 0);
    }
    else {
        updateCursor(cursorX, cursorY - lineNumber);
    }
}

void newLineCheck() {
    if (cursorY >= (height - 1))
        scrollUp(1);
}


void printch(char ch) {
    string video = (string) 0xb8000;

    switch (ch) {
        case (0x08):    //backspace character
            if ((cursorX % 80) > strLength("borix::shell:$ ")) {
                cursorX--;
                video[(cursorY * width + cursorX) * depth] = 0x00;
            }
            break;
        case (0x09):    //tab character
            cursorX = (cursorX + 4) & ~(4 - 1);
            break;
        case ('\r'):
            cursorX = 0;
            break;
        case ('\n'):
            cursorX = 0;
            cursorY ++;
            break;
        default:
            video [((cursorY * width + cursorX)) * depth] = ch;
            video [((cursorY * width + cursorX)) * depth + 1] = (char) color;
            cursorX++;
            break;
    }

    if (cursorX >= width) {
        cursorX = 0;
        cursorY++;
    }

    newLineCheck();
    updateCursor(cursorX, cursorY);
}

void printk(string str) {
    uint16 i = 0;
    uint8 length = (uint8) strLength(str);
    while (i < length) {
        printch(str[i]);
        i++;
    }
}

void setScreenColor(int textC, int backC) {
    color = (backC << 4) | textC;
}

void setScreenCode(int code) {
    color = code;
}

void printColor(string str, int textC, int backC) {
    int cur = color;
    setScreenColor(textC, backC);
    printk(str);
    setScreenCode(cur);
}
