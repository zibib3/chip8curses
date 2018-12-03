
void logger(char * fmt, ...)
{
	#ifdef _DEBUG
	va_list args;
	va_start(args, fmt);
	scroll(log_window);
	mvwprintw(log_window, 8, 1, fmt, args);
	#endif
}

void error_logger(char * fmt, ...)
{
	#ifdef _DEBUG
	va_list args;
	va_start(args, fmt);
	nodelay(stdscr, FALSE);
	scroll(log_window);
	mvwprintw(log_window, 8, 1, fmt, args);
	wrefresh(log_window);
	get();
	nodelay(stdscr, TRUE);
	#endif
}
