#include "m6502.h"

unsigned char m6502_adc(unsigned char data) //sets everything except ID
{
	m6502_a += data;
	return 0;
}

unsigned char m6502_sbc(unsigned char data) //sets same as ADC
{
	m6502_a -= data;
	return 0;
}

unsigned char m6502_dec(unsigned char data) //sets zero and sign
{
	data--;
	m6502_f = (m6502_f & NOTSZ) | sztable[data];
	return data;
}

unsigned char m6502_inc(unsigned char data) //sets zero and sign
{
	data++;
	m6502_f = (m6502_f & NOTSZ) | sztable[data];
	return data;
}

unsigned char m6502_cmp(unsigned char data) //sets sign, zero and carry
{
	return 0;
}

unsigned char m6502_cpx(unsigned char data) //sets same as cmp
{
	return 0;
}

unsigned char m6502_cpy(unsigned char data) //sets same as cmp
{
	return 0;
}
