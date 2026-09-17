#include "video.h"

volatile unsigned char default_color = DEFAULT_COLOR;
volatile unsigned char default_fon_color = DEFAULT_FON_COLOR;
volatile unsigned char cursor_x = 0;
volatile unsigned char cursor_y = 0;

int get_buffer_len(const char* buffer) {
    int lenght = 0;
    while (buffer[lenght] != '\0') {
        lenght++;
    }
    return lenght;
}

void print(const char *message) {
    print_string(message, default_color, get_buffer_len(message));
}

void print_symbol(char symbol) {
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    if (symbol == '\n') {
        cursor_x = 0;
        cursor_y++;
        return;
    }

    if (symbol == '\b') {
        if (cursor_x > 0) {
            cursor_x--;
        } else if (cursor_y > 0) {
            cursor_y--;
            cursor_x = VGA_WIDTH - 1;
        }
        VIDEO_MEMORY[cursor_y * VGA_WIDTH + cursor_x] = (default_fon_color << 8) | ' ';
        return;
    }

    VIDEO_MEMORY[cursor_y * VGA_WIDTH + cursor_x] = (default_color << 8) | symbol;
    cursor_x++;
}

void print_string(const char* message, const unsigned char color, int len) {
    for (short i = 0; i < len; i++) {
        if (cursor_x >= VGA_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }

        if (message[i] == '\n') {
            cursor_x = 0;
            cursor_y++;
            continue;
        }

        if (message[i] == '\b') {
            if (cursor_x > 0) {
                cursor_x--;
            } else if (cursor_y > 0) {
                cursor_y--;
                cursor_x = VGA_WIDTH - 1;
            }
            VIDEO_MEMORY[cursor_y * VGA_WIDTH + cursor_x] = (default_fon_color << 8) | ' ';
            continue;
        }

        VIDEO_MEMORY[cursor_y * VGA_WIDTH + cursor_x] = (color << 8) | message[i];
        cursor_x++;
    }
}
void set_cursor_zero() {
    cursor_y = 0;
    cursor_x = 0;
}

void clear_screen() {
    unsigned short symbol = (0x0 << 8) | ' ';
    for (short i = 0; VGA_WIDTH * VGA_HEIGHT > i; i++) {
        VIDEO_MEMORY[i] = symbol;
    }
    set_cursor_zero();
}

void set_default_color(unsigned char color) {
    default_color = color;
}

void set_default_fon_color(unsigned char color) {
    default_fon_color = color;
}

void del_symbol_current_pos() {
    VIDEO_MEMORY[cursor_y * VGA_WIDTH + cursor_x] = (default_fon_color << 8) | ' ';
    return;
}
void to_backspace() {
    if (cursor_x > 0) {
        cursor_x--;
    } else if (cursor_y > 0) {
        cursor_y--;
        cursor_x = VGA_WIDTH - 1;
    }
    VIDEO_MEMORY[cursor_y * VGA_WIDTH + cursor_x] = (default_fon_color << 8) | ' ';
    return;
}