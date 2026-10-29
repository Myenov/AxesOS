#include "../../lib/memory/alloc.h"
#include "../../lib/video/video.h"
#include "../../lib/keyboard/toclick.h"
#include "../../lib/clock/clock.h"
#include "../../lib/string/str.h"
#include "../../lib/cpu/cpu.h"

#include <stdbool.h>
#include <stdint.h>


#define COMMAND_LENGHT 256
bool is_run = true;

void send_command(const char* buffer) {
    // чем гуще лес - тем if else if else
    if (strcmp(buffer, "cpus") == 0) {
        char nbuf[21];
        print("\n\ncores: ");
        print(uint64_to_str(cpu_topology.count, nbuf));
        print("\n");
        for (uint32_t i = 0; i < cpu_topology.count; i++) {
            print("  apic_id=");
            print(uint64_to_str(cpu_topology.cores[i].apic_id, nbuf));
            print(" acpi_id=");
            print(uint64_to_str(cpu_topology.cores[i].acpi_processor_id, nbuf));
            print("\n");
        }
        return;
    }
    if (strcmp(buffer, "rdtsc") == 0) {
        uint64_t rdtsc_c = (uint64_t)rdtsc_serialized();
        char *buf = (char *)malloc(21);
        print_symbol('\n');
        print(uint64_to_str(rdtsc_c, buf));
        print("\n!user->:");
        free(buf);
        return;
    }
    
    if (strcmp(buffer, "exit") == 0) {
        is_run = false;
        return;
            
    } else if (strcmp(buffer, "clear") == 0) {
        clear_screen();
        print("!user->:");
        return;
    }
    print("\nundefined command");
    print("\n!user->:");
}

extern int terminal_main() {
    // clear_screen();

    char *buffer = (char *)malloc(COMMAND_LENGHT);
    
    uint16_t command_cursor = 0;
    print("!user->:");
    while (is_run) {
        if (is_new_data_available()) {
            char symbol = get_last_click();
            buffer[command_cursor] = symbol;
            if (symbol == '\n') {
                buffer[command_cursor] = '\0';
                send_command(buffer);
                command_cursor = 0;
                for (uint16_t i = 0; i < COMMAND_LENGHT; i++) buffer[i] = ' ';
                continue;
            } else if (symbol == '\b') {
                if (command_cursor > 0) {
                    command_cursor--;
                    buffer[command_cursor] = '\0';
                    to_backspace();
                }
                continue;
            }
            command_cursor++;
            print_symbol(symbol);
        }
    }

    free(buffer);
    return 0;
}