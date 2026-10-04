#include "../drivers/video/video.h"
#include "../drivers/int/isr.h"
#include "../drivers/int/pic.h"
#include "../drivers/keyboard/toclick.h"
#include "../drivers/alloc/allocator.h"
#include "../drivers/alloc/alloc.h"
#include "../drivers/str.h"
#include <stdint.h>
extern void init_tts();

__attribute__((section(".text.entry")))
extern void kernel_main(uint64_t e820_count) {
    init_tts();
    idt_init();
    clear_screen();
    set_default_color(COLOR_WHITE);
    set_default_fon_color(COLOR_BLACK);
    init_alloc_driver(e820_count);
    init_pic();
    init_keyboard_driver();
    asm volatile("sti");


    char *buffer = (char *)malloc(21);
    if (!buffer) {
        print("Buffer noalloc");
    }
    
    uint64_t *test = (uint64_t *)malloc(8);
    if (!test) {
        free(buffer);
        print("Test noalloc");
    }
    
    *test = 12345679;
    char numbuf[21];
    print(uint64_to_str(*test, numbuf));
    
    free(test);
    free(buffer);
    
    
    while(1) {
        print_symbol('|');
        to_backspace();
        if (is_new_data_available()) print_symbol(get_last_click());
    }

    while (1) {
        asm volatile("hlt");
    }
}