section .text
[bits 64]

extern isr_handler
extern irq_handler

%macro ISR_NOERRORCODE 1
global isr%1
isr%1:
    push 0
    push %1
    jmp isr_common
%endmacro

%macro ISR_ERRORCODE 1
global isr%1
isr%1:
    push %1
    jmp isr_common
%endmacro

%macro IRQ 2
global irq%1
irq%1:
    push 0
    push %2
    jmp irq_common
%endmacro

; вектора исключений
ISR_NOERRORCODE 0 ; деление на 0
ISR_NOERRORCODE 1 ; отладка
ISR_NOERRORCODE 2 ; nmi
ISR_NOERRORCODE 3 ; точка останова
ISR_NOERRORCODE 4 ; переполнение
ISR_NOERRORCODE 5 ; выход за границы
ISR_NOERRORCODE 6 ; неверный опкод
ISR_NOERRORCODE 7 ; устройство недоступно
ISR_ERRORCODE 8 ; двойная ошибка (есть код ошибки)
ISR_NOERRORCODE 9 ; сопроцессор (устарело)
ISR_ERRORCODE 10 ; Неверный TSS (есть код ошибки)
ISR_ERRORCODE 11 ; сегмент не присутствует (есть код ошибки)
ISR_ERRORCODE 12 ; ошибка стека (есть код ошибки)
ISR_ERRORCODE 13 ; общая защита (есть код ошибки)
ISR_ERRORCODE 14 ; ошибка страницы (есть код ошибки)
ISR_NOERRORCODE 15 ; зарезервировано
ISR_NOERRORCODE 16 ; ошибка FPU 
ISR_ERRORCODE 17 ; ошибка выравнивания (есть код ошибки)
ISR_NOERRORCODE 18 ; ошибка машинной проверки
ISR_NOERRORCODE 19 ; SIMD исключение
ISR_NOERRORCODE 20 ; виртуализация
ISR_ERRORCODE 21 ; ошибка управления
ISR_NOERRORCODE 22 ; Зарезервировано
ISR_NOERRORCODE 23 ; Зарезервировано
ISR_NOERRORCODE 24 ; Зарезервировано
ISR_NOERRORCODE 25 ; Зарезервировано
ISR_NOERRORCODE 26 ; Зарезервировано
ISR_NOERRORCODE 27 ; Зарезервировано
ISR_NOERRORCODE 28 ; Зарезервировано
ISR_NOERRORCODE 29 ; Зарезервировано
ISR_ERRORCODE 30 ; Ошибка безопасности (есть код ошибки)
ISR_NOERRORCODE 31 ; Зарезервировано

; вектора IRQ (32-47)
IRQ 0, 32 ; таймер
IRQ 1, 33 ; клавиатура
IRQ 2, 34 ; каскад для 8259А
IRQ 3, 35 ; COM2
IRQ 4, 36 ; COM1
IRQ 5, 38 ; LPT2
IRQ 6, 39 ; Floppy
IRQ 7, 40 ; LPT1
IRQ 8, 41 ; RTC
IRQ 9, 42 ; свободно
IRQ 10, 43 ; свободно
IRQ 11, 44 ; свободно
IRQ 12, 45 ; PS/2 мышь
IRQ 13, 46 ; FPU
IRQ 14, 47 ; Primary ATA
IRQ 15, 48 ; Secondary ATA

; общий обработчик исключений
isr_common:
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    mov ax, ds
    push rax
    mov ax, es
    push rax

    mov ax, 0x10 ; селектор данных ядра
    mov ds, ax
    mov es, ax

    mov rdi, rsp
    call isr_handler

    pop rax
    mov es, ax
    pop rax
    mov ds, ax

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax

    add rsp, 16 ; убираем vector_number и error_code

    iretq

; общий обработчик IRQ
irq_common:
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    
    mov ax, ds
    push rax
    mov ax, es
    push rax
    
    mov ax, 0x10 ; селектор данных ядра
    mov ds, ax
    mov es, ax
    
    mov rdi, rsp
    call irq_handler
    
    pop rax
    mov es, ax
    pop rax
    mov ds, ax
    
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    
    add rsp, 16 ; убираем vector_number и error_code
    
    iretq