#include "z80.h"

void z80_go_cb(void *var1, void *var2)
{
	unsigned char instr;

	inc_r();

	instr = readmemory(pc);
	pc++;
	z80_tstates += z80_cbcyc[instr];
	z80_cbinst[instr].func(z80_cbinst[instr].var1, z80_cbinst[instr].var2);
}

