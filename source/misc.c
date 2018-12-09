#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include "types.h"
static bool running = true;

bool is_running()
{
	return running;
}

void pause_execution()
{
	running = false;
}

void resume_execution()
{
	running = true;
}

void quit(return_codes_t retval)
{
	endwin();
	_exit(retval);
}