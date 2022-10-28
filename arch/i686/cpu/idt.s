
.global idtFlush.type idtFlush, @function idtFlush : mov 4(% esp), % eax lidt(% eax) ret