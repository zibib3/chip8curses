#define _DEBUG

#include <stdint.h>

#define logger(fmt, ...)  nodelay(stdscr, FALSE); noraw(); nocbreak(); mvwprintw(log_window, 1, 1, fmt, ##__VA_ARGS__);get(); nodelay(stdscr, TRUE)


bool read_rom(const char * rom_path);
void read_next_command();
void print_the_display();
void print_registers();
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
			unsigned char y : 4;
			unsigned char N : 4;
		};
	};
} opcode_t;

extern memory_t memory;
extern unsigned char v[];
extern uint16_t l; // address register.
extern uint16_t pc; // eip register.
extern uint16_t stack[];
extern unsigned int current_stack;
extern WINDOW * log_window;
extern WINDOW * display_window;
extern WINDOW * registers_window;