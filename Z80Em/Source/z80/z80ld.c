#include "z80.h"

#define get_addr()      addr.bytes.low = readmemory(pc);\
                        pc++;\
                        addr.bytes.high = readmemory(pc);\
                        pc++;

void z80_ld8(void *var1, void *var2) //8bit, 8bit			*var1 = *var2
{
	*(unsigned char *)var1 = *(unsigned char *)var2;
}

void z80_ld8n(void *var1, void *var2) //8bit, NULL			*val1 = specmem[pc]
{
	*(unsigned char *)var1	= readmemory(pc);
	pc++;
}

void z80_ldrf8(void *var1, void *var2) //16bit, 8bit		specmem[*var1] = *var2
{
	z80_outaddr.full = *(unsigned short *)var1;
	z80_outword.bytes.low = *(unsigned char *)var2;
	z80_outflag = 1;
}

void z80_ldrfn(void *var1, void *var2) //16bit, NULL		specmem[*var1] = specmem[pc]
{
	z80_outaddr.full = *(unsigned short *)var1;
	z80_outword.bytes.low = readmemory(pc);
	z80_outflag = 1;
	pc++;
}

void z80_ldrfnn8(void *var1, void *var2) //8bit, NULL		specmem[pc] = *var1
{
	Z80_REGISTERPAIR addr;
	get_addr();

	z80_outaddr.full = addr.full;
	z80_outword.bytes.low = *(unsigned char *)var1;
	z80_outflag = 1;
}

void z80_ld8rfnn(void *var1, void *var2) //8bit, NULL		opposite of above
{
	Z80_REGISTERPAIR addr;

	get_addr();

	*(unsigned char *)var1 = readmemory(addr.full);
}

void z80_ld8rf(void *var1, void *var2) //8bit, 16bit		*var1 = specmem[*var2]
{
	*(unsigned char *)var1 = readmemory(*(unsigned short *)var2);
}

void z80_ld16(void *var1, void *var2) //16bit, 16bit . . . *var1 = *var2
{
	*(unsigned short *)var1 = *(unsigned short *)var2;
}

void z80_ld16nn(void *var1, void *var2) //top, bottom		*var2 = specmem[pc], *var1 = specmem[pc+1]
{
	*((unsigned char *)var2)	= readmemory(pc);
	pc++;
	*((unsigned char *)var1)	= readmemory(pc);
	pc++;
}

void z80_ldrfnn16(void *var1, void *var2) //top, bottom		specmem[nn] = *var2, specmem[nn+1]=*var1
{
	Z80_REGISTERPAIR addr;
	get_addr();
	z80_outaddr.full = addr.full;
	z80_outword.bytes.low = *(unsigned char *)var2;
	z80_outword.bytes.high = *(unsigned char *)var1;
	z80_outflag = 2;
}

void z80_ld16rfnn(void *var1, void *var2) //top, bottom		opposite of above
{
	Z80_REGISTERPAIR addr;

	get_addr();

	*(unsigned char *)var2 = readmemory(addr.full);
	addr.full++;
	*(unsigned char *)var1 = readmemory(addr.full);
}

void z80_exx(void *var1, void *var2) //NULL, NULL
{
	unsigned short temp;
	temp = bc.full; bc.full = bcd.full; bcd.full = temp;
	temp = de.full; de.full = ded.full; ded.full = temp;
	temp = hl.full; hl.full = hld.full; hld.full = temp;
}

void z80_exdehl(void *var1, void *var2) //NULL, NULL
{
	unsigned short temp;
	temp = de.full; de.full = hl.full; hl.full = temp;
}

void z80_exafafd(void *var1, void *var2) //NULL, NULL
{
	unsigned short temp;
//	temp = af.bytes.high; af.bytes.high = afd.bytes.high; afd.bytes.high = temp;
	temp = af.full; af.full = afd.full; afd.full = temp;
}

void z80_exrfsp16(void *var1, void *var2) //top, bottom
{
	unsigned char temp;

	temp = readmemory(sp.full+1); writememory(sp.full+1) = *(unsigned char *)var1;*(unsigned char *)var1 = temp;
	temp = readmemory(sp.full); writememory(sp.full) = *(unsigned char *)var2; *(unsigned char *)var2 = temp;
}

#undef get_addr
