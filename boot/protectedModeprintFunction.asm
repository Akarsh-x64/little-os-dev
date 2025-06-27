[bits 32]

VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

printProtected:
	pusha
	mov edx, VIDEO_MEMORY

printProtectedloop:
	mov al, [ebx]
	mov ah, WHITE_ON_BLACK
	
	cmp al, 0
	je printProtectedDone
	
	mov [edx], ax
	add ebx, 1
	add edx, 2
	
	jmp printProtectedloop

printProtectedDone:
	popa
	ret
