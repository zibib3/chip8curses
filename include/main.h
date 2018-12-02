// #define _DEBUG

#include <stdint.h>

// #define error_logger(fmt, ...)  nodelay(stdscr, FALSE); scroll(log_window); mvwprintw(log_window, 8, 1, fmt, ##__VA_ARGS__);wrefresh(log_window); get(); nodelay(stdscr, TRUE)
// #define logger(fmt, ...)   scroll(log_window); mvwprintw(log_window, 8, 1, fmt, ##__VA_ARGS__)

void logger(char * fmt, ...);
void error_logger(char * fmt, ...);

bool read_rom(const char * rom_path);
void read_next_command();
void redraw_game_screen();
void print_registers();
void repaint();
int get();

typedef struct
{
	unsigned char start[512]; // 0x200
	unsigned char program[3328]; // 0xF00 - 0x200
	// unsigned char screen[256]; // 0x1000 - 0xF00
	unsigned char screen[32][64/8]; // 0x1000 - 0xF00
} memory_t;

typedef struct
{
	union
	{
		unsigned char first;
		unsigned char x : 4;
	};
	union
	{
		unsigned char second;	
		struct
		{
			unsigned char N : 4;
			unsigned char y : 4;
		};
	};
} opcode_t;

extern int pressed_key;
extern memory_t memory;
extern unsigned char v[];
extern unsigned char delay_timer;
extern uint16_t l; // address register.
extern uint16_t pc; // eip register.
extern uint16_t stack[];
extern unsigned int current_stack;
extern WINDOW * log_window;
extern WINDOW * memory_window;
extern WINDOW * display_window;
extern WINDOW * registers_window;