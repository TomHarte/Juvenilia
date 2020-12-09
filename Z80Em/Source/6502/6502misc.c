#include "m6502.h"

unsigned char m6502_nop(unsigned char data)
{
	return 0;
}

unsigned char m6502_store(unsigned char data)
{
	return data;
}

unsigned char m6502_setstore(unsigned char data)
{
	m6502_f = (m6502_f & NOTSZ) | sztable[data];
	return data;
}

unsigned char m6502_setf(unsigned char data)
{
	m6502_f |= data;
	return 0;
}

unsigned char m6502_clrf(unsigned char data)
{
	m6502_f &= ~data;
	return 0;
}
