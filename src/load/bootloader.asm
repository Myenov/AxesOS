[org 0x7c00]
[bits 16]

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
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

    mov si, dap_read_kernel
    mov ah, 42h
    mov dl, [boot_drive]
    int 13h
    jc disk_error

    in al, 0x92
    or al, 0x02
    out 0x92, al

    
    mov di, 0x6000
    xor ebx, ebx
    xor bp, bp
    mov edx, 0x534D4150
    mov ecx, 24

.e820_loop:
    mov eax, 0xE820
    int 0x15
    jc .e820_done
    inc bp
    test ebx, ebx
    jz .e820_done
    add di, 24
    jmp .e820_loop

.e820_done:
    mov [0x5000], bp
    jmp .PML4_init

.PML4_init:
    mov edi, 0x1000
    xor eax, eax
    mov ecx, 0x1000
    a32 rep stosd
    mov edi, 0x1000

    mov dword [edi], 0x2003
    add edi, 0x1000
    mov dword [edi], 0x3003
    add edi, 0x1000

    mov ebx, 0x83
    mov ecx, 512

.set_pt:
    mov dword [edi], ebx
    add ebx, 0x200000
    add edi, 8
    loop .set_pt

    mov eax, 0x1000
    mov cr3, eax

    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, cr0
    or eax, 1 << 31 | 1 << 0
    mov cr0, eax

    lgdt [gdt_descriptor]
    jmp 0x08:code64_start

[bits 64]
code64_start:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov rsp, 0x90000
    mov rbp, rsp

    xor rdi, rdi
    mov di, [0x5000]

    mov rax, 0x10000
    call rax

.hang:
    cli
    hlt
    jmp .hang

disk_error:
    cli
.error:
    hlt
    jmp .error

align 8
gdt_start:
    dq 0
gdt_code:
    dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53)
gdt_data:
    dq (1 << 41) | (1 << 44) | (1 << 47)
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dq gdt_start

dap_read_kernel:
    db 0x10
    db 0
    dw 128
    dw 0x0000
    dw 0x1000
    dq 1

boot_drive db 0

times 510 - ($ - $$) db 0
dw 0xAA55