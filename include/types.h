#ifndef __TYPES_H__
#define __TYPES_H__

typedef struct node_s
{
	WINDOW * window;
	struct node_s * next;
} node_t;

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

typedef enum
{
	RETURN_VALUE_SUCCESS = 0,
	RETURN_VALUE_INVALID_ARGUMENTS,
	RETURN_VALUE_CANT_REGISTER_WINDOW

} return_codes_t;

#endif