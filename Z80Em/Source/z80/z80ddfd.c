#include "z80.h"

char d;

void z80_pd8(void *var1, void *var2)
{
	z80_outaddr.full = idx.full + d;
	z80_outflag = 1;
	z80_outword.bytes.low = readmemory(z80_outaddr.full);
	((funcptr)var1)(&z80_outword.bytes.low, var2);
	//((funcptr)var1)(&memory(idx.full + d), var2);
}

void z80_go_ddfd_cb(void *var1, void *var2)
{
	unsigned char instr;

	d = (char)readmemory(pc);

	pc++;
	instr = readmemory(pc);
	pc++;
	z80_ddfdcbinst[instr].func(z80_ddfdcbinst[instr].var1, z80_ddfdcbinst[instr].var2);
	z80_tstates += z80_ddfdcbcyc[instr];
}

void z80_prfd8(void *var1, void *var2)
{
	z80_outaddr.full = idx.full + (char)readmemory(pc);
        z80_outflag = 1;
        z80_outword.bytes.low = readmemory(z80_outaddr.full);

	pc++;
//        ((funcptr)var1)(&specmem[addr], var2);
	((funcptr)var2)(&z80_outword.bytes.low, NULL);
}

void z80_ld8pd(void *var1, void *var2) //8bit, 16bit
{
	*(unsigned char *)var1 = readmemory(*(unsigned short *)var2 + (char)readmemory(pc));
	pc++;
}

void z80_ldpdn(void *var1, void *var2) //16bit, NULL
{
	z80_outaddr.full = idx.full + (char)readmemory(pc);
	z80_outflag = 1;
	z80_outword.bytes.low = readmemory(pc+1);
	pc+=2;
}

void z80_ldpd8(void *var1, void *var2) //16bit, 8bit
{
	z80_outaddr.full = *(unsigned short *)var1 + (char)readmemory(pc);
	z80_outflag = 1;
	z80_outword.bytes.low = *(unsigned char *)var2;
	pc++;
}

void z80_go_ddfd(void *var1, void *var2)
{
	unsigned char instr;

	idx.full = ((Z80_REGISTERPAIR *)var1)->full;

	inc_r();

	instr = readmemory(pc);
	pc++;
	z80_tstates += z80_ddfdcyc[instr];
	z80_ddfdinst[instr].func(z80_ddfdinst[instr].var1, z80_ddfdinst[instr].var2);

	((Z80_REGISTERPAIR *)var1)->full = idx.full;
}
