#include "m6502.h"

unsigned char m6502_asl(unsigned char data)
{
	m6502_f = (m6502_f & NOTSZC) | (data >> 7);
	data <<= 1;
	m6502_f |= sztable[data];
	return data;
}

unsigned char m6502_lsr(unsigned char data)
{
	m6502_f = (m6502_f & NOTSZC) | (data & 1);
	data >>= 1;
	m6502_f |= sztable[data];
	return data;
}

unsigned char m6502_rol(unsigned char data)
{
	unsigned char c;

	c = m6502_f & 1;
	m6502_f = (m6502_f & NOTSZC) | (data >> 7);
	data = (data << 1) | c;
	m6502_f |= sztable[data];
	return data;
}

unsigned char m6502_ror(unsigned char data)
{
	unsigned char c;

	c = m6502_f << 7;
	m6502_f = (m6502_f & NOTSZC) | (data & 1);
	data = (data >> 1) | c;
	m6502_f |= sztable[data];
	return data;
}

