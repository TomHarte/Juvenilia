#include <allegro.h>
#include <stdlib.h>
#include "m6502.h"
#include "6502bitwise.h"
#include "6502addr.h"
#include "6502shift.h"
#include "6502misc.h"
#include "6502branch.h"
#include "6502arith.h"

M6502_INSTRUCTION *m6502_inst;
unsigned char m6502_x, m6502_y, m6502_a, m6502_f, m6502_sp, *sztable, m6502_done, *m6502_maincyc;
unsigned short m6502_pc, m6502_addr;
int m6502_tstates, m6502_breakpoint;
void (* real_p_mem)(unsigned char data);
void (* m6502_callout)(int statenum);
void (* m6502_exitfunc)(void);

M6502_INSTRUCTION norm_inst[] =
{
	{d_none, g_none, m6502_brk, p_none},		{d_indrx, g_mem, m6502_ora, p_none},		{d_none, g_none, m6502_nop, p_none},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_none, m6502_nop, p_none},		{d_zero, g_mem, m6502_ora, p_none},		{d_zero, g_mem, m6502_asl, p_mem},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_f, m6502_push, p_mem},		{d_none, g_immd, m6502_ora, p_none},		{d_none, g_a, m6502_asl, p_a},			{d_none, g_none, m6502_nop, p_none},
	{d_none, g_none, m6502_nop, p_none},		{d_abs, g_mem, m6502_ora, p_none},		{d_abs, g_mem, m6502_asl, p_mem},		{d_none, g_none, m6502_nop, p_none},

	{d_none, g_b7, m6502_bnset, p_none},		{d_indry, g_mem, m6502_ora, p_none},		{d_none, g_none, m6502_nop, p_none},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_none, m6502_nop, p_none},		{d_zerox, g_mem, m6502_ora, p_none},		{d_zerox, g_mem, m6502_asl, p_mem},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_b0, m6502_clrf, p_none},		{d_absy, g_mem, m6502_ora, p_none},		{d_none, g_none, m6502_nop, p_none},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_none, m6502_nop, p_none},		{d_absx, g_mem, m6502_ora, p_none},		{d_absx, g_mem, m6502_asl, p_mem},		{d_none, g_none, m6502_nop, p_none},

	//0x20

	{d_abs, g_none, m6502_jsr, p_none},		{d_indrx, g_mem, m6502_and, p_none},		{d_none, g_none, m6502_nop, p_none},		{d_none, g_none, m6502_nop, p_none},
	{d_zero, g_mem, m6502_bit, p_none},		{d_zero, g_mem, m6502_and, p_none},		{d_zero, g_mem, m6502_rol, p_mem},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_none, m6502_pop, p_f},		{d_none, g_immd, m6502_and, p_none},		{d_none, g_a, m6502_rol, p_a},			{d_none, g_none, m6502_nop, p_none},
	{d_abs, g_mem, m6502_bit, p_none},		{d_abs, g_mem, m6502_and, p_none},		{d_abs, g_mem, m6502_rol, p_mem},		{d_none, g_none, m6502_nop, p_none},

	{d_none, g_b7, m6502_bset, p_none},		{d_indry, g_mem, m6502_and, p_none},		{d_none, g_none, m6502_nop, p_none},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_none, m6502_nop, p_none},		{d_zerox, g_mem, m6502_and, p_none},		{d_zerox, g_mem, m6502_rol, p_mem},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_b0, m6502_setf, p_none},		{d_absy, g_mem, m6502_and, p_none},		{d_none, g_none, m6502_nop, p_none},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_none, m6502_nop, p_none},		{d_absx, g_mem, m6502_and, p_none},		{d_absx, g_mem, m6502_rol, p_mem},		{d_none, g_none, m6502_nop, p_none},

	//0x40
        
	{d_none, g_none, m6502_rti, p_none},		{d_indrx, g_mem, m6502_eor, p_none},		{d_none, g_none, m6502_nop, p_none},		{d_none, g_none, m6502_nop, p_none},
	{d_zero, g_mem, m6502_bit, p_none},		{d_zero, g_mem, m6502_eor, p_none},		{d_zero, g_mem, m6502_lsr, p_mem},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_a, m6502_push, p_mem},		{d_none, g_immd, m6502_eor, p_none},		{d_none, g_a, m6502_lsr, p_a},			{d_none, g_none, m6502_nop, p_none},
	{d_abs, g_none, m6502_jmp, p_none},		{d_abs, g_mem, m6502_eor, p_none},		{d_abs, g_mem, m6502_lsr, p_mem},		{d_none, g_none, m6502_nop, p_none},

	{d_none, g_b6, m6502_bnset, p_none},		{d_indry, g_mem, m6502_eor, p_none},		{d_none, g_none, m6502_nop, p_none},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_none, m6502_nop, p_none},		{d_zerox, g_mem, m6502_eor, p_none},		{d_zerox, g_mem, m6502_lsr, p_mem},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_b2, m6502_clrf, p_none},		{d_absy, g_mem, m6502_eor, p_none},		{d_none, g_none, m6502_nop, p_none},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_none, m6502_nop, p_none},		{d_absx, g_mem, m6502_eor, p_none},		{d_absx, g_mem, m6502_lsr, p_mem},		{d_none, g_none, m6502_nop, p_none},

	//0x60
        
	{d_none, g_none, m6502_rts, p_none},		{d_indrx, g_mem, m6502_adc, p_none},		{d_none, g_none, m6502_nop, p_none},		{d_none, g_none, m6502_nop, p_none},
	{d_zero, g_mem, m6502_bit, p_none},		{d_zero, g_mem, m6502_adc, p_none},		{d_zero, g_mem, m6502_ror, p_mem},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_none, m6502_pop, p_a},		{d_none, g_immd, m6502_adc, p_none},		{d_none, g_a, m6502_ror, p_a},			{d_none, g_none, m6502_nop, p_none},
	{d_indr, g_none, m6502_jmp, p_none},		{d_abs, g_mem, m6502_adc, p_none},		{d_abs, g_mem, m6502_ror, p_mem},		{d_none, g_none, m6502_nop, p_none},

	{d_none, g_b6, m6502_bset, p_none},		{d_indry, g_mem, m6502_adc, p_none},		{d_none, g_none, m6502_nop, p_none},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_none, m6502_nop, p_none},		{d_zerox, g_mem, m6502_adc, p_none},		{d_zerox, g_mem, m6502_ror, p_mem},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_b2, m6502_setf, p_none},		{d_absy, g_mem, m6502_adc, p_none},		{d_none, g_none, m6502_nop, p_none},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_none, m6502_nop, p_none},		{d_absx, g_mem, m6502_adc, p_none},		{d_absx, g_mem, m6502_ror, p_mem},		{d_none, g_none, m6502_nop, p_none},

	//0x80
        
	{d_none, g_none, m6502_nop, p_none},		{d_indrx, g_a, m6502_store, p_mem},		{d_none, g_none, m6502_nop, p_none},		{d_none, g_none, m6502_nop, p_none},
	{d_zero, g_y, m6502_store, p_mem},		{d_zero, g_a, m6502_store, p_mem},		{d_zero, g_x, m6502_store, p_mem},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_y, m6502_dec, p_y},			{d_none, g_none, m6502_nop, p_none},		{d_none, g_x, m6502_setstore, p_a},		{d_none, g_none, m6502_nop, p_none},
	{d_abs, g_y, m6502_store, p_mem},		{d_abs, g_a, m6502_store, p_mem},		{d_abs, g_x, m6502_store, p_mem},		{d_none, g_none, m6502_nop, p_none},

	{d_none, g_b0, m6502_bnset, p_none},		{d_indry, g_a, m6502_store, p_mem},		{d_none, g_none, m6502_nop, p_none},		{d_none, g_none, m6502_nop, p_none},
	{d_zerox, g_y, m6502_store, p_mem},		{d_zerox, g_a, m6502_store, p_mem},		{d_zeroy, g_x, m6502_store, p_mem},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_y, m6502_setstore, p_a},		{d_absy, g_a, m6502_store, p_mem},		{d_none, g_x, m6502_store, p_sp},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_none, m6502_nop, p_none},		{d_absx, g_a, m6502_store, p_mem},		{d_none, g_none, m6502_nop, p_none},		{d_none, g_none, m6502_nop, p_none},

	//0xa0
        
	{d_none, g_immd, m6502_store, p_y},		{d_indrx, g_mem, m6502_store, p_a},		{d_none, g_immd, m6502_store, p_x},		{d_none, g_none, m6502_nop, p_none},
	{d_zero, g_mem, m6502_store, p_y},		{d_zero, g_mem, m6502_store, p_a},		{d_zero, g_mem, m6502_store, p_x},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_a, m6502_setstore, p_y},		{d_none, g_immd, m6502_store, p_a},		{d_none, g_a, m6502_setstore, p_x},		{d_none, g_none, m6502_nop, p_none},
	{d_abs, g_mem, m6502_store, p_y},		{d_abs, g_mem, m6502_store, p_a},		{d_abs, g_mem, m6502_store, p_x},		{d_none, g_none, m6502_nop, p_none},

	{d_none, g_b0, m6502_bset, p_none},		{d_indry, g_mem, m6502_store, p_a},		{d_none, g_none, m6502_nop, p_none},		{d_none, g_none, m6502_nop, p_none},
	{d_zerox, g_mem, m6502_store, p_y},		{d_zerox, g_mem, m6502_store, p_y},		{d_zeroy, g_mem, m6502_store, p_x},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_b6, m6502_clrf, p_none},		{d_absy, g_mem, m6502_store, p_a},		{d_none, g_sp, m6502_store, p_x},		{d_none, g_none, m6502_nop, p_none},
	{d_absx, g_mem, m6502_store, p_y},		{d_absx, g_mem, m6502_store, p_a},		{d_absy, g_mem, m6502_store, p_x},		{d_none, g_none, m6502_nop, p_none},

	//0xc0
        
	{d_none, g_immd, m6502_cpy, p_none},		{d_indrx, g_mem, m6502_cmp, p_none},		{d_none, g_none, m6502_nop, p_none},		{d_none, g_none, m6502_nop, p_none},
	{d_zero, g_mem, m6502_cpy, p_none},		{d_zero, g_mem, m6502_cmp, p_none},		{d_zero, g_mem, m6502_dec, p_mem},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_y, m6502_inc, p_y},			{d_none, g_immd, m6502_cmp, p_none},		{d_none, g_x, m6502_dec, p_x},			{d_none, g_none, m6502_nop, p_none},
	{d_abs, g_mem, m6502_cpy, p_none},		{d_abs, g_mem, m6502_cmp, p_none},		{d_abs, g_mem, m6502_dec, p_mem},		{d_none, g_none, m6502_nop, p_none},

	{d_none, g_b1, m6502_bnset, p_none},		{d_indry, g_mem, m6502_cmp, p_none},		{d_none, g_none, m6502_nop, p_none},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_none, m6502_nop, p_none},		{d_zerox, g_mem, m6502_cmp, p_none},		{d_zerox, g_mem, m6502_dec, p_mem},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_b3, m6502_clrf, p_none},		{d_absy, g_mem, m6502_cmp, p_none},		{d_none, g_none, m6502_nop, p_none},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_none, m6502_nop, p_none},		{d_absx, g_mem, m6502_cmp, p_none},		{d_absx, g_mem, m6502_dec, p_mem},		{d_none, g_none, m6502_nop, p_none},

	//0xe0
        
	{d_none, g_immd, m6502_cpx, p_none},		{d_indrx, g_mem, m6502_sbc, p_none},		{d_none, g_none, m6502_nop, p_none},		{d_none, g_none, m6502_nop, p_none},
	{d_zero, g_mem, m6502_cpx, p_none},		{d_zero, g_mem, m6502_sbc, p_none},		{d_zero, g_mem, m6502_inc, p_mem},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_x, m6502_inc, p_x},			{d_none, g_immd, m6502_sbc, p_none},		{d_none, g_none, m6502_nop, p_none},		{d_none, g_none, m6502_nop, p_none},
	{d_abs, g_mem, m6502_cpx, p_none},		{d_abs, g_mem, m6502_sbc, p_none},		{d_abs, g_mem, m6502_inc, p_mem},		{d_none, g_none, m6502_nop, p_none},

	{d_none, g_b1, m6502_bset, p_none},		{d_indry, g_mem, m6502_sbc, p_none},		{d_none, g_none, m6502_nop, p_none},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_none, m6502_nop, p_none},		{d_zerox, g_mem, m6502_sbc, p_none},		{d_zerox, g_mem, m6502_inc, p_mem},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_b3, m6502_setf, p_none},		{d_absy, g_mem, m6502_sbc, p_none},		{d_none, g_none, m6502_nop, p_none},		{d_none, g_none, m6502_nop, p_none},
	{d_none, g_none, m6502_nop, p_none},		{d_absx, g_mem, m6502_sbc, p_none},		{d_absx, g_mem, m6502_inc, p_mem},		{d_none, g_none, m6502_nop, p_none},
};

unsigned char m6502_norm_cyc[] =
{
	7,  6,  0,  0,  0,  3,  5,  0,  3,  2,  2,  0,  0,  4,  6,  0,
	2,  5,  0,  0,  0,  4,  6,  0,  2,  4,  0,  0,  0,  4,  7,  0,
	6,  6,  0,  0,  3,  3,  5,  0,  4,  2,  2,  0,  4,  4,  6,  0,
	2,  5,  0,  0,  0,  4,  6,  0,  2,  4,  0,  0,  0,  4,  7,  0,
	6,  6,  0,  0,  0,  3,  5,  0,  3,  2,  2,  0,  3,  4,  6,  0,
	2,  5,  0,  0,  0,  4,  6,  0,  2,  4,  0,  0,  0,  4,  7,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

void m6502_end(void)
{
	free(sztable);
}

void m6502_setup(void)
{
	int c;

	sztable = malloc(256);
	c = 256;
	while(c--)
	{
		sztable[c] = 0;

		if(!c)
			sztable[c] |= 2;

		if(c&128)
			sztable[c] |= 128;
	}
}

void m6502_hard_reset(void)
{
	m6502_x =
	m6502_y =
	m6502_a =
	m6502_f =
	m6502_sp =
	m6502_pc = 0;
}

void go_m6502(void)
{
	unsigned char instr;

	m6502_done = 0;

	while(!m6502_done)
	{
		if(m6502_tstates >= m6502_breakpoint)
                	m6502_callout(m6502_tstates);
                        
		instr = m6502_readmemory(pc);
        	m6502_pc++;
                
		m6502_inst[instr].decode_addr();
		m6502_inst[instr].putfunc(m6502_inst[instr].operate(m6502_inst[instr].getfunc()));
		m6502_tstates += m6502_maincyc[instr];
                        
		if(key[KEY_F12])
		{
			m6502_done = 1;
		}

		if(key[KEY_F11])
		{
			m6502_hard_reset();
		}

	}

	m6502_exitfunc();
}

