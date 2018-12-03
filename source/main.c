#include <string.h>
#include <time.h>
#include <ncurses.h>

#include "windows.h"
#include "main.h"
#include "memory_window.h"


int main(int argc, char const *argv[])
{
	return_codes_t retval = RETURN_VALUE_SUCCESS;
	const struct timespec delay = {0, 1000000000 / 60};
	const unsigned char fonts[] = {0xF0,0x90,0x90,0x90,0xF0,0x20,0x60,0x20,0x20,0x70,0xF0,0x10,0xF0,0x80,0xF0,0xF0,0x10,0xF0,0x10,0xF0,0x90,0x90,0xF0,0x10,0x10,0xF0,0x80,0xF0,0x10,0xF0,0xF0,0x80,0xF0,0x90,0xF0,0xF0,0x10,0x20,0x40,0x40,0xF0,0x90,0xF0,0x90,0xF0,0xF0,0x90,0xF0,0x10,0xF0,0xF0,0x90,0xF0,0x90,0x90,0xE0,0x90,0xE0,0x90,0xE0,0xF0,0x80,0x80,0x80,0xF0,0xE0,0x90,0x90,0x90,0xE0,0xF0,0x80,0xF0,0x80,0xF0,0xF0,0x80,0xF0,0x80,0x80};
	memcpy(memory.start, fonts, sizeof(fonts));

	initscr();
	start_color();
	init_pair(1, COLOR_RED, COLOR_YELLOW);
	if (argc != 2)
	{
		retval = RETURN_VALUE_INVALID_ARGUMENTS;
		goto exit;
	}
	for (size_t i = 0; i < sizeof(memory.screen); ++i)
	{
		((char *)(memory.screen))[i] = i;
	}
	if (!read_rom(argv[1]))
	{
		retval = RETURN_VALUE_INVALID_ARGUMENTS;
		goto exit;
	}

	display_window = newwin(32+2,64+2,2,5);
	if (!register_window(display_window))
	{
		retval = RETURN_VALUE_CANT_REGISTER_WINDOW;
		goto exit;
	}
	log_window = newwin(10,66,37,5);
	if (!register_window(log_window))
	{
		retval = RETURN_VALUE_CANT_REGISTER_WINDOW;
		goto exit;
	}
	registers_window = newwin(10,70,2,72);
	if (!register_window(registers_window))
	{
		retval = RETURN_VALUE_CANT_REGISTER_WINDOW;
		goto exit;
	}
	memory_window = newwin(40,70,12,72);
	if (!register_window(memory_window))
	{
		retval = RETURN_VALUE_CANT_REGISTER_WINDOW;
		goto exit;
	}

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
		if (v[0xf] == 1)
		{
			// error_logger("\n");
		}
		refresh_memory_window();
		print_registers();
		redraw_game_screen();
		execute_opcode();
		nanosleep(&delay, NULL);
		repaint();
		pressed_key  = get();
	}

exit:
	endwin();
	return retval;
}
