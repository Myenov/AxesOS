#ifndef VIDEO_H
#define VIDEO_H

#define VIDEO_MEMORY ((volatile unsigned short*)0xB8000)
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define DEFAULT_COLOR 0x0F
#define DEFAULT_FON_COLOR 0x0


#define COLOR_BLACK 0x0
#define COLOR_BLUE 0x1
#define COLOR_GREEN 0x2
#define COLOR_CYAN 0x3
#define COLOR_RED 0x4
#define COLOR_MAGENTA 0x5
#define COLOR_BROWM 0x6
#define COLOR_LIGHT_GRAY 0x7
#define COLOR_DARK_GRAY 0x8
#define COLOR_LIGHT_BLUE 0x9
#define COLOR_LIGHT_GREEN 0xA
#define COLOR_LIGHT_CYAN 0xB
#define COLOR_LIGHT_RED 0xC
#define COLOR_LIGHT_MAGENTA 0xD
#define COLOR_YELLOW 0xE
#define COLOR_WHITE 0xF



int get_buffer_len(const char* buffer);

void print(const char* message);
void print_symbol(char symbol);
void print_string(const char* message, const unsigned char color, int len);
void clear_screen();

void set_default_color(unsigned char color);
void set_default_fon_color(unsigned char color);

void del_symbol_current_pos();
void to_backspace();
#endif