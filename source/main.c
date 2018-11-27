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

void repaint_memory()
{
	wmove(memory_window, 1, 1);
	for (size_t i = 0; i < sizeof(memory.program) / 16; ++i)
	{
		mvwprintw(memory_window, 1 + i, 1, "0x%X\t%02X%02X %02X%02X  %02X%02X %02X%02X    %02X%02X %02X%02X  %02X%02X %02X%02X", (i*16)+sizeof(memory.start), 	memory.program[i*16], memory.program[i*16+1], memory.program[i*16+2], memory.program[i*16+3],
																															memory.program[i*16+4], memory.program[i*16+5], memory.program[i*16+6], memory.program[i*16+7],
																															memory.program[i*16+8], memory.program[i*16+9], memory.program[i*16+10], memory.program[i*16+11],
																															memory.program[i*16+12], memory.program[i*16+13], memory.program[i*16+14], memory.program[i*16+15]);
	}
}

int main(int argc, char const *argv[])
{
	struct timespec delay = {0, 1000000000 / 60};

	initscr();
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