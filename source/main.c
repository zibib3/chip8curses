#include <string.h>
#include <time.h>
#include <ncurses.h>
#include <stdlib.h>

#include "windows.h"
#include "main.h"
#include "main_utils.h"
#include "memory_window.h"

/*
 * @brief initialize the emulator, call to function that will create a thread for reading the
 *		  user input, and refresh the emulator state.
 */
int main(int argc, char const *argv[])
{
	return_codes_t retval = RETURN_VALUE_SUCCESS;
	struct timespec delay = {0, 0};
	// You can look at the file "font" to see what thats need to be. I copied it from "http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.4"
	const unsigned char fonts[] = {0xF0,0x90,0x90,0x90,0xF0,0x20,0x60,0x20,0x20,0x70,0xF0,0x10,0xF0,0x80,0xF0,0xF0,0x10,0xF0,0x10,0xF0,0x90,0x90,0xF0,0x10,0x10,0xF0,0x80,0xF0,0x10,0xF0,0xF0,0x80,0xF0,0x90,0xF0,0xF0,0x10,0x20,0x40,0x40,0xF0,0x90,0xF0,0x90,0xF0,0xF0,0x90,0xF0,0x10,0xF0,0xF0,0x90,0xF0,0x90,0x90,0xE0,0x90,0xE0,0x90,0xE0,0xF0,0x80,0x80,0x80,0xF0,0xE0,0x90,0x90,0x90,0xE0,0xF0,0x80,0xF0,0x80,0xF0,0xF0,0x80,0xF0,0x80,0x80};
	memcpy(memory.start, fonts, sizeof(fonts));	

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
	initscr();
	start_color();
	init_pair(1, COLOR_RED, COLOR_YELLOW);
	game_screen_window = newwin(32+2,64+2,2,5);
	if (!register_window(game_screen_window))
	{
		retval = RETURN_VALUE_CANT_REGISTER_WINDOW;
		goto exit_2;
	}
#ifdef _DEBUG
	log_window = newwin(10,66,37,5);
	if (!register_window(log_window))
	{
		retval = RETURN_VALUE_CANT_REGISTER_WINDOW;
		goto exit_2;
	}
	scrollok(log_window, TRUE);
	registers_window = newwin(10,70,2,72);
	if (!register_window(registers_window))
	{
		retval = RETURN_VALUE_CANT_REGISTER_WINDOW;
		goto exit_2;
	}
	memory_window = newwin(40,70,12,72);
	if (!register_window(memory_window))
	{
		retval = RETURN_VALUE_CANT_REGISTER_WINDOW;
		goto exit_2;
	}
#endif	

	start_input_listener(&delay);
	while (true)
	{
		if (delay_timer > 0)
		{
			delay_timer--;
		}
		if (sound_timer > 0)
		{
			if (sound_timer % 10 == 0)
			{
				beep();
			}
			sound_timer--;
		}
		refresh_memory_window();
		refresh_registers_window();
		refresh_game_screen_window();
		if (is_running())
		{
			execute_opcode();
		}
		nanosleep(&delay, NULL);
		repaint_registered_windows();
	}
exit_2:
	endwin();
exit:
	if (retval == RETURN_VALUE_INVALID_ARGUMENTS)
	{
		printf("Wrong usage. please do:\n\t./chip8curses romname\n");
	}
	return retval;
}
