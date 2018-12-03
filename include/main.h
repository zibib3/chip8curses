#define _DEBUG

#ifndef __MAIN_H__
#define __MAIN_H__
#include "log.h"
#include "types.h"
#include "interpreter.h"

bool read_rom(const char * rom_path);
void redraw_game_screen();
void print_registers();
void repaint();
int get();

extern const int keyboard[];

extern int pressed_key;
extern memory_t memory;
extern unsigned char v[];
extern unsigned char delay_timer;
extern uint16_t l; // address register.
extern uint16_t pc; // eip register.
extern uint16_t stack[];
extern unsigned int current_stack;
extern WINDOW * log_window;
extern WINDOW * memory_window;
extern WINDOW * display_window;
extern WINDOW * registers_window;

#endif