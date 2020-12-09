#include "z80.h"

#define inc(x,y)	result.full = z80_addhvc[256 | x];\
			y = result.bytes.high;\
			af.bytes.low = (af.bytes.low&1) | (result.bytes.low&20) | (z80_sz53p[result.bytes.high]&232);

#define dec(x,y)	result.full = z80_subhvc[(x << 8) | 1];\
			y = result.bytes.high;\
			af.bytes.low = (af.bytes.low&1) | (result.bytes.low&20) | (z80_sz53p[result.bytes.high]&232) | 2;

#define comp(x)		result.full = z80_subhvc[(af.bytes.high << 8) | x];\
			af.bytes.low = result.bytes.low | 2 | (z80_sz53p[result.bytes.high] & 192) | (x&40);

#define add(x)		af.full = z80_addhvc[(af.bytes.high << 8) | x];\
			af.bytes.low |= (z80_sz53p[af.bytes.high]&232);

#define adc(x)		af.full = z80_addhvc[((af.bytes.low&1) << 16) | (af.bytes.high << 8) | x];\
			af.bytes.low |= (z80_sz53p[af.bytes.high]&232);

#define sub(x)		af.full = z80_subhvc[(af.bytes.high << 8) | x];\
			af.bytes.low |= (z80_sz53p[af.bytes.high]&232) | 2;

#define sbc(x)		af.full = z80_subhvc[((af.bytes.low&1) << 16) | (af.bytes.high << 8) | x];\
			af.bytes.low |= (z80_sz53p[af.bytes.high]&232) | 2;


void z80_inc8(void *var1, void *var2) //8bit, NULL
{
	Z80_REGISTERPAIR result;

	inc(*(unsigned char *)var1,*(unsigned char *)var1);
}

void z80_inc8rf(void *var1, void *var2) //16bit, NULL
{
	Z80_REGISTERPAIR result;

	z80_outaddr.full = *(unsigned short *)var1;
	inc(readmemory(z80_outaddr.full), z80_outword.bytes.low);
	z80_outflag = 1;
}

void z80_dec8(void *var1, void *var2) //8bit, NULL
{
	Z80_REGISTERPAIR result;

	dec(*(unsigned char *)var1,*(unsigned char *)var1);
}

void z80_dec8rf(void *var1, void *var2) //16bit, NULL
{
	Z80_REGISTERPAIR result;

	z80_outaddr.full = *(unsigned short *)var1;
	dec(readmemory(z80_outaddr.full), z80_outword.bytes.low);
	z80_outflag = 1;
}

void z80_inc16(void *var1, void *var2) //16bit, NULL
{
	(*(unsigned short *)var1)++;
}

void z80_dec16(void *var1, void *var2) //16bit, NULL
{
	(*(unsigned short *)var1)--;
}

void z80_cpn(void *var1, void *var2) //NULL, NULL
{
	Z80_REGISTERPAIR result;

	comp(readmemory(pc));
	pc++;
}

void z80_cp(void *var1, void *var2) //8bit, NULL
{
	Z80_REGISTERPAIR result;

	comp(*(unsigned char *)var1);
}

void z80_cprf(void *var1, void *var2) //16bit, NULL
{
	Z80_REGISTERPAIR result;

	comp(readmemory(*(unsigned short *)var1));
}

void z80_add8(void *var1, void *var2) //8bit, NULL
{
	add(*(unsigned char *)var1);
}

void z80_add8n(void *var1, void *var2) //8bit, NULL
{
	add(readmemory(pc));
	pc++;
}

void z80_add8rf(void *var1, void *var2) //16bit, NULL
{
	add(readmemory(*(unsigned short *)var1));
}

void z80_adc8(void *var1, void *var2) //8bit, NULL
{
	adc(*(unsigned char *)var1);
}

void z80_adc8n(void *var1, void *var2) //8bit, NULL
{
	adc(readmemory(pc));
	pc++;
}

void z80_adc8rf(void *var1, void *var2) //16bit, NULL
{
	adc(readmemory(*(unsigned short *)var1));
}

void z80_sub8(void *var1, void *var2) //8bit, NULL
{
	sub(*(unsigned char *)var1);
}

void z80_sub8n(void *var1, void *var2) //NULL, NULL
{
	sub(readmemory(pc));
	pc++;
}

void z80_sub8rf(void *var1, void *var2) //16bit, NULL
{
	sub(readmemory(*(unsigned short *)var1));
}

void z80_sbc8(void *var1, void *var2) //8bit, NULL
{
	sbc(*(unsigned char *)var1);
}

void z80_sbc8n(void *var1, void *var2) //8bit, NULL
{
	sbc(readmemory(pc));
	pc++;
}

void z80_sbc8rf(void *var1, void *var2) //16bit, NULL
{
	sbc(readmemory(*(unsigned short *)var1));
}

void z80_add16(void *var1, void *var2) //16bit, 16bit
{
	Z80_REGISTERPAIR result1, result2;

	result1.full	= z80_addhvc[	(((Z80_REGISTERPAIR *)var1)->bytes.low << 8)
					| ((Z80_REGISTERPAIR *)var2)->bytes.low];

	result2.full	= z80_addhvc[	((result1.bytes.low&1) << 16)
					| (((Z80_REGISTERPAIR *)var1)->bytes.high << 8)
					|  ((Z80_REGISTERPAIR *)var2)->bytes.high];

	af.bytes.low = (af.bytes.low & 196) | (result2.bytes.low & 17) | (result2.bytes.high & 40);
	result2.bytes.low = result1.bytes.high;
	((Z80_REGISTERPAIR *)var1)->full = result2.full;
}

void z80_adc16(void *var1, void *var2) //16bit, 16bit
{
	Z80_REGISTERPAIR result1, result2;

	result1.full	= z80_addhvc[	((af.bytes.low&1)<<16)
					| (((Z80_REGISTERPAIR *)var1)->bytes.low << 8)
					| ((Z80_REGISTERPAIR *)var2)->bytes.low];

	result2.full	= z80_addhvc[	((result1.bytes.low&1) << 16)
					| (((Z80_REGISTERPAIR *)var1)->bytes.high << 8)
					| ((Z80_REGISTERPAIR *)var2)->bytes.high];

	af.bytes.low = result2.bytes.low | (result2.bytes.high & 168);
	result2.bytes.low = result1.bytes.high;
        if(!result2.full)
          af.bytes.low |= 64;
	((Z80_REGISTERPAIR *)var1)->full = result2.full;
}

void z80_sbc16(void *var1, void *var2) //8bit top, 8bit bottom - assumes HL as register to sbc from
{
	Z80_REGISTERPAIR result;

	result.full	= z80_subhvc[	((af.bytes.low&1)<<16)
					|   (hl.bytes.low << 8)
					|   *(unsigned char *)var2];

	hl.full 	= z80_subhvc[	((result.bytes.low&1)<<16)
					| (hl.bytes.high << 8)
					| *(unsigned char *)var1];

	af.bytes.low = hl.bytes.low | (z80_sz53p[hl.bytes.high] & 168) | 2;
	hl.bytes.low = result.bytes.high;

	if(!hl.full)
		af.bytes.low |= 64;
}

#undef sbc
#undef sub
#undef adc
#undef add
#undef comp
#undef dec
#undef inc

