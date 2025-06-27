[org 0x7c00]
KERNEL_OFFSET equ 0x1000
	mov [BOOT_DRIVE], dl

	mov bp, 0x90000
	mov sp, bp

	mov bx, DEBUG_REAL_MODE
	call print
	call print_nl

	call kernelINIT
	call protectedModeInit
	jmp $

%include "./boot/printFunction.asm"
%include "./boot/printhexFunction.asm"
%include "./boot/bootSectorDisk.asm"
%include "./boot/protectedModeGDT.asm"
%include "./boot/protectedModeprintFunction.asm"
%include "./boot/protectedModeinit.asm"

[bits 16]
kernelINIT:
	mov bx, DEBUG_KERNEL_LOADING
	call print
	call print_nl
	
	mov bx, KERNEL_OFFSET	
	mov dh, 3               ;NUMBER OF SECTORS TO LOAD
	mov dl, [BOOT_DRIVE]
	call disk_load
	ret

[bits 32]
PROTECTED_MODE_INIT:
	mov ebx, DEBUG_PROTECTED_MODE
	call printProtected
	call KERNEL_OFFSET
	jmp $

BOOT_DRIVE db 0
DEBUG_REAL_MODE db "Started in 16-bit Real mode", 0
DEBUG_PROTECTED_MODE db "Landed in 32-bit Protected mode", 0
DEBUG_KERNEL_LOADING db "Loading kernel into memory (real mode)", 0

times 510-($-$$) db 0
dw 0xaa55
