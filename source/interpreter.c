#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "main.h"

/*
 * @brief Interpret the opcode at memory location pc, and increase pc by 2.
 */
void execute_opcode()
{
	opcode_t opcode = *((opcode_t *)(&(memory.start[pc])));
	logger("x is %X y is %X N is %X NN is %X\n", opcode.x, opcode.y, opcode.N, opcode.NN);
	int misc;
	switch (opcode.first >> 4)
	{
	case 0x0:
		if (opcode.x == 0)
		{
			if (opcode.NN == 0xEE)
			{
				logger("return. current_stack is %lu\n", current_stack);
				#ifdef _DEBUG
				if (current_stack == 0)
				{
					error_logger("return from outer space.\n");
				}
				#endif
				pc = stack[--current_stack];
				break;
			}
			if (opcode.NN == 0xE0)
			{
				logger("cls\n");
				memset(memory.screen, 0, sizeof(memory.screen));
				break;
			}
			error_logger("call 1802. unsupported. ignoring\n");
		}
		error_logger("unknown opcode: %#2x%2x\n", opcode.first, opcode.NN);
		break;
	case 0x1:
		logger("goto from %#2x ", pc);
		misc = pc;
		pc = opcode.x;
		pc <<= 8;
		pc += opcode.NN;
		logger("to %#2x\n", pc);
		if (misc == pc)
		{
			logger("Program is ended.\n");
		}
		pc -= 2;
		break;
	case 0x2:
		logger("call\n");
		stack[current_stack++] = pc;
		pc = opcode.x;
		pc <<= 8;
		pc += opcode.NN;
		pc -= 2;
		break;
	case 0x3:
		logger("skip if(Vx==NN)\n");
		if (v[opcode.x] == opcode.NN)
		{
			pc += 2;
		}
		break;
	case 0x4:
		logger("skip if(Vx!=NN)\n");
		if (v[opcode.x] != opcode.NN)
		{
			pc += 2;
		}
		break;
	case 0x5:
		logger("skip if(Vx==Vy) \n");
		if (v[opcode.x] == v[opcode.y])
		{
			pc += 2;
		}
		break;
	case 0x6:
		logger("Vx = NN \n");
		v[opcode.x] = opcode.NN;
		break;
	case 0x7:
		logger("Vx += NN\n");
		v[opcode.x] += opcode.NN;
		break;
	case 0x8:
		switch (opcode.N)
		{
		case 0x0:
			logger("Vx=Vy\n");
			v[opcode.x] = v[opcode.y];
			break;
		case 0x1:
			logger("Vx|=Vy\n");
			v[opcode.x] |= v[opcode.y];
			break;
		case 0x2:
			logger("Vx&=Vy\n");
			v[opcode.x] &= v[opcode.y];
			break;
		case 0x3:
			logger("Vx^=Vy\n");
			v[opcode.x] ^= v[opcode.y];
			break;
		case 0x4:
			logger("Vx+=Vy\n");
			v[opcode.x] += v[opcode.y];
			break;
		case 0x5:
			logger("Vx-=Vy\n");
			v[opcode.x] -= v[opcode.y];
			break;
		case 0x6:
			logger("Vx>>=1\n");
			v[0xF] = 1 & v[opcode.x];
			v[opcode.x] >>= 1;
			break;
		case 0x7:
			logger("Vx=Vy-Vx\n");
			if (v[opcode.x] > v[opcode.y])
				v[0xF] = 0;
			else
				v[0xF] = 1;
			v[opcode.x] = v[opcode.y] - v[opcode.x];
			break;
		case 0xE:
			logger("Vx<<=1\n");
			v[0xF] = 0b10000000 & v[opcode.x];
			v[opcode.x] <<= 1;
			break;
		default:
			error_logger("unknown opcode: %#2x%2x\n", opcode.first, opcode.NN);
			break;
		}
		break;
	case 0x9:
		logger("if(Vx!=Vy)\n");
		if (v[opcode.x] != v[opcode.y])
		{
			pc +=2;
		}
		break;
	case 0xA:
		logger("I = NNN\n");
		l = opcode.x;
		l <<= 8;
		l += opcode.NN;
		break;
	case 0xB:
		logger("jmp $+NNN\n");
		pc = opcode.x;
		pc <<= 8;
		pc += opcode.NN;
		pc += v[0];
		pc -= 2;
		break;
	case 0xC:
		logger("Vx=rand()&NN\n");
		v[opcode.x] = (rand() % 256) & opcode.NN;
		break;
	case 0xD:
		v[0xF] = 0;

		for (int y = 0; y < opcode.N; ++y)
		{
			logger("draw(Vx,Vy,N) character at %p-%x\n",&memory.start[y + l], memory.start[y + l]);
			unsigned char test = memory.screen[v[opcode.y]+y][v[opcode.x] / 8];
			unsigned char test_2 = memory.screen[v[opcode.y]+y][v[opcode.x] / 8 + 1];
			memory.screen[v[opcode.y]+y][(v[opcode.x] / 8)] ^= (memory.start[y + l]) >> (v[opcode.x] % 8);
			memory.screen[v[opcode.y]+y][(v[opcode.x] / 8) + 1] ^= (memory.start[y + l]) << (8 - (v[opcode.x] % 8));
			if (test != (test & memory.screen[v[opcode.y]+y][v[opcode.x] / 8]))
			{
				v[0xF] = 1;
			}
			if (test_2 != (test_2 & memory.screen[v[opcode.y]+y][v[opcode.x] / 8 + 1]))
			{
				v[0xF] = 1;
			}
		}
		break;
	case 0xE:
		switch (opcode.NN)
		{ 
		case 0x9E:
			logger("if(key()==Vx)\tVx=%#X\tpressed key=%#X\n", keyboard[v[opcode.x]], pressed_key);
			if (keyboard[v[opcode.x]] == pressed_key)
			{
				logger("key is pressed.\n");
				pc +=2;
			}
			break;
		case 0xA1:
			logger("if(key()!=Vx)\tVx=%#X\tpressed key=%#X\n", keyboard[v[opcode.x]], pressed_key);
			if (keyboard[v[opcode.x]] != pressed_key)
			{
				pc +=2;
			}
			break;
		default:
			error_logger("unknown opcode: %#2x%2x\n", opcode.first, opcode.NN);
		}
		break;
	case 0xF:
		switch (opcode.NN)
		{
			case 0x07:
				logger("Vx = get_delay()\n");
				v[opcode.x] = delay_timer;
				break;
			case 0x0A:
				logger("Vx = get_key()\n");
				wrefresh(log_window); // We are refreshing out log window because we are entering into a loop that may consume a lot of time.
				bool is_key_pressed = false;

				while (is_key_pressed == false)
				{
					for (size_t i = 0; i < 16; i++)
					{
						if (keyboard[i] == pressed_key)
						{
							v[opcode.x] = i;
							is_key_pressed = true;
							break;
						}
					}
				}
				break;
			case 0x15:
				logger("delay_timer(Vx)\n");
				delay_timer = v[opcode.x];
				break;
			case 0x18:
				logger("sound_timer(Vx)\n");
				sound_timer = v[opcode.x];
				break;
			case 0x1E:
				logger("I +=Vx\n");
				v[0xf] = 0;
				if (l + v[opcode.x] > 0xFFF)
				{
					v[0xf] = 1;
				}
				l += v[opcode.x];
				break;
			case 0x29:
				logger("I=sprite_addr[Vx]\n");
				l = v[opcode.x] * 5;
				break;
			case 0x33:
				logger("set_BCD(Vx);\n");
				memory.start[l] = (v[opcode.x] - (v[opcode.x] % 1000)) % 100;
				memory.start[l+1] = (v[opcode.x] - (v[opcode.x] % 100)) % 10;
				memory.start[l+2] = (v[opcode.x] - (v[opcode.x] % 10)) % 1;
				break;
			case 0x55:
				logger("reg_dump(Vx,&I)\n");
				for (size_t i = 0; i <= opcode.x; ++i)
				{
					memory.start[l+(i*sizeof(v[0]))] = v[i];
				}
				break;
			case 0x65:
				logger("reg_load(Vx,&I)\n");
				for (size_t i = 0; i <= opcode.x; ++i)
				{
					v[i] = memory.start[l+(i*sizeof(v[0]))];
				}
				break;

		default:
			error_logger("unknown opcode: %#2x%2x\n", opcode.first, opcode.NN);
		}
		break;
	default:
		error_logger("unknown opcode: %#2x%2x\n", opcode.first, opcode.NN);
	}
	pc+=2;
}
