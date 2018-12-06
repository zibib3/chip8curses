
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include <stdlib.h>

#include "windows.h"
#include "main.h"
#include "main_utils.h"
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
#ifdef _DEBUG
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
#endif

	raw();
	keypad(stdscr, TRUE);
	noecho();
	scrollok(log_window, TRUE);


	// nodelay(stdscr, FALSE);
	start_key_logger();

	logger("pressed_key is %p", &pressed_key);
	while (true)
	{
		if (delay_timer > 0)
		{
			delay_timer--;
		}
		if (sound_timer > 0)
		{
			sound_timer--;
		}
		refresh_memory_window();
		print_registers();
		redraw_game_screen();
		if (is_running())
		{
			execute_opcode();
		}
		else
		{
			if (pressed_key == 0x10D) // F5
			{
				execute_opcode();
				pressed_key = ERR;
			}
		}
		nanosleep(&delay, NULL);
		repaint();
		if (pressed_key == 'q')
		{
			goto exit;
		}
	}

exit:
	endwin();
	return retval;
}
