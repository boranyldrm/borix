bits	32
section	.text
	align	4
	dd	0x1BADB002	;##magic number
	dd	0x00
	dd	-(0x1BADB002 + 0x00)

global start
extern kmain	;##this function is located in "kernel.c"
start:
	cli	;##clears interrupts
	call kmain
	hlt	;##pause CPU executing from this address
