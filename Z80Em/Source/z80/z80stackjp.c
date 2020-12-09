#include "z80.h"

#define get_jaddr()   jaddr.bytes.low = readmemory(pc);\
                      pc++;\
                      jaddr.bytes.high = readmemory(pc);\
                      pc++;


void z80_push(void *var1, void *var2) //top, bottom
{
	z80_outflag = 2;
	sp.full -= 2;
	z80_outaddr.full = sp.full;
	z80_outword.bytes.low = *(unsigned char *)var2;
	z80_outword.bytes.high = *(unsigned char *)var1;
}

void z80_pop(void *var1, void *var2) //top, bottom
{
	*(unsigned char *)var2 = readmemory(sp.full);
	sp.full++;
	*(unsigned char *)var1 = readmemory(sp.full);
	sp.full++;
}

void z80_call(void *var1, void *var2) //NULL, NULL
{
	Z80_REGISTERPAIR jaddr;

	get_jaddr();

	z80_outflag = 2;
	sp.full -= 2;
	z80_outaddr.full = sp.full;
	z80_outword.full = pc;

	pc = jaddr.full;
}

void z80_callcond(void *var1, void *var2) //8bit, NULL
{
	Z80_REGISTERPAIR jaddr;

	get_jaddr();

	if(af.bytes.low & *(unsigned char *)var1)
	{
		z80_outflag = 2;
		sp.full -= 2;
		z80_outaddr.full = sp.full;
		z80_outword.full = pc;

		pc = jaddr.full;
		z80_tstates += CALLCC_ADD;
	}
}

void z80_callncond(void *var1, void *var2) //8bit, NULL
{
	Z80_REGISTERPAIR jaddr;

	get_jaddr();

	if(!(af.bytes.low & *(unsigned char *)var1))
	{
		z80_outflag = 2;
		sp.full -= 2;
		z80_outaddr.full = sp.full;
		z80_outword.full = pc;

		pc = jaddr.full;
		z80_tstates += CALLCC_ADD;
	}
}

void z80_rst(void *var1, void *var2) //16bit, NULL
{
	z80_outflag = 2;
	sp.full -= 2;
	z80_outaddr.full = sp.full;
	z80_outword.full = pc;

	pc = *(unsigned short *)var1;
}

void z80_ret(void *var1, void *var2) //NULL, NULL
{
	Z80_REGISTERPAIR addr;

	addr.bytes.low = readmemory(sp.full);
	sp.full++;
	addr.bytes.high = readmemory(sp.full);
	sp.full++;
	pc = addr.full;
}

void z80_retcond(void *var1, void *var2) //8bit, NULL
{
	Z80_REGISTERPAIR addr;

	if(af.bytes.low & *(unsigned char *)var1)
	{
		addr.bytes.low = readmemory(sp.full);
		sp.full++;
		addr.bytes.high = readmemory(sp.full);
		sp.full++;

		pc = addr.full;
		z80_tstates += RETCC_ADD;
	}
}

void z80_retncond(void *var1, void *var2) //8bit, NULL
{
	Z80_REGISTERPAIR addr;

	if(!(af.bytes.low & *(unsigned char *)var1))
	{
		addr.bytes.low = readmemory(sp.full);
		sp.full++;
		addr.bytes.high = readmemory(sp.full);
		sp.full++;

		pc = addr.full;
		z80_tstates += RETCC_ADD;
	}
}

void z80_jp16(void *var1, void *var2) //NULL, NULL
{
	pc = *(unsigned short *)var1;
}

void z80_jpnn(void *var1, void *var2) //NULL, NULL
{
	Z80_REGISTERPAIR addr;

	addr.bytes.low = readmemory(pc);
	pc++;
	addr.bytes.high = readmemory(pc);
	pc = addr.full;
}

void z80_jrn(void *var1, void *var2) //condition, NULL
{
	pc += (char)readmemory(pc);
	pc++;
}

void z80_jpcond(void *var1, void *var2) //condition, NULL
{
	Z80_REGISTERPAIR addr;

	addr.bytes.low = readmemory(pc);
	pc++;
	addr.bytes.high = readmemory(pc);
	pc++;

	if(af.bytes.low & (*((unsigned char *)var1)))
	{
		pc = addr.full;
		z80_tstates += JPCC_ADD;
	}
}

void z80_jpncond(void *var1, void *var2) //condition, NULL
{
	Z80_REGISTERPAIR addr;

	addr.bytes.low = readmemory(pc);
	pc++;
	addr.bytes.high = readmemory(pc);
	pc++;

	if(!(af.bytes.low & (*((unsigned char *)var1))))
	{
		pc = addr.full;
		z80_tstates += JPCC_ADD;
	}
}

void z80_jrcond(void *var1, void *var2) //condition, NULL
{
	if(af.bytes.low & *(unsigned char *)var1)
	{
		pc += (char)readmemory(pc);
		z80_tstates += JRCC_ADD;
	}
	pc++;
}

void z80_jrncond(void *var1, void *var2) //condition, NULL
{
	if(! (af.bytes.low & *(unsigned char *)var1) )
	{
		pc += (char)readmemory(pc);
		z80_tstates += JRCC_ADD;
	}
	pc++;
}

void z80_djnz(void *var1, void *var2) //NULL, NULL
{
	bc.bytes.high--;

	if(bc.bytes.high)
	{
		pc += (char)readmemory(pc);
		z80_tstates += DJNZ_ADD;
	}
	pc++;
}

#undef get_jaddr
