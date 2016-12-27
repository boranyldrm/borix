COMPILER = gcc
LINKER = ld
ASSEMBLER = nasm
CFLAGS = -m32 -c -ffreestanding
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T linker/linker.ld
EMULATOR = qemu-system-i386
EMULATOR_FLAGS = -kernel

OUTS = outputs/kernelasm.o outputs/kernelc.o outputs/idt.o outputs/isr.o outputs/keyboard.o outputs/screen.o outputs/string.o outputs/system.o outputs/util.o outputs/shell.o outputs/sum.o outputs/echo.o outputs/arr_func.o outputs/process.o outputs/file.o outputs/file_sys.o outputs/alloc.o outputs/vmm.o outputs/list.o
OUTPUT = boot/kernel.bin

run: all
	$(EMULATOR) $(EMULATOR_FLAGS) $(OUTPUT)

all:$(OUTS)
	$(LINKER) $(LDFLAGS) -o $(OUTPUT) $(OUTS)

outputs/kernelasm.o:kernel/kernel.asm
	$(ASSEMBLER) $(ASFLAGS) -o outputs/kernelasm.o kernel/kernel.asm

outputs/kernelc.o:kernel/kernel.c
	$(COMPILER) $(CFLAGS) kernel/kernel.c -o outputs/kernelc.o

outputs/idt.o:libc/interrupts/idt.c
	$(COMPILER) $(CFLAGS) libc/interrupts/idt.c -o outputs/idt.o

outputs/isr.o:libc/interrupts/isr.c
	$(COMPILER) $(CFLAGS) libc/interrupts/isr.c -o outputs/isr.o

outputs/keyboard.o:libc/keyboard.c
	$(COMPILER) $(CFLAGS) libc/keyboard.c -o outputs/keyboard.o

outputs/screen.o:libc/screen.c
	$(COMPILER) $(CFLAGS) libc/screen.c -o outputs/screen.o

outputs/string.o:libc/string.c
	$(COMPILER) $(CFLAGS) libc/string.c -o outputs/string.o

outputs/system.o:libc/system.c
	$(COMPILER) $(CFLAGS) libc/system.c -o outputs/system.o

outputs/util.o:libc/util.c
	$(COMPILER) $(CFLAGS) libc/util.c -o outputs/util.o

outputs/shell.o:libc/shell.c
	$(COMPILER) $(CFLAGS) libc/shell.c -o outputs/shell.o

outputs/list.o:libc/list.c
	$(COMPILER) $(CFLAGS) libc/list.c -o outputs/list.o

outputs/sum.o:commands/c/sum.c
	$(COMPILER) $(CFLAGS) commands/c/sum.c -o outputs/sum.o

outputs/echo.o:commands/c/echo.c
	$(COMPILER) $(CFLAGS) commands/c/echo.c -o outputs/echo.o

outputs/arr_func.o:commands/helper_functions/array_functions.c
	$(COMPILER) $(CFLAGS) commands/helper_functions/array_functions.c -o outputs/arr_func.o

outputs/process.o:process/process.c
	$(COMPILER) $(CFLAGS) process/process.c -o outputs/process.o

outputs/file.o:fs/file.c
	$(COMPILER) $(CFLAGS) fs/file.c -o outputs/file.o

outputs/file_sys.o:fs/filesystem.c
	$(COMPILER) $(CFLAGS) fs/filesystem.c -o outputs/file_sys.o

outputs/alloc.o:mm/alloc.c
	$(COMPILER) $(CFLAGS) mm/alloc.c -o outputs/alloc.o

outputs/vmm.o:mm/vmm.c
	$(COMPILER) $(CFLAGS) mm/vmm.c -o outputs/vmm.o

build:all
	#Activate the install xorr if you do not have it already installed
	#sudo apt-get install xorriso

	#mkdir boot/grub/
	echo set default=0 >> boot/grub/grub.cfg
	echo set timeout=0 >> boot/grub/grub.cfg
	echo menuentry "borix" { >> boot/grub/grub.cfg
	echo         set root='(hd96)' >> boot/grub/grub.cfg
	echo         multiboot /boot/kernel.bin >> boot/grub/grub.cfg
	echo } >> boot/grub/grub.cfg

	grub-mkrescue -o borix.iso iso/

clean:
	rm -f outputs/*.o
	rm -r -f boot/*.bin
