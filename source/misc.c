#include <stdbool.h>

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