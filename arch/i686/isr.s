.global isr0
.global isr1
.global isr2
.global isr3
.global isr4
.global isr5
.global isr6
.global isr7
.global isr8
.global isr9
.global isr10
.global isr11
.global isr12
.global isr13
.global isr14
.global isr15
.global isr16
.global isr17
.global isr18
.global isr19
.global isr20
.global isr21
.global isr22
.global isr23
.global isr24
.global isr25
.global isr26
.global isr27
.global isr28
.global isr29
.global isr30
.global isr31

isr0:
    cli
    push $0
    push $0
    jmp common_isr_stub_handler
isr1:
    cli
    push $1
    push $1
    jmp common_isr_stub_handler
isr2:
    cli
    push $2
    push $2
    jmp common_isr_stub_handler

isr3:
    cli
    push $3
    push $3
    jmp common_isr_stub_handler
isr4:
    cli
    push $4
    push $4
    jmp common_isr_stub_handler
isr5:
    cli
    push $5
    push $5
    jmp common_isr_stub_handler
isr6:
    cli
    push $6
    push $6
    jmp common_isr_stub_handler
isr7:
    cli
    push $7
    push $7
    jmp common_isr_stub_handler
isr8:
    cli
    push $8
    push $8
    jmp common_isr_stub_handler
isr9:
    cli
    push $9
    push $9
    jmp common_isr_stub_handler
isr10:
    cli
    push $10
    push $10
    jmp common_isr_stub_handler
isr11:
    cli
    push $11
    push $11
    jmp common_isr_stub_handler
isr12:
    cli
    push $12
    push $12
    jmp common_isr_stub_handler
isr13:
    cli
    push $13
    push $13
    jmp common_isr_stub_handler
isr14:
    cli
    push $14
    push $14
    jmp common_isr_stub_handler
isr15:
    cli
    push $15
    push $15
    jmp common_isr_stub_handler
isr16:
    cli
    push $16
    push $16
    jmp common_isr_stub_handler
isr17:
    cli
    push $17
    push $17
    jmp common_isr_stub_handler
isr18:
    cli
    push $18
    push $18
    jmp common_isr_stub_handler
isr19:
    cli
    push $19
    push $19
    jmp common_isr_stub_handler
isr20:
    cli
    push $20
    push $20
    jmp common_isr_stub_handler
isr21:
    cli
    push $21
    push $21
    jmp common_isr_stub_handler
isr22:
    cli
    push $22
    push $22
    jmp common_isr_stub_handler
isr23:
    cli
    push $23
    push $23
    jmp common_isr_stub_handler
isr24:
    cli
    push $24
    push $24
    jmp common_isr_stub_handler
isr25:
    cli
    push $25
    push $25
    jmp common_isr_stub_handler
isr26:
    cli
    push $26
    push $26
    jmp common_isr_stub_handler
isr27:
    cli
    push $27
    push $27
    jmp common_isr_stub_handler
isr28:
    cli
    push $28
    push $28
    jmp common_isr_stub_handler
isr29:
    cli
    push $29
    push $29
    jmp common_isr_stub_handler
isr30:
    cli
    push $30
    push $30
    jmp common_isr_stub_handler
isr31:
    cli
	push $31
	push $31
	jmp common_isr_stub_handler

common_isr_stub_handler:
	pusha
	mov %ds, %eax
	push %eax
	mov $0x10, %eax
	mov %eax, %ds
	mov %eax, %es
	mov %eax, %fs
	mov %eax, %gs
	call isrHandler
	pop %eax
	mov %eax, %ds
	mov %eax, %es
	mov %eax, %fs
	mov %eax, %gs
	popa
	add $8, %esp
	sti
	iret
