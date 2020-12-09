#include "m6502.h"

unsigned char m6502_ora(unsigned char data)
{
	m6502_a |= data;
	m6502_f = (m6502_f & NOTSZ) | sztable[m6502_a];

	return 0;
}

unsigned char m6502_and(unsigned char data)
{
	m6502_a &= data;
	m6502_f = (m6502_f & NOTSZ) | sztable[m6502_a];
        
	return 0;
}

unsigned char m6502_eor(unsigned char data)
{
	m6502_a ^= data;
	m6502_f = (m6502_f & NOTSZ) | sztable[m6502_a];
        
	return 0;
}

unsigned char m6502_bit(unsigned char data)
{
	m6502_f = (m6502_f & NOTNVZ) | (sztable[m6502_a & data]&2) | (data&192);
        
	return 0;
}

