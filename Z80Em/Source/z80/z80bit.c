#include "z80.h"

#define set(x,y,z)	z = x|y;
#define res(x,y,z)	z = x&~y;

#define bit(x,y)	af.bytes.low &= 1;\
			if(x&y)\
			{\
				af.bytes.low |= 16;\
				if(y & 168)\
					x |= y;\
			}\
			else\
				af.bytes.low |= 84;

void z80_setrf(void *var1, void *var2) //16bit, 8bit
{
	z80_outaddr.full = *(unsigned short *)var1;
	set(readmemory(z80_outaddr.full), *(unsigned char *)var2, z80_outword.bytes.low);
	z80_outflag = 1;
}

void z80_resrf(void *var1, void *var2) //16bit, 8bit
{
	z80_outaddr.full = *(unsigned short *)var1;
	res(readmemory(z80_outaddr.full), *(unsigned char *)var2, z80_outword.bytes.low);
	z80_outflag = 1;
}

void z80_set8(void *var1, void *var2) //8bit, 8bit
{
	set(*(unsigned char *)var1, *(unsigned char *)var2, *(unsigned char *)var1);
}

void z80_res8(void *var1, void *var2) //8bit, 8bit
{
	res(*(unsigned char *)var1, *(unsigned char *)var2, *(unsigned char *)var1);
}

void z80_bit8(void *var1, void *var2) //8bit, 8bit
{
	bit(*(unsigned char *)var1, *(unsigned char *)var2);
}

void z80_bitrf(void *var1, void *var2) //16bit, 8bit
{
	bit(readmemory(*(unsigned short *)var1), *(unsigned char *)var2);
}

#undef bit
#undef res
#undef set

