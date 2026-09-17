#include "toclick.h"
#include <stdint.h>

#define BUFFER_SIZE 256

volatile char buffer[BUFFER_SIZE];
volatile unsigned int cursor_position = 0;
volatile uint8_t new_data_available = 0;

volatile uint8_t shift_pressed = 0;
volatile uint8_t caps_lock = 0;

void to_zero_buffer() {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = 0;
    }
}

void init_keyboard_driver() {
    to_zero_buffer();
    shift_pressed = 0;
    caps_lock = 0;
    cursor_position = 0;
    new_data_available = 0;
}

void write_to_fifo(char symbol) {
    buffer[cursor_position] = symbol;
    cursor_position++;
    if (cursor_position >= BUFFER_SIZE) {
        cursor_position = 0;
    }
    new_data_available = 1;
}

char get_last_click() {
    new_data_available = 0;
    if (cursor_position == 0) {
        return buffer[BUFFER_SIZE - 1];
    }
    return buffer[cursor_position - 1];
}

uint8_t is_new_data_available() {
    return new_data_available;
}

void to_click_symbol(char symbol) {
    write_to_fifo(symbol);
}

void to_source_click(uint64_t vector) {
    uint8_t scancode = (uint8_t)(vector & 0xFF);
    
    switch(scancode) {
        case LEFT_SHIFT_MAKE:
        case RIGHT_SHIFT_MAKE:
            shift_pressed = 1;
            return;
            
        case LEFT_SHIFT_BREAK:
        case RIGHT_SHIFT_BREAK:
            shift_pressed = 0;
            return;
            
        case CAPS_LOCK_MAKE:
            caps_lock ^= 1;
            return;
    }
    
    switch(scancode) {
        case ESC_MAKE: to_click_symbol(ESC); break;
        
        case ONE_MAKE: to_click_symbol(shift_pressed ? '!' : '1'); break;
        case TWO_MAKE: to_click_symbol(shift_pressed ? '@' : '2'); break;
        case THREE_MAKE: to_click_symbol(shift_pressed ? '#' : '3'); break;
        case FOUR_MAKE: to_click_symbol(shift_pressed ? '$' : '4'); break;
        case FIVE_MAKE: to_click_symbol(shift_pressed ? '%' : '5'); break;
        case SIX_MAKE: to_click_symbol(shift_pressed ? '^' : '6'); break;
        case SEVEN_MAKE: to_click_symbol(shift_pressed ? '&' : '7'); break;
        case EIGHT_MAKE: to_click_symbol(shift_pressed ? '*' : '8'); break;
        case NINE_MAKE: to_click_symbol(shift_pressed ? '(' : '9'); break;
        case ZERO_MAKE: to_click_symbol(shift_pressed ? ')' : '0'); break;
        
        case MINUS_MAKE: to_click_symbol(shift_pressed ? '_' : '-'); break;
        case EQUALS_MAKE: to_click_symbol(shift_pressed ? '+' : '='); break;
        case BACKSPACE_MAKE: to_click_symbol('\b'); break;
        case TAB_MAKE: to_click_symbol('\t'); break;
        
        case Q_MAKE: to_click_symbol((caps_lock ^ shift_pressed) ? 'Q' : 'q'); break;
        case W_MAKE: to_click_symbol((caps_lock ^ shift_pressed) ? 'W' : 'w'); break;
        case E_MAKE: to_click_symbol((caps_lock ^ shift_pressed) ? 'E' : 'e'); break;
        case R_MAKE: to_click_symbol((caps_lock ^ shift_pressed) ? 'R' : 'r'); break;
        case T_MAKE: to_click_symbol((caps_lock ^ shift_pressed) ? 'T' : 't'); break;
        case Y_MAKE: to_click_symbol((caps_lock ^ shift_pressed) ? 'Y' : 'y'); break;
        case U_MAKE: to_click_symbol((caps_lock ^ shift_pressed) ? 'U' : 'u'); break;
        case I_MAKE: to_click_symbol((caps_lock ^ shift_pressed) ? 'I' : 'i'); break;
        case O_MAKE: to_click_symbol((caps_lock ^ shift_pressed) ? 'O' : 'o'); break;
        case P_MAKE: to_click_symbol((caps_lock ^ shift_pressed) ? 'P' : 'p'); break;
        
        case LEFT_SQUARE_BRACKET_MAKE: to_click_symbol(shift_pressed ? '{' : '['); break;
        case RIGHT_SQUARE_BRACKET_MAKE: to_click_symbol(shift_pressed ? '}' : ']'); break;
        case ENTER_MAKE: to_click_symbol('\n'); break;
        
        case A_MAKE: to_click_symbol((caps_lock ^ shift_pressed) ? 'A' : 'a'); break;
        case S_MAKE: to_click_symbol((caps_lock ^ shift_pressed) ? 'S' : 's'); break;
        case D_MAKE: to_click_symbol((caps_lock ^ shift_pressed) ? 'D' : 'd'); break;
        case F_MAKE: to_click_symbol((caps_lock ^ shift_pressed) ? 'F' : 'f'); break;
        case G_MAKE: to_click_symbol((caps_lock ^ shift_pressed) ? 'G' : 'g'); break;
        case H_MAKE: to_click_symbol((caps_lock ^ shift_pressed) ? 'H' : 'h'); break;
        case J_MAKE: to_click_symbol((caps_lock ^ shift_pressed) ? 'J' : 'j'); break;
        case K_MAKE: to_click_symbol((caps_lock ^ shift_pressed) ? 'K' : 'k'); break;
        case L_MAKE: to_click_symbol((caps_lock ^ shift_pressed) ? 'L' : 'l'); break;
        
        case SEMICOLON_MAKE: to_click_symbol(shift_pressed ? ':' : ';'); break;
        case APOSTROPHE_MAKE: to_click_symbol(shift_pressed ? '"' : '\''); break;
        case GRAVE_MAKE: to_click_symbol(shift_pressed ? '~' : '`'); break;
        case BACKSLASH_MAKE: to_click_symbol(shift_pressed ? '|' : '\\'); break;
        
        case Z_MAKE: to_click_symbol((caps_lock ^ shift_pressed) ? 'Z' : 'z'); break;
        case X_MAKE: to_click_symbol((caps_lock ^ shift_pressed) ? 'X' : 'x'); break;
        case C_MAKE: to_click_symbol((caps_lock ^ shift_pressed) ? 'C' : 'c'); break;
        case V_MAKE: to_click_symbol((caps_lock ^ shift_pressed) ? 'V' : 'v'); break;
        case B_MAKE: to_click_symbol((caps_lock ^ shift_pressed) ? 'B' : 'b'); break;
        case N_MAKE: to_click_symbol((caps_lock ^ shift_pressed) ? 'N' : 'n'); break;
        case M_MAKE: to_click_symbol((caps_lock ^ shift_pressed) ? 'M' : 'm'); break;
        
        case COMMA_MAKE: to_click_symbol(shift_pressed ? '<' : ','); break;
        case PERIOD_MAKE: to_click_symbol(shift_pressed ? '>' : '.'); break;
        case SLASH_MAKE: to_click_symbol(shift_pressed ? '?' : '/'); break;
        case SPACE_MAKE: to_click_symbol(' '); break;
        
        case NUM_PAD_7_HOME_MAKE: to_click_symbol('7'); break;
        case NUM_PAD_8_UP_MAKE: to_click_symbol('8'); break;
        case NUM_PAD_9_PAGE_UP_MAKE: to_click_symbol('9'); break;
        case NUM_PAD_MINUS_MAKE: to_click_symbol('-'); break;
        case NUM_PAD_4_LEFT_MAKE: to_click_symbol('4'); break;
        case NUM_PAD_5_CENTER_MAKE: to_click_symbol('5'); break;
        case NUM_PAD_6_RIGHT_MAKE: to_click_symbol('6'); break;
        case NUM_PAD_PLUS_MAKE: to_click_symbol('+'); break;
        case NUM_PAD_1_END_MAKE: to_click_symbol('1'); break;
        case NUM_PAD_2_DOWN_MAKE: to_click_symbol('2'); break;
        case NUM_PAD_3_PAGE_DOWN_MAKE: to_click_symbol('3'); break;
        case NUM_PAD_0_INSERT_MAKE: to_click_symbol('0'); break;
        case NUM_PAD_DELETE_MAKE: to_click_symbol('.'); break;
        
        default: break;
    }
}