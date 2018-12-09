#ifndef __MAIN_UTILS_H__
#define __MAIN_UTILS_H__

#include <time.h>

bool read_rom(const char * rom_path);
bool start_key_logger(struct timespec * delay);
void redraw_game_screen();
void print_registers();
#endif
