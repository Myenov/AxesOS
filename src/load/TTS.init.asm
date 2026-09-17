[bits 64]
global init_tts

section .data
align 16

gdt_start:
    dq 0
gdt_code:
    dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53)
gdt_data:
    dq (1 << 41) | (1 << 44) | (1 << 47)
gdt_tss:
    dq 0
    dq 0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dq gdt_start

align 16
tss_entry:
    dd 0
    dq 0
    dq 0
    dq 0
    dq 0
    dq 0
    dq 0
    dq 0
    dq 0
    dq 0
    dq 0
    dq 0
    dw 0
    dw 104

section .text
init_tts:
    mov rax, tss_entry
    mov rdi, gdt_tss

    mov word [rdi], 103

    mov [rdi + 2], ax
    shr rax, 16
    mov [rdi + 4], al
    mov byte [rdi + 5], 0x89
    mov byte [rdi + 6], 0x00
    shr rax, 8
    mov [rdi + 8], eax
    shr rax, 24
    mov [rdi + 12], eax

    lgdt [gdt_descriptor]

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    push 0x08
    lea rax, [rel .reload_cs]
    push rax
    retfq
.reload_cs:

    mov ax, 0x18
    ltr ax

    ret