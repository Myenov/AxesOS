[org 0x7C00]
[bits 16]

%include "bootloader.inc"

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    mov [boot_drive], dl

    mov ah, 41h
    mov bx, 55AAh
    mov dl, [boot_drive]
    int 13h
    jc disk_error
    test cx, 1
    jz disk_error
    cmp bx, 0AA55h
    jne disk_error

    mov si, dap_read_stage2
    mov ah, 42h
    mov dl, [boot_drive]
    int 13h
    jc disk_error

    mov dl, [boot_drive]
    jmp STAGE2_OFFSET

disk_error:
    cli
.hang:
    hlt
    jmp .hang

align 8
dap_read_stage2:
    db 0x10
    db 0
    dw STAGE2_SECTORS
    dw STAGE2_OFFSET
    dw 0x0000
    dq 1

boot_drive db 0

times 510 - ($ - $$) db 0
dw 0xAA55