cmake_minimum_required(VERSION 3.6)
project(borix)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    boot/kernel.bin
        libc/interrupts/idt.c
    include/interrupts/idt.h
        libc/interrupts/isr.c
    include/interrupts/isr.h
        libc/keyboard.c
    include/keyboard.h
        libc/screen.c
    include/screen.h
        libc/shell.c
    include/shell.h
        libc/string.c
    include/string.h
        libc/system.c
    include/system.h
    include/types.h
        libc/util.c
    include/util.h
    kernel/kernel.asm
    kernel/kernel.c
    linker/linker.ld
        mm/alloc.c
        mm/alloc.h
    outputs/idt.o
    outputs/isr.o
    outputs/kernelasm.o
    outputs/kernelc.o
    outputs/keyboard.o
    outputs/README.md
    outputs/screen.o
    outputs/shell.o
    outputs/string.o
    outputs/system.o
    outputs/util.o
    LICENSE.md
    makefile
    README.md commands/h/sum.h commands/h/echo.h commands/helper_functions/array_functions.h commands/all_commands.h include/gdt.h libc/gdt.c commands/c/echo.c commands/c/sum.c commands/helper_functions/array_functions.c process/process.c process/process.h fs/file.h fs/file.c fs/filesystem.h fs/filesystem.c mm/vmm.h mm/vmm.c include/list.h mm/page_vars.h libc/list.c)

add_executable(borix ${SOURCE_FILES})