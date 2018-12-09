#ifndef __MISC_H__
#define __MISC_H__

#include "types.h"

bool is_running();
void pause_execution();
void resume_execution();
void quit(return_codes_t retval);
#endif