#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "main.h"


#ifdef _DEBUG
#include <stdio.h>
#endif


bool read_rom(const char * rom_path)
{
	int fd = open (rom_path, O_RDONLY);
	if (fd == -1)
	{
		return false;
	}
	memset(&memory, 0, sizeof(memory));
	ssize_t rom_size = read(fd, memory.program, sizeof(memory.program));
	#ifdef _DEBUG
	printf("rom size is %u bytes.\n", rom_size);
	#endif
	if (rom_size == 0)
	{
		return false;
	}
	return true;
}

void print_the_display()
{
	const char fill = 'H';
	const char blank = '_';
	system("clear");
	for (int y = 0; y < 32; ++y)
	{
		for (int x = 0; x < 64/8; ++x)
		{
			putchar((memory.screen[y][x] & 0b1) ? fill : blank );
			putchar((memory.screen[y][x] & 0b10) ? fill : blank );
			putchar((memory.screen[y][x] & 0b100) ? fill : blank );
			putchar((memory.screen[y][x] & 0b1000) ? fill : blank );
			putchar((memory.screen[y][x] & 0b10000) ? fill : blank );
			putchar((memory.screen[y][x] & 0b100000) ? fill : blank );
			putchar((memory.screen[y][x] & 0b1000000) ? fill : blank );
			putchar((memory.screen[y][x] & 0b10000000) ? fill : blank );
		}
		putchar('\n');
	}
	putchar('\n');
}

void read_next_command()
{
	opcode_t opcode = *((opcode_t *)(&(memory.start[pc])));
	switch (opcode.first >> 4)
	{
	case 0x0:
		if (opcode.x == 0)
		{
			if (opcode.second = 0xEE)
			{
				pc = stack[--current_stack];
				break;
			}
		}
		printf("unknown opcode: %#2x%2x\n", opcode.first, opcode.second);
		getchar();
		break;
	case 0x1:
		pc = opcode.first - 0x10;
		pc <<= 8;
		pc += opcode.second;
		pc -= 2;
		break;
	case 0x2:
		stack[current_stack++] = pc;
		pc = opcode.first - 0x20;
		pc <<= 8;
		pc += opcode.second;
		pc -= 2;
		break;
	case 0x3:
		if (v[opcode.x] == opcode.second)
		{
			pc += 2;
		}
		break;
	case 0x4:
		if (v[opcode.x] != opcode.second)
		{
			pc += 2;
		}
		break;
	case 0x6:
		v[opcode.x] = opcode.second;
		break;
	case 0x7:
		v[opcode.x] += opcode.second;
		break;
	case 0xA:
		l = opcode.first - 0xA0;
		l <<= 8;
		l += opcode.second;
		break;
	case 0xB:
		pc = opcode.first - 0xB0;
		pc <<= 8;
		pc += opcode.second;
		pc += v[0];
		pc -= 2;
		break;
	case 0xC:
		v[opcode.x] = (rand() % 256) & opcode.N;
		break;
	case 0xD:
		v[0xF] = 0;

		for (int y = 0; y < opcode.N; ++y)
		{
			unsigned char test = memory.screen[v[opcode.y]][v[opcode.x]];

			memory.screen[v[opcode.y + y]][v[opcode.x] / 8] = ((char *) (&memory))[y + l];
			printf("unknown opcode: %#2x%2x\n", opcode.first, opcode.second);
			printf("changing line %u index %u at address %p.\nscreen is at: %p", v[opcode.y + y], v[opcode.x] / 8, &memory.screen[v[opcode.y + y]][v[opcode.x]], &memory.screen);
			// memory.screen[v[opcode.y + y]][v[opcode.x] / 8] = 0xff;
			
			if (test != (test & memory.screen[v[opcode.y + y]][v[opcode.x] / 8]))
			{
				v[0xF] = 1;
			}
			// getchar();
		}
		break;
	case 0xE:
		// switch (opcode.second)
		{ // todo: keyboard.

		// default:
			// printf("unknown opcode: %#2x%2x\n", opcode.first, opcode.second);
			// getchar();	
		}
		break;
	case 0xF:
		switch (opcode.second)
		{
			case 0x07: // todo: timer.
				break;
			case 0x15: // todo: timer.
				break;
			case 0x1E:
				l += v[opcode.x];
				break;
		default:
			printf("unknown opcode: %#2x%2x\n", opcode.first, opcode.second);
			getchar();	
		}
		break;
	default:
		printf("unknown opcode: %#2x%2x\n", opcode.first, opcode.second);
		getchar();
	}
	pc++;
	pc++;

}

void print_registers()
{
	printf("pc: %#X - %d\n", pc, pc-0x200);
	printf("v0: %#X - %d\n", v[0], v[0]);
	printf("v1: %#X - %d\n", v[1], v[1]);
}