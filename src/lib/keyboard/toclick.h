#ifndef TOCLICK_FILE_H
#define TOCLICK_FILE_H

#include <stdint.h>
#include <stdbool.h>

void init_keyboard_driver();
void to_source_click(uint64_t vector);
void to_click_symbol(char symbol);
char get_last_click();
uint8_t is_new_data_available();


#define NUL 0x00
#define NULL_CHAR 0x00

#define SOH 0x01
#define CTRL_A 0x01

#define STX 0x02
#define CTRL_B 0x02

#define ETX 0x03
#define CTRL_C 0x03

#define EOT 0x04
#define CTRL_D 0x04

#define ENQ 0x05
#define CTRL_E 0x05

#define ACK 0x06
#define CTRL_F 0x06

#define BEL 0x07
#define CTRL_G 0x07

#define BS 0x08
#define BACKSPACE 0x08
#define CTRL_H 0x08

#define TAB 0x09
#define HT 0x09
#define CTRL_I 0x09

#define LF 0x0A
#define NL 0x0A
#define CTRL_J 0x0A

#define VT 0x0B
#define CTRL_K 0x0B

#define FF 0x0C
#define CTRL_L 0x0C

#define CR 0x0D
#define CTRL_M 0x0D

#define SO 0x0E
#define CTRL_N 0x0E

#define SI 0x0F
#define CTRL_O 0x0F

#define DLE 0x10
#define CTRL_P 0x10

#define DC1 0x11
#define XON 0x11
#define CTRL_Q 0x11

#define DC2 0x12
#define CTRL_R 0x12

#define DC3 0x13
#define XOFF 0x13
#define CTRL_S 0x13

#define DC4 0x14
#define CTRL_T 0x14

#define NAK 0x15
#define CTRL_U 0x15

#define SYN 0x16
#define CTRL_V 0x16

#define ETB 0x17
#define CTRL_W 0x17

#define CAN 0x18
#define CTRL_X 0x18

#define EM 0x19
#define CTRL_Y 0x19

#define SUB 0x1A
#define CTRL_Z 0x1A

#define ESC 0x1B
#define CTRL_LBRACKET 0x1B

#define FS 0x1C
#define CTRL_BACKSLASH 0x1C

#define GS 0x1D
#define CTRL_RBRACKET 0x1D

#define RS 0x1E
#define CTRL_CARET 0x1E

#define US 0x1F
#define CTRL_UNDERSCORE 0x1F

#define DEL 0x7F
#define DELETE_ASCII 0x7F

#define SPACE 0x20
#define EXCLAMATION 0x21
#define DOUBLE_QUOTE 0x22
#define HASH 0x23
#define DOLLAR 0x24
#define PERCENT 0x25
#define AMPERSAND 0x26
#define SINGLE_QUOTE 0x27
#define LEFT_PAREN 0x28
#define RIGHT_PAREN 0x29
#define ASTERISK 0x2A
#define PLUS 0x2B
#define COMMA 0x2C
#define MINUS 0x2D
#define PERIOD 0x2E
#define SLASH 0x2F

#define DIGIT_0 0x30
#define DIGIT_1 0x31
#define DIGIT_2 0x32
#define DIGIT_3 0x33
#define DIGIT_4 0x34
#define DIGIT_5 0x35
#define DIGIT_6 0x36
#define DIGIT_7 0x37
#define DIGIT_8 0x38
#define DIGIT_9 0x39

#define COLON 0x3A
#define SEMICOLON 0x3B
#define LESS_THAN 0x3C
#define EQUALS 0x3D
#define GREATER_THAN 0x3E
#define QUESTION 0x3F
#define AT_SIGN 0x40

#define UPPER_A 0x41
#define UPPER_B 0x42
#define UPPER_C 0x43
#define UPPER_D 0x44
#define UPPER_E 0x45
#define UPPER_F 0x46
#define UPPER_G 0x47
#define UPPER_H 0x48
#define UPPER_I 0x49
#define UPPER_J 0x4A
#define UPPER_K 0x4B
#define UPPER_L 0x4C
#define UPPER_M 0x4D
#define UPPER_N 0x4E
#define UPPER_O 0x4F
#define UPPER_P 0x50
#define UPPER_Q 0x51
#define UPPER_R 0x52
#define UPPER_S 0x53
#define UPPER_T 0x54
#define UPPER_U 0x55
#define UPPER_V 0x56
#define UPPER_W 0x57
#define UPPER_X 0x58
#define UPPER_Y 0x59
#define UPPER_Z 0x5A

#define LEFT_SQUARE_BRACKET 0x5B
#define BACKSLASH 0x5C
#define RIGHT_SQUARE_BRACKET 0x5D
#define CARET 0x5E
#define UNDERSCORE 0x5F
#define GRAVE 0x60

#define LOWER_A 0x61
#define LOWER_B 0x62
#define LOWER_C 0x63
#define LOWER_D 0x64
#define LOWER_E 0x65
#define LOWER_F 0x66
#define LOWER_G 0x67
#define LOWER_H 0x68
#define LOWER_I 0x69
#define LOWER_J 0x6A
#define LOWER_K 0x6B
#define LOWER_L 0x6C
#define LOWER_M 0x6D
#define LOWER_N 0x6E
#define LOWER_O 0x6F
#define LOWER_P 0x70
#define LOWER_Q 0x71
#define LOWER_R 0x72
#define LOWER_S 0x73
#define LOWER_T 0x74
#define LOWER_U 0x75
#define LOWER_V 0x76
#define LOWER_W 0x77
#define LOWER_X 0x78
#define LOWER_Y 0x79
#define LOWER_Z 0x7A

#define LEFT_BRACE 0x7B
#define VERTICAL_BAR 0x7C
#define RIGHT_BRACE 0x7D
#define TILDE 0x7E

#define ESC_MAKE 0x01
#define ESC_BREAK 0x81

#define ONE_MAKE 0x02
#define ONE_BREAK 0x82

#define TWO_MAKE 0x03
#define TWO_BREAK 0x83

#define THREE_MAKE 0x04
#define THREE_BREAK 0x84

#define FOUR_MAKE 0x05
#define FOUR_BREAK 0x85

#define FIVE_MAKE 0x06
#define FIVE_BREAK 0x86

#define SIX_MAKE 0x07
#define SIX_BREAK 0x87

#define SEVEN_MAKE 0x08
#define SEVEN_BREAK 0x88

#define EIGHT_MAKE 0x09
#define EIGHT_BREAK 0x89

#define NINE_MAKE 0x0A
#define NINE_BREAK 0x8A

#define ZERO_MAKE 0x0B
#define ZERO_BREAK 0x8B

#define MINUS_MAKE 0x0C
#define MINUS_BREAK 0x8C

#define EQUALS_MAKE 0x0D
#define EQUALS_BREAK 0x8D

#define BACKSPACE_MAKE 0x0E
#define BACKSPACE_BREAK 0x8E

#define TAB_MAKE 0x0F
#define TAB_BREAK 0x8F

#define Q_MAKE 0x10
#define Q_BREAK 0x90

#define W_MAKE 0x11
#define W_BREAK 0x91

#define E_MAKE 0x12
#define E_BREAK 0x92

#define R_MAKE 0x13
#define R_BREAK 0x93

#define T_MAKE 0x14
#define T_BREAK 0x94

#define Y_MAKE 0x15
#define Y_BREAK 0x95

#define U_MAKE 0x16
#define U_BREAK 0x96

#define I_MAKE 0x17
#define I_BREAK 0x97

#define O_MAKE 0x18
#define O_BREAK 0x98

#define P_MAKE 0x19
#define P_BREAK 0x99

#define LEFT_SQUARE_BRACKET_MAKE 0x1A
#define LEFT_SQUARE_BRACKET_BREAK 0x9A

#define RIGHT_SQUARE_BRACKET_MAKE 0x1B
#define RIGHT_SQUARE_BRACKET_BREAK 0x9B

#define ENTER_MAKE 0x1C
#define ENTER_BREAK 0x9C

#define LEFT_CTRL_MAKE 0x1D
#define LEFT_CTRL_BREAK 0x9D

#define A_MAKE 0x1E
#define A_BREAK 0x9E

#define S_MAKE 0x1F
#define S_BREAK 0x9F

#define D_MAKE 0x20
#define D_BREAK 0xA0

#define F_MAKE 0x21
#define F_BREAK 0xA1

#define G_MAKE 0x22
#define G_BREAK 0xA2

#define H_MAKE 0x23
#define H_BREAK 0xA3

#define J_MAKE 0x24
#define J_BREAK 0xA4

#define K_MAKE 0x25
#define K_BREAK 0xA5

#define L_MAKE 0x26
#define L_BREAK 0xA6

#define SEMICOLON_MAKE 0x27
#define SEMICOLON_BREAK 0xA7

#define APOSTROPHE_MAKE 0x28
#define APOSTROPHE_BREAK 0xA8

#define GRAVE_MAKE 0x29
#define GRAVE_BREAK 0xA9

#define LEFT_SHIFT_MAKE 0x2A
#define LEFT_SHIFT_BREAK 0xAA

#define BACKSLASH_MAKE 0x2B
#define BACKSLASH_BREAK 0xAB

#define Z_MAKE 0x2C
#define Z_BREAK 0xAC

#define X_MAKE 0x2D
#define X_BREAK 0xAD

#define C_MAKE 0x2E
#define C_BREAK 0xAE

#define V_MAKE 0x2F
#define V_BREAK 0xAF

#define B_MAKE 0x30
#define B_BREAK 0xB0

#define N_MAKE 0x31
#define N_BREAK 0xB1

#define M_MAKE 0x32
#define M_BREAK 0xB2

#define COMMA_MAKE 0x33
#define COMMA_BREAK 0xB3

#define PERIOD_MAKE 0x34
#define PERIOD_BREAK 0xB4

#define SLASH_MAKE 0x35
#define SLASH_BREAK 0xB5

#define RIGHT_SHIFT_MAKE 0x36
#define RIGHT_SHIFT_BREAK 0xB6

#define NUM_PAD_ASTERISK_MAKE 0x37
#define NUM_PAD_ASTERISK_BREAK 0xB7

#define LEFT_ALT_MAKE 0x38
#define LEFT_ALT_BREAK 0xB8

#define SPACE_MAKE 0x39
#define SPACE_BREAK 0xB9

#define CAPS_LOCK_MAKE 0x3A
#define CAPS_LOCK_BREAK 0xBA

#define F1_MAKE 0x3B
#define F1_BREAK 0xBB

#define F2_MAKE 0x3C
#define F2_BREAK 0xBC

#define F3_MAKE 0x3D
#define F3_BREAK 0xBD

#define F4_MAKE 0x3E
#define F4_BREAK 0xBE

#define F5_MAKE 0x3F
#define F5_BREAK 0xBF

#define F6_MAKE 0x40
#define F6_BREAK 0xC0

#define F7_MAKE 0x41
#define F7_BREAK 0xC1

#define F8_MAKE 0x42
#define F8_BREAK 0xC2

#define F9_MAKE 0x43
#define F9_BREAK 0xC3

#define F10_MAKE 0x44
#define F10_BREAK 0xC4

#define NUM_LOCK_MAKE 0x45
#define NUM_LOCK_BREAK 0xC5

#define SCROLL_LOCK_MAKE 0x46
#define SCROLL_LOCK_BREAK 0xC6

#define NUM_PAD_7_HOME_MAKE 0x47
#define NUM_PAD_7_HOME_BREAK 0xC7

#define NUM_PAD_8_UP_MAKE 0x48
#define NUM_PAD_8_UP_BREAK 0xC8

#define NUM_PAD_9_PAGE_UP_MAKE 0x49
#define NUM_PAD_9_PAGE_UP_BREAK 0xC9

#define NUM_PAD_MINUS_MAKE 0x4A
#define NUM_PAD_MINUS_BREAK 0xCA

#define NUM_PAD_4_LEFT_MAKE 0x4B
#define NUM_PAD_4_LEFT_BREAK 0xCB

#define NUM_PAD_5_CENTER_MAKE 0x4C
#define NUM_PAD_5_CENTER_BREAK 0xCC

#define NUM_PAD_6_RIGHT_MAKE 0x4D
#define NUM_PAD_6_RIGHT_BREAK 0xCD

#define NUM_PAD_PLUS_MAKE 0x4E
#define NUM_PAD_PLUS_BREAK 0xCE

#define NUM_PAD_1_END_MAKE 0x4F
#define NUM_PAD_1_END_BREAK 0xCF

#define NUM_PAD_2_DOWN_MAKE 0x50
#define NUM_PAD_2_DOWN_BREAK 0xD0

#define NUM_PAD_3_PAGE_DOWN_MAKE 0x51
#define NUM_PAD_3_PAGE_DOWN_BREAK 0xD1

#define NUM_PAD_0_INSERT_MAKE 0x52
#define NUM_PAD_0_INSERT_BREAK 0xD2

#define NUM_PAD_DELETE_MAKE 0x53
#define NUM_PAD_DELETE_BREAK 0xD3

#define F11_MAKE 0x57
#define F11_BREAK 0xD7

#define F12_MAKE 0x58
#define F12_BREAK 0xD8

#define RIGHT_CTRL_MAKE 0xE0
#define RIGHT_CTRL_MAKE2 0x1D
#define RIGHT_CTRL_BREAK 0xE0
#define RIGHT_CTRL_BREAK2 0x9D

#define RIGHT_ALT_MAKE 0xE0
#define RIGHT_ALT_MAKE2 0x38
#define RIGHT_ALT_BREAK 0xE0
#define RIGHT_ALT_BREAK2 0xB8

#define LEFT_WIN_MAKE 0xE0
#define LEFT_WIN_MAKE2 0x5B
#define LEFT_WIN_BREAK 0xE0
#define LEFT_WIN_BREAK2 0xDB

#define RIGHT_WIN_MAKE 0xE0
#define RIGHT_WIN_MAKE2 0x5C
#define RIGHT_WIN_BREAK 0xE0
#define RIGHT_WIN_BREAK2 0xDC

#define MENU_MAKE 0xE0
#define MENU_MAKE2 0x5D
#define MENU_BREAK 0xE0
#define MENU_BREAK2 0xDD

#define INSERT_MAKE 0xE0
#define INSERT_MAKE2 0x52
#define INSERT_BREAK 0xE0
#define INSERT_BREAK2 0xD2

#define DELETE_MAKE 0xE0
#define DELETE_MAKE2 0x53
#define DELETE_BREAK 0xE0
#define DELETE_BREAK2 0xD3

#define HOME_MAKE 0xE0
#define HOME_MAKE2 0x47
#define HOME_BREAK 0xE0
#define HOME_BREAK2 0xC7

#define END_MAKE 0xE0
#define END_MAKE2 0x4F
#define END_BREAK 0xE0
#define END_BREAK2 0xCF

#define PAGE_UP_MAKE 0xE0
#define PAGE_UP_MAKE2 0x49
#define PAGE_UP_BREAK 0xE0
#define PAGE_UP_BREAK2 0xC9

#define PAGE_DOWN_MAKE 0xE0
#define PAGE_DOWN_MAKE2 0x51
#define PAGE_DOWN_BREAK 0xE0
#define PAGE_DOWN_BREAK2 0xD1

#define UP_ARROW_MAKE 0xE0
#define UP_ARROW_MAKE2 0x48
#define UP_ARROW_BREAK 0xE0
#define UP_ARROW_BREAK2 0xC8

#define DOWN_ARROW_MAKE 0xE0
#define DOWN_ARROW_MAKE2 0x50
#define DOWN_ARROW_BREAK 0xE0
#define DOWN_ARROW_BREAK2 0xD0

#define LEFT_ARROW_MAKE 0xE0
#define LEFT_ARROW_MAKE2 0x4B
#define LEFT_ARROW_BREAK 0xE0
#define LEFT_ARROW_BREAK2 0xCB

#define RIGHT_ARROW_MAKE 0xE0
#define RIGHT_ARROW_MAKE2 0x4D
#define RIGHT_ARROW_BREAK 0xE0
#define RIGHT_ARROW_BREAK2 0xCD

#define NUM_PAD_ENTER_MAKE 0xE0
#define NUM_PAD_ENTER_MAKE2 0x1C
#define NUM_PAD_ENTER_BREAK 0xE0
#define NUM_PAD_ENTER_BREAK2 0x9C

#define NUM_PAD_SLASH_MAKE 0xE0
#define NUM_PAD_SLASH_MAKE2 0x35
#define NUM_PAD_SLASH_BREAK 0xE0
#define NUM_PAD_SLASH_BREAK2 0xB5

#define PRINT_SCREEN_MAKE 0xE0
#define PRINT_SCREEN_MAKE2 0x2A
#define PRINT_SCREEN_MAKE3 0xE0
#define PRINT_SCREEN_MAKE4 0x37
#define PRINT_SCREEN_BREAK 0xE0
#define PRINT_SCREEN_BREAK2 0xB7
#define PRINT_SCREEN_BREAK3 0xE0
#define PRINT_SCREEN_BREAK4 0xAA

#define PAUSE_MAKE 0xE1
#define PAUSE_MAKE2 0x1D
#define PAUSE_MAKE3 0x45
#define PAUSE_MAKE4 0xE1
#define PAUSE_MAKE5 0x9D
#define PAUSE_MAKE6 0xC5

#define ACPI_POWER_MAKE 0xE0
#define ACPI_POWER_MAKE2 0x5E
#define ACPI_POWER_BREAK 0xE0
#define ACPI_POWER_BREAK2 0xDE

#define ACPI_SLEEP_MAKE 0xE0
#define ACPI_SLEEP_MAKE2 0x5F
#define ACPI_SLEEP_BREAK 0xE0
#define ACPI_SLEEP_BREAK2 0xDF

#define ACPI_WAKE_MAKE 0xE0
#define ACPI_WAKE_MAKE2 0x63
#define ACPI_WAKE_BREAK 0xE0
#define ACPI_WAKE_BREAK2 0xE3

#define NEXT_TRACK_MAKE 0xE0
#define NEXT_TRACK_MAKE2 0x19
#define NEXT_TRACK_BREAK 0xE0
#define NEXT_TRACK_BREAK2 0x99

#define PREVIOUS_TRACK_MAKE 0xE0
#define PREVIOUS_TRACK_MAKE2 0x10
#define PREVIOUS_TRACK_BREAK 0xE0
#define PREVIOUS_TRACK_BREAK2 0x90

#define STOP_MAKE 0xE0
#define STOP_MAKE2 0x24
#define STOP_BREAK 0xE0
#define STOP_BREAK2 0xA4

#define PLAY_PAUSE_MAKE 0xE0
#define PLAY_PAUSE_MAKE2 0x22
#define PLAY_PAUSE_BREAK 0xE0
#define PLAY_PAUSE_BREAK2 0xA2

#define MUTE_MAKE 0xE0
#define MUTE_MAKE2 0x20
#define MUTE_BREAK 0xE0
#define MUTE_BREAK2 0xA0

#define VOLUME_UP_MAKE 0xE0
#define VOLUME_UP_MAKE2 0x30
#define VOLUME_UP_BREAK 0xE0
#define VOLUME_UP_BREAK2 0xB0

#define VOLUME_DOWN_MAKE 0xE0
#define VOLUME_DOWN_MAKE2 0x2E
#define VOLUME_DOWN_BREAK 0xE0
#define VOLUME_DOWN_BREAK2 0xAE

#define MEDIA_SELECT_MAKE 0xE0
#define MEDIA_SELECT_MAKE2 0x6D
#define MEDIA_SELECT_BREAK 0xE0
#define MEDIA_SELECT_BREAK2 0xED

#define EMAIL_MAKE 0xE0
#define EMAIL_MAKE2 0x6C
#define EMAIL_BREAK 0xE0
#define EMAIL_BREAK2 0xEC

#define CALCULATOR_MAKE 0xE0
#define CALCULATOR_MAKE2 0x21
#define CALCULATOR_BREAK 0xE0
#define CALCULATOR_BREAK2 0xA1

#define MY_COMPUTER_MAKE 0xE0
#define MY_COMPUTER_MAKE2 0x6B
#define MY_COMPUTER_BREAK 0xE0
#define MY_COMPUTER_BREAK2 0xEB

#define WWW_SEARCH_MAKE 0xE0
#define WWW_SEARCH_MAKE2 0x65
#define WWW_SEARCH_BREAK 0xE0
#define WWW_SEARCH_BREAK2 0xE5

#define WWW_HOME_MAKE 0xE0
#define WWW_HOME_MAKE2 0x32
#define WWW_HOME_BREAK 0xE0
#define WWW_HOME_BREAK2 0xB2

#define WWW_BACK_MAKE 0xE0
#define WWW_BACK_MAKE2 0x6A
#define WWW_BACK_BREAK 0xE0
#define WWW_BACK_BREAK2 0xEA

#define WWW_FORWARD_MAKE 0xE0
#define WWW_FORWARD_MAKE2 0x69
#define WWW_FORWARD_BREAK 0xE0
#define WWW_FORWARD_BREAK2 0xE9

#define WWW_STOP_MAKE 0xE0
#define WWW_STOP_MAKE2 0x68
#define WWW_STOP_BREAK 0xE0
#define WWW_STOP_BREAK2 0xE8

#define WWW_REFRESH_MAKE 0xE0
#define WWW_REFRESH_MAKE2 0x67
#define WWW_REFRESH_BREAK 0xE0
#define WWW_REFRESH_BREAK2 0xE7

#define WWW_FAVORITES_MAKE 0xE0
#define WWW_FAVORITES_MAKE2 0x66
#define WWW_FAVORITES_BREAK 0xE0
#define WWW_FAVORITES_BREAK2 0xE6

#endif