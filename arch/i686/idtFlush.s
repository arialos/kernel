
.global idtFlush
idtFlush:
	mov 4(%esp), %eax
	lidt (%eax)
    ret