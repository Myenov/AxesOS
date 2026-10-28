#include "../lib/video/video.h"
#include "../drivers/int/isr.h"
#include "../drivers/int/pic.h"
#include "../lib/keyboard/toclick.h"
#include "../lib/memory/allocator.h"
#include "../lib/memory/alloc.h"
#include "../lib/string/str.h"
#include "../lib/clock/clock.h"
#include "../lib/signature/signature_search.h"
#include "../lib/apic/xsdt/xsdt.h"
#include "../lib/apic/rsdp/rsdp.h"
#include "../lib/apic/headers_search.h"
#include <stdint.h>

// #define KERNEL_DEBUG
extern void init_tts();

__attribute__((section(".text.entry")))
extern void kernel_main(uint64_t e820_count) {
    init_tts();
    idt_init();
    clear_screen();
    set_default_color(COLOR_WHITE);
    set_default_fon_color(COLOR_BLACK);

    volatile uint16_t ebda_segment = *(volatile uint16_t *)0x040E;
    if (ebda_segment == 0) {
        print("EBDA - zero.\n");
        while (1) {
            asm volatile("hlt");
        }
    }
    uintptr_t ebda_address = ((uintptr_t)ebda_segment) << 4;
    
    volatile uint8_t ebda_size_kb = *(volatile uint8_t *)ebda_address;

    #ifdef KERNEL_DEBUG
    char numbuffer[21]; 
    print("ebda size: ");
    print(uint64_to_str((uint64_t)ebda_size_kb, numbuffer));
    print("kb\n");
    #endif
    
    init_alloc_driver(e820_count, ebda_address, ebda_size_kb);

    const uint8_t rsdp_sign[8] = { 0x52, 0x53, 0x44, 0x20, 0x50, 0x54, 0x52, 0x20 };
    const char* mask = "xxxxxxxx";
    uint8_t* rsdp_address = find_sign((unsigned char*)ebda_address, ebda_size_kb*1024, rsdp_sign, mask);
    
    if (rsdp_address == NULL) {
        rsdp_address = find_sign((unsigned char*)0x000E0000, 0x000FFFFF-0x000E0000+1, rsdp_sign, mask);
    }

    if (rsdp_address == NULL) {
        print("rsdp_address - zero.\n");
        while (1) {
            asm volatile("hlt");
        }
    }

    #ifdef KERNEL_DEBUG
    for (uint8_t i = 0; i < 21; i++) numbuffer[i] = ' ';
    for (int i = 0; i < 36; i++) {
        print(uint64_to_str(rsdp_address[i], numbuffer));
        print_symbol(' ');
    }
    print("rsdp address: ");
    for (uint8_t i = 0; i < 21; i++) numbuffer[i] = ' ';
    print(ptr_to_str(rsdp_address, numbuffer));
    print_symbol('\n');
    #endif

    struct HPETTable *hpet;
    struct RSDPDescriptor20 *rsdp = search_ACPIHeaders(rsdp_address);
    struct XSDT *xsdt = NULL;
    struct RSDT *rsdt = NULL;
    #ifdef KERNEL_DEBUG
    print("rsdp revision: ");
    print(uint64_to_str(rsdp->firstPart.revision, numbuffer));
    print_symbol('\n');
    #endif
    
    if (rsdp->firstPart.revision < 2) {
        rsdt = (struct RSDT *)(uintptr_t)rsdp->firstPart.rsdt_address;
        reserve_range((uint64_t)rsdt, (uint64_t)rsdt+rsdt->header.length);

        #ifdef KERNEL_DEBUG
        for (uint8_t i = 0; i < 21; i++) numbuffer[i] = ' ';
        print("rsdt length: ");
        print(uint64_to_str(rsdt->header.length, numbuffer));
        print_symbol('\n');
        #endif
        
        hpet = search_HPET_table_in_RSDT(rsdt);
    } else {
        xsdt = (struct XSDT *)rsdp->xsdt_address;
        reserve_range((uint64_t)xsdt, (uint64_t)xsdt+xsdt->header.length);
        
        #ifdef KERNEL_DEBUG
        for (uint8_t i = 0; i < 21; i++) numbuffer[i] = ' ';
        print("xsdt length: ");
        print(uint64_to_str(xsdt->header.length, numbuffer));
        print_symbol('\n');
        #endif

        hpet = search_HPET_table_in_XSDT(xsdt);
    }
    
    
    #ifdef KERNEL_DEBUG
    for (uint8_t i = 0; i < 21; i++) numbuffer[i] = ' ';
    print("HPET: ");
    print(ptr_to_str(hpet, numbuffer));
    print_symbol('\n');

    for (uint8_t i = 0; i < 21; i++) numbuffer[i] = ' ';
    print("HPET base: ");
    print(ptr_to_str((void*)hpet->base_address.address, numbuffer));
    print_symbol('\n');
    #endif  

    if (hpet == NULL) {
        print("HPET table not found!\n");
        while (1) {
            asm volatile("hlt");
        }
    }

    #ifdef KERNEL_DEBUG
    print("HPET signature: ");
    for (int i = 0; i < 4; i++) {
        print_symbol(hpet->header.sign[i]);
    }
    print_symbol('\n');
    #endif
    reserve_range((uint64_t)hpet, (uint64_t)hpet+sizeof(struct HPETTable));
    reserve_range(hpet->base_address.address, hpet->base_address.address + 0x400);
    init_clock_driver(hpet);
    
    
    init_pic();
    init_keyboard_driver();
    asm volatile("sti");

    // char *buffer = (char *)malloc(21);
    // if (!buffer) {
    //     print("Buffer noalloc");
    // }
    
    // uint64_t *test = (uint64_t *)malloc(8);
    // if (!test) {
    //     free(buffer);
    //     print("Test noalloc");
    // }
    
    // *test = (uint64_t)rdtsc_serialized();
    // char numbuf[21];
    // print(uint64_to_str(*test, numbuf));
    
    // free(test);
    // free(buffer);
    
    // terminal_main();
    // while(1) {
    //     print_symbol('|');
    //     to_backspace();
    //     if (is_new_data_available()) print_symbol(get_last_click());
    // }

    while (1) {
        asm volatile("hlt");
    }
}