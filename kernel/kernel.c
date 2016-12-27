#include "../include/screen.h"
#include "../include/shell.h"
#include "../include/interrupts/isr.h"
#include "../fs/filesystem.h"

File_System *fsm;
void kmain() {

    isr_install();
    clearScreen();
    vmm_init(4000);
    fs_init(fsm);

    launchShell();

}