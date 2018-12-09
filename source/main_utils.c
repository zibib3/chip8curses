#define _GNU_SOURCE
#include <sched.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ncurses.h>
#include <signal.h>
#include <time.h>

#include "main.h"
#include "interpreter.h"
#include "misc.h"

static int key_logger(void * delay)
{
	// nodelay(stdscr, TRUE);
	// int delay_time_tenth = ((struct timespec *) delay) -> tv_nsec = delay_time_tenth / (100000000);

	halfdelay(1); // half delay is cancelling raw, so ctrl + c will terminate the program.
	logger("character is %p\n", pressed_key);
	while (true)
	{
		// timeout(-1);
		// nodelay(stdscr, FALSE);
		// (*(int *)input) = getch();
		pressed_key = getch();

		switch (pressed_key)
		{
			case 'q':
				quit(RETURN_VALUE_SUCCESS);
			break;
			case '\n':
				if (is_running())
				{
				// nodelay(stdscr, TRUE);
				// halfdelay(1); // half delay is cancelling raw, so ctrl + c will terminate the program.
				pause_execution();
				}
				else
				{
					resume_execution();
					// nodelay(stdscr, FALSE);
					// raw();
				}
			break;
			case 0x10D: // <F5>
				execute_opcode();
			break;
			case 0x10E: // <F6>
				if (((struct timespec *) delay) -> tv_nsec - 1000000 >= 0)
				{
					// delay_time_tenth += 1;
					// ((struct timespec *) delay) -> tv_nsec = delay_time_tenth * (100000000);
					((struct timespec *) delay) -> tv_nsec -= 1000000;
					logger("setting delay to %ld nano of second.\n", ((struct timespec *) delay) -> tv_nsec);
				}
			break;
			case 0x10F: // <F7>
				if (((struct timespec *) delay) -> tv_nsec < 1000000000 - 1000000)
				{
					// delay_time_tenth -= 1;
					// ((struct timespec *) delay) -> tv_nsec = delay_time_tenth * (100000000);
					((struct timespec *) delay) -> tv_nsec += 1000000;
					logger("setting delay to %ld nano of second.\n", ((struct timespec *) delay) -> tv_nsec);
				}
			break;
		}
	}
	return -1;
}

bool read_rom(const char * rom_path)
{
	int fd = open (rom_path, O_RDONLY);
	if (fd == -1)
	{
		return false;
	}
	memset(&memory.program, 0, sizeof(memory.program));
	ssize_t rom_size = read(fd, memory.program, sizeof(memory.program));
	#ifdef _DEBUG
	// error_logger("rom size is %u bytes.\n", rom_size);
	#endif
	if (rom_size == 0)
	{
		return false;
	}
	return true;
}

bool start_key_logger(struct timespec * delay)
{
	const size_t STACK_SIZE = 0x1000;

	char * stack = malloc(STACK_SIZE);
	if (stack == NULL)
	{
		return false;
	}
	if  (clone(key_logger, stack + STACK_SIZE-1, CLONE_VM | CLONE_FILES | SIGQUIT, delay) == -1)
	{
		return false;	
	}
	return true;
}

void redraw_game_screen()
{
	const chtype fill = ACS_BLOCK | COLOR_PAIR(1);
	const chtype blank = ' ';

	for (int y = 0; y < 32; ++y)
	{
		for (int x = 0; x < 64/8; ++x)
		{
			mvwaddch(display_window, 1 + y, 1 + (x * 8), (memory.screen[y][x] & 0b10000000) ? fill : blank );
			waddch(display_window, (memory.screen[y][x] & 0b1000000) ? fill : blank );
			waddch(display_window, (memory.screen[y][x] & 0b100000) ? fill : blank );
			waddch(display_window, (memory.screen[y][x] & 0b10000) ? fill : blank );
			waddch(display_window, (memory.screen[y][x] & 0b1000) ? fill : blank );
			waddch(display_window, (memory.screen[y][x] & 0b100) ? fill : blank );
			waddch(display_window, (memory.screen[y][x] & 0b10) ? fill : blank );
			waddch(display_window, (memory.screen[y][x] & 0b1) ? fill : blank );
		}
	}
}

void print_registers()
{
	mvwprintw(registers_window, 1, 1, "v0: %#X - %d\n", v[0], v[0]);
	mvwprintw(registers_window, 2, 1, "v1: %#X - %d\n", v[1], v[1]);
	mvwprintw(registers_window, 3, 1, "v2: %#X - %d\n", v[2], v[2]);
	mvwprintw(registers_window, 4, 1, "v3: %#X - %d\n", v[3], v[3]);
	mvwprintw(registers_window, 5, 1, "v4: %#X - %d\n", v[4], v[4]);
	mvwprintw(registers_window, 6, 1, "v5: %#X - %d\n", v[5], v[5]);
	mvwprintw(registers_window, 7, 1, "v6: %#X - %d\n", v[6], v[6]);
	mvwprintw(registers_window, 8, 1, "v7: %#X - %d\n", v[7], v[7]);
	mvwprintw(registers_window, 1, 30, "v8: %#X - %d\n", v[8], v[8]);
	mvwprintw(registers_window, 2, 30, "v9: %#X - %d\n", v[9], v[9]);
	mvwprintw(registers_window, 3, 30, "va: %#X - %d\n", v[0xa], v[0xa]);
	mvwprintw(registers_window, 4, 30, "vb: %#X - %d\n", v[0xb], v[0xb]);
	mvwprintw(registers_window, 5, 30, "vc: %#X - %d\n", v[0xc], v[0xc]);
	mvwprintw(registers_window, 6, 30, "vd: %#X - %d\n", v[0xd], v[0xd]);
	mvwprintw(registers_window, 7, 30, "ve: %#X - %d\n", v[0xe], v[0xe]);
	mvwprintw(registers_window, 8, 30, "vf: %#X - %d\n", v[0xf], v[0xf]);
	mvwprintw(registers_window, 1, 50, "pc: %#X - %#02X%02X\n", pc, ((opcode_t *)(&(memory.start[pc])))->first, ((opcode_t *)(&(memory.start[pc])))->NN);
	mvwprintw(registers_window, 2, 50, "keyboard: %X\n", pressed_key);
	mvwprintw(registers_window, 3, 50, "l: %X\n", l);
	mvwprintw(registers_window, 4, 50, "delay_timer: %d\n", delay_timer);
	mvwprintw(registers_window, 5, 50, "sound_timer: %d\n", sound_timer);
}