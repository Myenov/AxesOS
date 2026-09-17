#include "../drivers/video/video.h"
#include "../drivers/int/isr.h"
#include "../drivers/int/pic.h"
#include "../drivers/keyboard/toclick.h"
#include "../drivers/alloc/allocator.h"
#include "../drivers/str.h"
#include "../drivers/clock/clock.h"
#include <stdint.h>

extern void init_tts();


extern void kernel_main(uint64_t e820_count) {
    init_tts();
    clear_screen();
    set_default_color(COLOR_WHITE);
    set_default_fon_color(COLOR_BLACK);
    init_alloc_driver(e820_count);
    idt_init();
    init_pic();
    init_keyboard_driver();
    asm volatile("sti");
    
    while(1) {
        print_symbol('|');
        to_backspace();
        if (is_new_data_available()) print_symbol(get_last_click());
    }

    while (1) {
        asm volatile("hlt");
    }
}