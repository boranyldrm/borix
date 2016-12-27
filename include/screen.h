#ifndef SCREEN_H
#define SCREEN_H

#include "types.h"
#include "system.h"
#include "string.h"

int cursorX;
int cursorY;

//screen width, height and depth
const uint8 width, height, depth;

//clear line from "from" to "to"
void clearLine(uint8 from, uint8 to);

//update cursor position to x and y
void updateCursor(int x, int y);

void clearScreen();

//scroll the screen lineNumber up
void scrollUp(uint8 lineNumber);

void newLineCheck();

void printch(char c);

void printk(string str);

void setScreenColor(int textColor, int backgColor);

void setColorCode(int code); // set screen color from color code

void printColor(string str, int textColor, int backgColor);

#endif
