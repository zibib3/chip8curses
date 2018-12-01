#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <ncurses.h>

#include "main.h"

int pressed_key = 0;
memory_t memory = {0};
unsigned char v[16] = {0}; // registers
unsigned char delay_timer = 0;
uint16_t l = 0; // address register.
uint16_t pc = 0x200;
uint16_t stack[16] = {0};
unsigned int current_stack = 0;
WINDOW * log_window = NULL;
WINDOW * memory_window = NULL;
WINDOW * display_window = NULL;
WINDOW * registers_window = NULL;

chtype get_colored_memory(size_t address)
{
	if (pc - 0x200 == address || pc - 0x200 == address-1)
	{
		wattrset(memory_window, COLOR_PAIR(1));
	}
	else
	{
		if (pc - 0x200 == address -2)
		{
			wattrset(memory_window, COLOR_PAIR(0));
		}
	}
	return memory.program[address];
}

void repaint_memory()
{
	wmove(memory_window, 1, 1);
	for (size_t i = 0; i < sizeof(memory.program) / 16; ++i)
	{
		// I can't use colors with mvprintw arguments...
/*		mvwprintw(memory_window, 1 + i, 1, "0x%X\t%02X%02X %02X%02X  %02X%02X %02X%02X    %02X%02X %02X%02X  %02X%02X %02X%02X", (i*16)+sizeof(memory.start), 	get_colored_memory(i*16), get_colored_memory(i*16+1), get_colored_memory(i*16+2), get_colored_memory(i*16+3),
																															get_colored_memory(i*16+4), get_colored_memory(i*16+5), get_colored_memory(i*16+6), get_colored_memory(i*16+7),
																															get_colored_memory(i*16+8), get_colored_memory(i*16+9), get_colored_memory(i*16+10), get_colored_memory(i*16+11),
																															get_colored_memory(i*16+12), get_colored_memory(i*16+13), get_colored_memory(i*16+14), get_colored_memory(i*16+15));*/
	mvwprintw(memory_window, 1 + i, 1, "0x%X\t", (i*16)+sizeof(memory.start));
	wprintw(memory_window, "%02X", get_colored_memory(i*16));
	wprintw(memory_window, "%02X ", get_colored_memory(i*16+1));
	wprintw(memory_window, "%02X", get_colored_memory(i*16+2));
	wprintw(memory_window, "%02X ", get_colored_memory(i*16+3));
	wprintw(memory_window, "%02X", get_colored_memory(i*16+4));
	wprintw(memory_window, "%02X ", get_colored_memory(i*16+5));
	wprintw(memory_window, "%02X", get_colored_memory(i*16+6));
	wprintw(memory_window, "%02X\t", get_colored_memory(i*16+7));
	wprintw(memory_window, "%02X", get_colored_memory(i*16+8));
	wprintw(memory_window, "%02X ", get_colored_memory(i*16+9));
	wprintw(memory_window, "%02X", get_colored_memory(i*16+10));
	wprintw(memory_window, "%02X ", get_colored_memory(i*16+11));
	wprintw(memory_window, "%02X", get_colored_memory(i*16+12));
	wprintw(memory_window, "%02X ", get_colored_memory(i*16+13));
	wprintw(memory_window, "%02X", get_colored_memory(i*16+14));
	wprintw(memory_window, "%02X", get_colored_memory(i*16+15));
	}
}

int main(int argc, char const *argv[])
{
	struct timespec delay = {0, 1000000000 / 60};

	initscr();
	start_color();
	init_pair(1, COLOR_RED, COLOR_YELLOW);
	/*printf("sizeof(memory.screen) = %u\n", sizeof(memory.screen));
	printf("sizeof(memory.screen[0]) = %u\n", sizeof(memory.screen[0]));
	printf("sizeof(memory.screen[1]) = %u\n", sizeof(memory.screen[1]));
	printf("&memory.screen[0] = %p\n", &memory.screen[0]);
	printf("&memory.screen[1] = %p\n", &memory.screen[1]);
	printf("&memory.screen[0][0] = %p\n", &memory.screen[0][0]);
	printf("&memory.screen[0][1] = %p\n", &memory.screen[0][1]);*/
	// return 0;
	if (argc != 2)
	{
		endwin();
		return -1;
	}
	for (size_t i = 0; i < sizeof(memory.screen); ++i)
	{
		((char *)(memory.screen))[i] = i;
	}
	if (!read_rom(argv[1]))
	{
		return -2;
	}

	display_window = newwin(32+2,64+2,2,10);
	log_window = newwin(10,100,37,10);
	registers_window = newwin(10,70,2,90);
	memory_window = newwin(40,70,12,90);
	box(memory_window, '|', '=');


	raw();
	keypad(stdscr, TRUE);
	noecho();
	scrollok(log_window, TRUE);
	while (true)
	{
		if (delay_timer > 0)
		{
			delay_timer--;
		}
		repaint_memory();
		redraw_game_screen();
		print_registers();
		redraw_game_screen();
		read_next_command();
		nanosleep(&delay, NULL);
		repaint();
		pressed_key  = get();
	}

	return 0;
}