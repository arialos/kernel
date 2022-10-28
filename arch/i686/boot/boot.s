/* Declare constants for the multiboot header. */
.set ALIGN,         1<<0             /* align loaded modules on page boundaries */
.set MEMINFO,       1<<1             /* provide memory map */
.set FRAMEBUFFER,   1<<2           /* provide framebuffer information */
.set FLAGS,         ALIGN | MEMINFO | FRAMEBUFFER  /* this is the Multiboot 'flag' field */
.set MAGIC,         0x1BADB002       /* 'magic number' lets bootloader find the header */
.set CHECKSUM,      -(MAGIC + FLAGS) /* checksum of above, to prove we are multiboot */

/* Declare the multiboot header. */
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
    .long 720 # Screen Width
    .long 1024 # Screen Height
    .long 32
 
/* Declare the kernel stack. */
.section .stack
stack_bottom:
    .skip 32768 # 32 KiB
stack_top:
 

/* Declare the kernel entry point. */
.section .text
.global _start

_start:
    cli
	mov $stack_top, %esp
    push %eax # push the multiboot header onto the stack
    push %ebx # push the multiboot magic number onto the stack
	call main
    

.Lhang:
    cli
    hlt
	jmp .Lhang