#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <ncurses.h>

#include "main.h"

int pressed_key = ERR;
memory_t memory = {0};
unsigned char v[16] = {0}; // registers
unsigned char delay_timer = 0;
unsigned char sound_timer = 0;
uint16_t l = 0; // address register.
uint16_t pc = 0x200;
uint16_t stack[16] = {0};
unsigned int current_stack = 0;
WINDOW * log_window = NULL;
WINDOW * memory_window = NULL;
WINDOW * game_screen_window = NULL;
WINDOW * registers_window = NULL;

const int keyboard[] = {'2', '3', '4', '5',
						'w', 'e', 'r', 't',
						's', 'd', 'f', 'g',
						'x', 'c', 'v', 'b'};
