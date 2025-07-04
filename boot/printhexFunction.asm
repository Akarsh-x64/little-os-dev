print_hex:
    pusha
    mov cx, 0

hex_loop:
    cmp cx, 4 ; loop 4 times
    je end
    mov ax, dx
    and ax, 0x000f
    add al, 0x30
    cmp al, 0x39
    jle phase2
    add al, 7

phase2:
    mov bx, HEX_OUT + 5
    sub bx, cx
    mov [bx], al
    ror dx, 4
    add cx, 1
    jmp hex_loop

end:
    mov bx, HEX_OUT
    call print

    popa
    ret

HEX_OUT:
    db '0x0000',0
