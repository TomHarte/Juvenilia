#include "z80.h"

void hard_reset(void)
{
	af.full =
	bc.full =
	de.full =
	hl.full =
	ix.full =
	iy.full =
	afd.full =
	bcd.full =
	ded.full =
	hld.full = 0;
	pc = 0;
	z80_outflag = 0;
	z80_imchange = 0;
}


void create_hvc_lists(void)
{
	unsigned short var1, var2, cvar, result, myf;
	short lsv1, lsv2;
	unsigned short bit4;

	for(cvar = 0; cvar < 2; cvar++)
		for(var1 = 0; var1 < 256; var1++)
			for(var2 = 0; var2 < 256; var2++)
			{
				myf = 0;
				result = var1 + var2 + cvar;
				lsv1 = (char)var1;
				lsv2 = (char)var2;

				bit4 = (var1 & 15) + (var2 & 15) + cvar;
				if(bit4 & 240)
					myf |= 16;

				if(result & 65280)
					myf |= 1;

				if((char)(lsv2 + lsv1 +cvar) != lsv2 + lsv1+cvar)
					myf |= 4;

				z80_addhvc[(cvar << 16) | (var1 << 8) | var2] = ((unsigned char)result << 8) | myf;

				myf = 0;
				result = var2 - var1 - cvar;

				if(result & 65280)
					myf |= 1;

				bit4 = (var2 & 15) - (var1 & 15) - cvar;
				if(bit4 & 65280)
					myf |= 16;

				if((char)(lsv2 - lsv1 - cvar) != lsv2 - lsv1 - cvar)
					myf |= 4;

				z80_subhvc[(cvar << 16) | (var2 << 8) | var1] = ((unsigned char)result << 8) | myf;
			}
}

void z80_close(void)
{
	free(writebanks);
	free(readbanks);
}

void z80_setup(void)
{
	int count, pcount;

	count = 256;
	while(count--)
	{
		pcount = 0;
		if(count & 1)
			pcount++;
		if(count & 2)
			pcount++;
		if(count & 4)
			pcount++;
		if(count & 8)
			pcount++;
		if(count & 16)
			pcount++;
		if(count & 32)
			pcount++;
		if(count & 64)
			pcount++;
		if(count & 128)
			pcount++;

		pcount = ((~pcount) & 1) << 2;

		if(!count)
			pcount |= 64;

		z80_sz53p[count] = (count & 168) | pcount;
	}

	create_hvc_lists();

	z80_jpconds[0]	= 128;
	z80_jpconds[1]	= 64;
	z80_jpconds[2]	= 4;
	z80_jpconds[3]	= 1;

	z80_bmasks[0] = 1;
	z80_bmasks[1] = 2;
	z80_bmasks[2] = 4;
	z80_bmasks[3] = 8;
	z80_bmasks[4] = 16;
	z80_bmasks[5] = 32;
	z80_bmasks[6] = 64;
	z80_bmasks[7] = 128;

	z80_rstlist[0] = 0x00;
	z80_rstlist[1] = 0x08;
	z80_rstlist[2] = 0x10;
	z80_rstlist[3] = 0x18;
	z80_rstlist[4] = 0x20;
	z80_rstlist[5] = 0x28;
	z80_rstlist[6] = 0x30;
	z80_rstlist[7] = 0x38;

	readbanks = malloc(sizeof(unsigned char *)*8);
	writebanks = malloc(sizeof(unsigned char *)*8);
}