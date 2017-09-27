

// for ANSI terminal

#ifndef _ANSI_TERMINAL_
#define _ANSI_TERMINAL_

// for basic
#define ANSI_PREFIX "\033["
#define M_POSTFIX "m"

// basic color
#define COLOR_BLACK     "0"
#define COLOR_RED       "1"
#define COLOR_GREEN     "2"
#define COLOR_YELLOW    "3"
#define COLOR_BLUE      "4"
#define COLOR_PURPLE    "5"
#define COLOR_DRAKGREEN "6"
#define COLOR_WHITE     "7"
#define COLOR_DEFAULT   "9"

// set color
#define FONT_COLOR(x) ANSI_PREFIX "3" x M_POSTFIX
#define BACK_COLOR(x) ANSI_PREFIX "4" x M_POSTFIX
#define FB_COLOR(x,y) ANSI_PREFIX "4" x ";3" y M_POSTFIX

// control
#define CLEAN_ALL_ATTR ANSI_PREFIX "0" M_POSTFIX
#define HIGHLIGHT      ANSI_PREFIX "1" M_POSTFIX
#define UNDERLINE      ANSI_PREFIX "4" M_POSTFIX
#define BLINK          ANSI_PREFIX "5" M_POSTFIX
#define NEGATIVE       ANSI_PREFIX "7" M_POSTFIX

// operation
#define MOVE_CUR_UP(x) ANSI_PREFIX x "A"
#define MOVE_CUR_DOWN(x) ANSI_PREFIX x "B"
#define MOVE_CUR_FORWARD(x) ANSI_PREFIX x "C"
#define MOVE_CUR_BACK(x) ANSI_PREFIX x "D"
#define MOVE_LINE_BEG_DOWN(x) ANSI_PREFIX x "E"
#define MOVE_LINE_BEG_UP(x) ANSI_PREFIX x "F"
#define MOVE_CUR_COL(x) ANSI_PREFIX x "G"
#define MOVE_CUR_POS(x,y) ANSI_PREFIX x ";" y "H"
#define CLEAN_SCN(x) ANSI_PREFIX x "J"
#define ERASE_LINE(x) ANSI_PREFIX x "K"
#define HIDE_CUR ANSI_PREFIX "?25l"
#define HIDE_CUR ANSI_PREFIX "?25h"


#endif // _ANSI_TERMINAL_
