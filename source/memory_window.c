#include <ncurses.h>
#include "main.h"

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
	return memory.start[address];
	return memory.program[address];
}

void refresh_memory_window()
{
	wmove(memory_window, 1, 1);
	for (size_t i = 0; i < sizeof(memory.program) / 16; ++i)
	{
		// I can't use colors with mvprintw arguments...
/*		mvwprintw(memory_window, 1 + i, 1, "0x%X\t%02X%02X %02X%02X  %02X%02X %02X%02X    %02X%02X %02X%02X  %02X%02X %02X%02X", (i*16)+sizeof(memory.start), 	get_colored_memory(i*16), get_colored_memory(i*16+1), get_colored_memory(i*16+2), get_colored_memory(i*16+3),
																															get_colored_memory(i*16+4), get_colored_memory(i*16+5), get_colored_memory(i*16+6), get_colored_memory(i*16+7),
																															get_colored_memory(i*16+8), get_colored_memory(i*16+9), get_colored_memory(i*16+10), get_colored_memory(i*16+11),
																															get_colored_memory(i*16+12), get_colored_memory(i*16+13), get_colored_memory(i*16+14), get_colored_memory(i*16+15));*/
	mvwprintw(memory_window, 1 + i, 1, "0x%X\t", (i*16)/*+sizeof(memory.start)*/);
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
