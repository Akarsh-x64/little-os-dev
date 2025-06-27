[bits 32]
[extern init]
call init ; here the kernel.c takes over and we try to stay here forever if the kernel returns control
jmp $
