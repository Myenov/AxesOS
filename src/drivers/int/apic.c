#include "../../lib/apic/apic.h"
// #include "../../lib/msr/msr.h"
#include <stdint.h>

static inline void outb(uint16_t port, uint8_t value) {
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void init_apic() {
    // outb(0x21, 0xFF); // Маскировка master PIC
    // outb(0xA1, 0xFF); // Маскировка slave PIC
    // uint64_t apic_base_msr = read_msr(IA32_APIC_BASE_MSR);
    // apic_base_msr |= IA32_APIC_BASE_MSR_ENABLE;
    // write_msr(IA32_APIC_BASE_MSR, apic_base_msr);
    
    
    // Инициализация master PIC
    outb(0x20, 0x11);  // ICW1: начать инициализацию
    outb(0x21, 0x20);  // ICW2: базовый вектор 0x20 (32)
    outb(0x21, 0x04);  // ICW3: slave на IRQ2
    outb(0x21, 0x01);  // ICW4: режим 8086


    
    // Инициализация slave PIC
    
    outb(0xA0, 0x11);  // ICW1: начать инициализацию
    outb(0xA1, 0x28);  // ICW2: базовый вектор 0x28 (40)
    outb(0xA1, 0x02);  // ICW3: slave ID 2
    outb(0xA1, 0x01);  // ICW4: режим 8086
    
    

    // Маскирование всех прерываний кроме клавиатуры (IRQ1)
    outb(0x21, 0xFD);  // 11111101 - разрешаем только IRQ1 (клавиатура)
    outb(0xA1, 0xFF);  // Маскируем все прерывания slave PIC
}
