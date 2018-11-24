#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>
// #include <stdio.h>
#include <ncurses.h>

#include "main.h"
memory_t memory = {0};
unsigned char v[16] = {0}; // registers
uint16_t l = 0; // address register.
uint16_t pc = 0x200;
uint16_t stack[16] = {0};
unsigned int current_stack = 0;
WINDOW * log_window = NULL;
WINDOW * display_window = NULL;
WINDOW * registers_window = NULL;

int main(int argc, char const *argv[])
{
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

	display_window = newwin(32+2,64+2,5,10);
	log_window = newwin(10,100,40,30);
	registers_window = newwin(10,70,5,90);

	raw();
	keypad(stdscr, TRUE);
	noecho();
	nodelay(stdscr, TRUE);
	while (true)
	{
		print_the_display();
		read_next_command();
		print_registers();
		get();
	}

	getch();
	endwin();
	
	return 0;
}