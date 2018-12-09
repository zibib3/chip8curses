#ifndef __MAIN_UTILS_H__
#define __MAIN_UTILS_H__

#include <time.h>

bool read_rom(const char * rom_path);
bool start_input_listener(struct timespec * delay);
void refresh_game_screen_window();
void refresh_registers_window();
#endif
