#include "z80.h"

#define drl(x,y)	cbit = af.bytes.low & 1;\
			af.bytes.low = x >> 7;\
			y = (x << 1) | cbit;\
			af.bytes.low |= z80_sz53p[x];

#define drr(x,y)	cbit = af.bytes.low & 1;\
			af.bytes.low = x &1;\
			y = (x >> 1) | (cbit << 7);\
			af.bytes.low |= z80_sz53p[x];

#define drlc(x,y)	af.bytes.low = x >> 7;\
			y = (x << 1) | af.bytes.low;\
			af.bytes.low |= z80_sz53p[x];

#define drrc(x,y)	af.bytes.low = x&1;\
			y = (x >> 1) | (af.bytes.low << 7);\
			af.bytes.low |= z80_sz53p[x];

#define dsra(x,y)	af.bytes.low = x &1;\
			y = (x >> 1) | (x & 128);\
			af.bytes.low |= z80_sz53p[x];

#define dsrl(x,y)	af.bytes.low = x &1;\
			y = x >> 1;\
			af.bytes.low |= z80_sz53p[x];

#define dsla(x,y)	af.bytes.low = x >> 7;\
			y = x << 1;\
			af.bytes.low |= z80_sz53p[x];

#define dsll(x,y)	af.bytes.low = x >> 7;\
			y = (x << 1) | 1;\
			af.bytes.low |= z80_sz53p[x];

void z80_rrca(void *var1, void *var2) //NULL, NULL
{
	af.bytes.low = (af.bytes.low&196) | (af.bytes.high&1);
	af.bytes.high = (af.bytes.high >> 1) | (af.bytes.low << 7);
	af.bytes.low |= af.bytes.high&40;
}

void z80_rra(void *var1, void *var2) //NULL, NULL
{
	unsigned char cbit;

	cbit = af.bytes.low&1;
	af.bytes.low = (af.bytes.low&196) | (af.bytes.high&1);
	af.bytes.high = (af.bytes.high >> 1) | (cbit << 7);
	af.bytes.low |= af.bytes.high&40;
}

void z80_rlca(void *var1, void *var2) //NULL, NULL
{
	af.bytes.low = (af.bytes.low&196) | (af.bytes.high >> 7);
	af.bytes.high = (af.bytes.high << 1) | (af.bytes.low & 1);
	af.bytes.low |= af.bytes.high&40;
}

void z80_rla(void *var1, void *var2) //NULL, NULL
{
	unsigned char cbit;

	cbit = af.bytes.low&1;
	af.bytes.low = (af.bytes.low&196) | (af.bytes.high >> 7);
	af.bytes.high = (af.bytes.high << 1) | cbit;
	af.bytes.low |= af.bytes.high&40;
}

void z80_rlc(void *var1, void *var2) //8bit, NULL
{
	drlc(*(unsigned char *)var1,*(unsigned char *)var1);
}

void z80_rlcrf(void *var1, void *var2) //8bit, NULL
{
	drlc(readmemory(*(unsigned short *)var1),z80_outword.bytes.low);
	z80_outaddr.full = *(unsigned short *)var1;
	z80_outflag = 1;
}

void z80_rl(void *var1, void *var2) //8bit, NULL
{
	unsigned char cbit;

	drl(*(unsigned char *)var1, *(unsigned char *)var1);
}

void z80_rlrf(void *var1, void *var2) //8bit, NULL
{
	unsigned char cbit;

	drl(readmemory(*(unsigned short *)var1),z80_outword.bytes.low);
	z80_outaddr.full = *(unsigned short *)var1;
	z80_outflag = 1;
}

void z80_rr(void *var1, void *var2) //8bit, NULL
{
	unsigned char cbit;

	drr(*(unsigned char *)var1,*(unsigned char *)var1);
}

void z80_rrrf(void *var1, void *var2) //8bit, NULL
{
	unsigned char cbit;

	drr(readmemory(*(unsigned short *)var1),z80_outword.bytes.low);
	z80_outaddr.full = *(unsigned short *)var1;
	z80_outflag = 1;
}

void z80_rrc(void *var1, void *var2) //8bit, NULL
{
	drrc(*(unsigned char *)var1,*(unsigned char *)var1);
}

void z80_rrcrf(void *var1, void *var2) //8bit, NULL
{
	drrc(readmemory(*(unsigned short *)var1),z80_outword.bytes.low);
	z80_outaddr.full = *(unsigned short *)var1;
	z80_outflag = 1;
}


void z80_sra(void *var1, void *var2) //8bit, NULL
{
	dsra(*(unsigned char *)var1,*(unsigned char *)var1);
}

void z80_srarf(void *var1, void *var2) //8bit, NULL
{
	dsra(readmemory(*(unsigned short *)var1),z80_outword.bytes.low);
	z80_outaddr.full = *(unsigned short *)var1;
	z80_outflag = 1;
}

void z80_srl(void *var1, void *var2) //8bit, NULL
{
	dsrl(*(unsigned char *)var1,*(unsigned char *)var1);
}

void z80_srlrf(void *var1, void *var2) //8bit, NULL
{
	dsrl(readmemory(*(unsigned short *)var1),z80_outword.bytes.low);
	z80_outaddr.full = *(unsigned short *)var1;
	z80_outflag = 1;
}

void z80_sla(void *var1, void *var2) //8bit, NULL
{
	dsla(*(unsigned char *)var1,*(unsigned char *)var1);
}

void z80_slarf(void *var1, void *var2) //8bit, NULL
{
	dsla(readmemory(*(unsigned short *)var1),z80_outword.bytes.low);
	z80_outaddr.full = *(unsigned short *)var1;
	z80_outflag = 1;
}

void z80_sll(void *var1, void *var2) //8bit, NULL
{
	dsll(*(unsigned char *)var1,*(unsigned char *)var1);
}

void z80_sllrf(void *var1, void *var2) //8bit, NULL
{
	dsll(readmemory(*(unsigned short *)var1),z80_outword.bytes.low);
	z80_outaddr.full = *(unsigned short *)var1;
	z80_outflag = 1;
}

void z80_rld(void *var1, void *var2) //8bit, NULL
{
	unsigned char spare;

	af.bytes.low &= 1;

	spare=af.bytes.high & 15;
	af.bytes.high = (af.bytes.high&240) | (readmemory(hl.full) >> 4);
	writememory(hl.full) = (readmemory(hl.full)<< 4) | spare;

	af.bytes.low |= z80_sz53p[af.bytes.high];
}

void z80_rrd(void *var1, void *var2) //8bit, NULL
{
	unsigned char spare;

	af.bytes.low &= 1;

	spare=af.bytes.high & 15;
	af.bytes.high = (af.bytes.high&240) | (readmemory(hl.full) & 15);
	writememory(hl.full) = (readmemory(hl.full) >> 4) | (spare << 4);

	af.bytes.low |= z80_sz53p[af.bytes.high];
}

#undef dsll
#undef dsla
#undef dsrl
#undef dsra
#undef drrc
#undef drlc
#undef drr
#undef drl
