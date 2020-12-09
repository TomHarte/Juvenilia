#include "z80.h"

#define and(x)	af.bytes.high &= x;\
		af.bytes.low = z80_sz53p[af.bytes.high] | 16;

#define xor(x)	af.bytes.high ^= x;\
		af.bytes.low = z80_sz53p[af.bytes.high];

#define or(x)	af.bytes.high |= x;\
		af.bytes.low = z80_sz53p[af.bytes.high];


void z80_and8(void *var1, void *var2) //8bit, NULL
{
	and(*(unsigned char *)var1);
}

void z80_and8rf(void *var1, void *var2) //8bit, NULL
{
	and(readmemory(*(unsigned short *)var1));
}

void z80_and8n(void *var1, void *var2) //NULL, NULL
{
	and(readmemory(pc));
	pc++;
}

void z80_xor8(void *var1, void *var2) //8bit, NULL
{
	xor(*(unsigned char *)var1);
}

void z80_xor8rf(void *var1, void *var2) //8bit, NULL
{
	xor(readmemory(*(unsigned short *)var1));
}

void z80_xor8n(void *var1, void *var2) //8bit, NULL
{
	xor(readmemory(pc));
	pc++;
}

void z80_or8(void *var1, void *var2) //8bit, NULL
{
	or(*(unsigned char *)var1);
}

void z80_or8rf(void *var1, void *var2) //8bit, NULL
{
	or(readmemory(*(unsigned short *)var1));
}

void z80_or8n(void *var1, void *var2) //8bit, NULL
{
	or(readmemory(pc));
	pc++;
}

#undef or
#undef xor
#undef and
