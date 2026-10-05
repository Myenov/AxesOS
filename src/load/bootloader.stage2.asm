[org 0x7E00]
[bits 16]

%include "bootloader.inc"

stage2_start:
    cli
    mov [boot_drive], dl

    in al, 0x92
    or al, 0x02
    out 0x92, al

    mov di, E820_ADDR
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
    mov [E820_COUNT_ADDR], bp

    mov si, dap_read_kernel
    mov ah, 42h
    mov dl, [boot_drive]
    int 13h
    jc disk_error

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
.set_pd:
    mov dword [edi], ebx
    add ebx, 0x200000
    add edi, 8
    loop .set_pd

    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, 0x1000
    mov cr3, eax

    mov eax, cr0
    or eax, (1 << 31) | (1 << 0)
    mov cr0, eax

    lgdt [gdt_descriptor]
    jmp 0x08:code64_start

disk_error:
    cli
.hang:
    hlt
    jmp .hang

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
    mov di, [E820_COUNT_ADDR]

    mov rax, KERNEL_OFFSET
    call rax

.hang64:
    cli
    hlt
    jmp .hang64

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
    dw KERNEL_SECTORS
    dw KERNEL_OFFSET & 0xFFFF
    dw (KERNEL_OFFSET >> 4) & 0xFFFF
    dq 1 + STAGE2_SECTORS

boot_drive db 0

times (STAGE2_SECTORS * 512) - ($ - $$) db 0