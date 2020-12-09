#include <allegro.h>
#include "z80.h"

//Z80 registers
Z80_REGISTERPAIR	af,		bc,		de,		hl,		ix,		iy,		sp;
Z80_REGISTERPAIR	afd,	bcd,	ded,	hld;
unsigned short	pc;
unsigned char	i,		r;

//interrupt mode, memory and index register variables
unsigned char	imode,		**writebanks,	**readbanks,		*mempool;
Z80_REGISTERPAIR	idx;

//result, rst, bitmask and condition lookup tables
unsigned char	z80_sz53p[256], z80_bmasks[8];
unsigned short	z80_addhvc[131072], z80_subhvc[131072], z80_rstlist[8];
unsigned char	z80_jpconds[4];

//done flag (set to leave), and tstates counter variables
char done;

//memory input/output vars
Z80_REGISTERPAIR z80_outword, z80_outaddr;
unsigned char z80_outflag;

void (* z80_port_out)(unsigned short portnum, unsigned char data);
void (* z80_port_in)(unsigned short portnum, unsigned char *data);
void (* z80_callout)(int statenum);
void (* z80_writeout)(void);
void (* z80_exitfunc)(void);
void (* z80_ei_callback)(void);

int z80_tstates, z80_breakpoint;

unsigned char *z80_maincyc, *z80_cbcyc, *z80_ddfdcyc, *z80_ddfdcbcyc, *z80_edcyc, *z80_additives, z80_imchange, *z80_zones, *z80_bankzones;
Z80_INSTRUCTION *z80_baseinst, *z80_cbinst, *z80_ddfdinst, *z80_ddfdcbinst, *z80_edinst;

void z80_scf(void *var1, void *var2) //NULL, NULL
{
	af.bytes.low = (af.bytes.low&196) | 1 | (af.bytes.high&40);
}

void z80_ccf(void *var1, void *var2) //NULL, NULL
{
	if(af.bytes.low &1)
		af.bytes.low = (af.bytes.low&196) | 16 | (af.bytes.high&40);
	else
		af.bytes.low = (af.bytes.low&196) | 1 | (af.bytes.high&40);
}

void z80_cpl(void *var1, void *var2) //NULL, NULL
{
	af.bytes.high = ~af.bytes.high;
	af.bytes.low = (af.bytes.low&197) | 18 | (af.bytes.high&40);
}

void z80_nop(void *var1, void *var2) //NULL, NULL
{
}

void z80_di(void *var1, void *var2) //NULL, NULL
{
	imode &= 31;
}

void z80_ei(void *var1, void *var2) //NULL, NULL
{
	z80_imchange = 1;
}

void z80_daa(void *var1, void *var2) //NULL, NULL
{
	unsigned char incr=0, carry;
	carry = af.bytes.low & 1;

	if((af.bytes.low & 0x10) || ((af.bytes.high & 0x0f) > 9))
		incr=6;

	if((af.bytes.low&1) || (af.bytes.high >> 4) > 9)
		incr |= 0x60;

	if(af.bytes.low&2)
		z80_sub8(&incr, NULL);
	else
	{
		if(af.bytes.high > 0x90 && (af.bytes.high & 0x0f) > 9)
			incr |= 0x60;

		z80_add8(&incr, NULL);
	}

	af.bytes.low = ((af.bytes.low | carry) & 0xfb) | (z80_sz53p[af.bytes.high] & 4);
}

void z80_outna(void *var1, void *var2) //NULL, NULL
{
	Z80_REGISTERPAIR portnum;

	portnum.bytes.low = readmemory(pc);
	portnum.bytes.high = af.bytes.high;
	pc++;

	z80_port_out(portnum.full, af.bytes.high);
}

void z80_inna(void *var1, void *var2) //NULL, NULL
{
	Z80_REGISTERPAIR portnum;

	portnum.bytes.low = readmemory(pc);
	portnum.bytes.high = af.bytes.high;
	pc++;

	z80_port_in(portnum.full, &af.bytes.high);
}

void z80_fail(void *var1, void *var2) //NULL, NULL
{
	unsigned short c;

	fprintf(stderr, "Unkown at %04x : \n", pc-1);

	for(c = pc-5; c < pc+5; c++)
		fprintf(stderr, "%04x : %02x\n", c, readmemory(c));
	done = 1;
}

void z80_halt(void *var1, void *var2)
{
	pc--;
}


void z80_enable_interrupts(void)
{
	imode |= 192;
	if(z80_ei_callback)
		z80_ei_callback();
}

void go_z80(void)
{
	unsigned char instr;

	done = 0;
	z80_outflag = 0;

	while(!done)
	{
		if(z80_tstates >= z80_breakpoint)
                	z80_callout(z80_tstates);
                        
		instr = readmemory(pc);
        	pc++;
                
		inc_r();
        	z80_baseinst[instr].func(z80_baseinst[instr].var1, z80_baseinst[instr].var2);
		z80_tstates += z80_maincyc[instr];

		if(z80_outflag)
			z80_writeout();
                
                        
                z80_imchange <<= 1;
		if(z80_imchange&4)
			z80_enable_interrupts();
                        
		if(key[KEY_F12] || (key[KEY_ESC] && key[KEY_LSHIFT]|key[KEY_RSHIFT]))
		{
			done = 1;
		}

		if(key[KEY_F11])
		{
			hard_reset();
		}

	}

	z80_exitfunc();
}

void z80_do_interrupt(void)
{
	Z80_REGISTERPAIR call;
	unsigned short addr;

	if(readmemory(pc) == 0x76)
		pc++;

	if(imode&128) //interrupts on
	{
		imode &= 31; //switch interrupts off
		switch(imode)
		{
			case 0 :
			case 1 :
				//call 0x38
				z80_rst(RST38h, NULL);
			break;

			case 2 :
				//read short from (i * 256)+255, call it
				addr = (i << 8) | 255;
				call.bytes.low = readmemory(addr);
				addr++;
				call.bytes.high = readmemory(addr);
				z80_rst(&call.full, NULL);
			break;
		}
                z80_writeout();
	}
}

