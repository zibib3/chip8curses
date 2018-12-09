#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>

#include "types.h"

static bool running = true;

/*
 * @brief Return if the emulator is running.
 *
 * @return true		- If we are running.
 * @return false	- On pause.
 */
bool is_running()
{
	return running;
}

/*
 * @brief Pause the emulator
 */
void pause_execution()
{
	running = false;
}

/*
 * @brief Resume the emulator
 */
void resume_execution()
{
	running = true;
}

/*
 * @brief Quit the emulator
 */
void quit(return_codes_t retval)
{
	endwin();
	_exit(retval);
}