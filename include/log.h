#ifndef __LOG_H__
#define __LOG_H__

#include <ncurses.h>
#include <unistd.h>

#include "main.h"
#include "misc.h"


#define logger(...) __logger(log_window, ##__VA_ARGS__)
#define error_logger(...) __error_logger(log_window, ##__VA_ARGS__)
static inline void __logger(WINDOW * log_window, char * fmt, ...)
{
	#ifdef _DEBUG
	va_list args;
	va_start(args, fmt);
	scroll(log_window);
	mvwprintw(log_window, 8, 1, fmt, args);
	#endif
}

static inline void __error_logger(WINDOW * log_window, char * fmt, ...)
{
	#ifdef _DEBUG
	va_list args;
	va_start(args, fmt);
	// nodelay(stdscr, FALSE);
	scroll(log_window);
	mvwprintw(log_window, 8, 1, fmt, args);
	wrefresh(log_window);
	pause_execution();
	// nodelay(stdscr, TRUE);
	#endif
}

#endif