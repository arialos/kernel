/* Declare constants for the multiboot header. */
.set ALIGN,    1<<0             /* align loaded modules on page boundaries */
.set MEMINFO,  1<<1             /* provide memory map */
.set FRAMEBUFFER, 1<<2           /* provide framebuffer information */
.set FLAGS,    ALIGN | MEMINFO | FRAMEBUFFER  /* this is the Multiboot 'flag' field */
.set MAGIC,    0x1BADB002       /* 'magic number' lets bootloader find the header */
.set CHECKSUM, -(MAGIC + FLAGS) /* checksum of above, to prove we are multiboot */
 

.section .multiboot
    .align 4
multiboot_header:
    .long MAGIC
    .long FLAGS
    .long CHECKSUM
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0 # 1 = Text mode only, 0 = Graphics
    .long 0 # Screen Width
    .long 0 # Screen Height
    .long 32
 
.section .bss
    .align 16
stack_bottom:
    .skip 16384 # 16 KiB
stack_top:
 

.section .text
    .global _start
_start:
	mov $stack_top, %esp

    push %eax
    push %ebx
    
	call main
	
	cli

1:	hlt
	jmp 1b
 
.size _start, . - _start
