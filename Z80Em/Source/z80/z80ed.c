#include "z80.h"

void z80_im0(void *var1, void *var2)
{
	imode = (imode&252) | 0;
}

void z80_im1(void *var1, void *var2)
{
	imode = (imode&252) | 1;
}

void z80_im2(void *var1, void *var2)
{
	imode = (imode&252) | 2;
}

void z80_neg(void *var1, void *var2)
{
	af.full = z80_subhvc[af.bytes.high] | 2;
	af.bytes.low |= z80_sz53p[af.bytes.high] &232;
}

void z80_lddr(void *var1, void *var2) //NULL, NULL
{
	unsigned char ft;

	z80_outaddr.full = de.full;
	z80_outflag = 1;
	z80_outword.bytes.low = readmemory(hl.full);

	de.full--;
	hl.full--;
	bc.full--;

	if(bc.full)
	{
		pc -=2;
		z80_tstates += R_ADD;
	}
	else
	{
		af.bytes.low &= 193;
		ft = af.bytes.high + readmemory(hl.full+1);
		af.bytes.low |= ((ft & 2) << 4) | (ft&8);
	}
}

void z80_ldir(void *var1, void *var2) //NULL, NULL
{
	unsigned char ft;

	z80_outaddr.full = de.full;
	z80_outflag = 1;
	z80_outword.bytes.low = readmemory(hl.full);

	de.full++;
	hl.full++;
	bc.full--;

	if(bc.full)
	{
		pc -=2;
		z80_tstates += R_ADD;
	}
	else
	{
		af.bytes.low &= 193;
		ft = af.bytes.high + readmemory(hl.full-1);
		af.bytes.low |= ((ft & 2) << 4) | (ft&8);
	}
}

void z80_ldi(void *var1, void *var2)
{
	unsigned char ft;

	af.bytes.low &= 193;

	z80_outaddr.full = de.full;
	z80_outflag = 1;
	z80_outword.bytes.low = readmemory(hl.full);
	ft = af.bytes.high + readmemory(hl.full);

	de.full++;
	hl.full++;
	bc.full--;

	af.bytes.low |= (ft&8) | ((ft&2) << 4);

	if(bc.full)
		af.bytes.low |= 4;
	else
		af.bytes.low &= 251;
}

void z80_ldd(void *var1, void *var2)
{
	unsigned char ft;

	af.bytes.low &= 193;

	z80_outaddr.full = de.full;
	z80_outflag = 1;
	z80_outword.bytes.low = readmemory(hl.full);
	ft = af.bytes.high + readmemory(hl.full);

	de.full--;
	hl.full--;
	bc.full--;

	af.bytes.low |= (ft&8) | ((ft&2) << 4);

	if(bc.full)
		af.bytes.low |= 4;
}

void z80_cpir(void *var1, void *var2) //NULL, NULL
{
	Z80_REGISTERPAIR result;

	result.full = z80_subhvc[(af.bytes.high << 8) | readmemory(hl.full)];
	hl.full++;
	bc.full--;

	if(!result.bytes.high || !bc.full)
	{
		af.bytes.low = (af.bytes.low&1) | 2;

		if(bc.full)
			af.bytes.low |= 4;

		af.bytes.low |= (result.bytes.low&16) | (z80_sz53p[result.bytes.high]&192);
	}
	else
	{
		pc -= 2;
		z80_tstates += R_ADD;
	}
}

void z80_cpdr(void *var1, void *var2) //NULL, NULL
{
	Z80_REGISTERPAIR result;

	result.full = z80_subhvc[(af.bytes.high << 8) | readmemory(hl.full)];
	hl.full--;
	bc.full--;

	if(!result.bytes.high || !bc.full)
	{
		af.bytes.low = (af.bytes.low&1) | 2;

		if(bc.full)
			af.bytes.low |= 4;

		af.bytes.low |= (result.bytes.low&16) | (z80_sz53p[result.bytes.high]&192);
	}
	else
	{
		pc -= 2;
		z80_tstates += R_ADD;
	}
}

void z80_cpd(void *var1, void *var2) //NULL, NULL
{
	Z80_REGISTERPAIR result;

	result.full = z80_subhvc[(af.bytes.high << 8) | readmemory(hl.full)];
	hl.full--;
	bc.full--;

	af.bytes.low = (af.bytes.low&1) | 2 | (result.bytes.low&16) | (z80_sz53p[result.bytes.high]&192);

	if(bc.full)
		af.bytes.low |= 4;
}

void z80_cpi(void *var1, void *var2) //NULL, NULL
{
	Z80_REGISTERPAIR result;

	result.full = z80_subhvc[(af.bytes.high << 8) | readmemory(hl.full)];
	hl.full++;
	bc.full--;

	af.bytes.low = (af.bytes.low&1) | 2 | (result.bytes.low&16) | (z80_sz53p[result.bytes.high]&192);

	if(bc.full)
		af.bytes.low |= 4;
}


void z80_in(void *var1, void *var2) //8bit, 16bit
{
	z80_port_in(*(unsigned short *)var2, (unsigned char *)var1);
	af.bytes.low = z80_sz53p[*(unsigned char *)var1] | (af.bytes.low&1);
}

void z80_out(void *var1, void *var2) //8bit, 16bit
{
	z80_port_out(*(unsigned short *)var2, *(unsigned char *)var1);
}

void z80_outd(void *var1, void *var2) //8bit, 16bit
{
	z80_port_out(bc.full, readmemory(hl.full));
	hl.full--;
	z80_dec8(&bc.bytes.high, NULL);
}

void z80_outi(void *var1, void *var2) //8bit, 16bit
{
	z80_port_out(bc.full, readmemory(hl.full));
	hl.full++;
	z80_dec8(&bc.bytes.high, NULL);
}

void z80_ind(void *var1, void *var2) //8bit, 16bit
{
	z80_outaddr.full = hl.full;
	z80_outflag = 1;
	z80_port_in(bc.full, &z80_outword.bytes.low);
	hl.full--;
	z80_dec8(&bc.bytes.high, NULL);
}

void z80_ini(void *var1, void *var2) //8bit, 16bit
{
	z80_outaddr.full = hl.full;
	z80_outflag = 1;
	z80_port_in(bc.full, &z80_outword.bytes.low);
	hl.full++;
	z80_dec8(&bc.bytes.high, NULL);
}

void z80_go_ed(void *var1, void *var2)
{
	unsigned char instr;

	inc_r();

	instr = readmemory(pc);
	pc++;
	z80_tstates += z80_edcyc[instr];
	z80_edinst[instr].func(z80_edinst[instr].var1, z80_edinst[instr].var2);
}
