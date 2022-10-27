.global gdtFlush

gdtFlush:
	movl 4(%esp), %eax
	lgdt (%eax)
	jmp $0x08, $jmpFlush
jmpFlush:
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	ret
