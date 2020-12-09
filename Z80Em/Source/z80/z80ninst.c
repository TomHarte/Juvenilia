#include "z80.h"
#include "z80ld.h"
#include "z80stackjp.h"
#include "z80arith.h"
#include "z80bitwise.h"
#include "z80shift.h"
#include "z80bit.h"
#include "z80misc.h"

Z80_INSTRUCTION z80_norm_baseinst[] =
{
	{z80_nop, NULL, NULL},		{z80_ld16nn, regB, regC},	{z80_ldrf8, regBC, regA},	{z80_inc16, regBC, NULL},
	{z80_inc8, regB, NULL},		{z80_dec8, regB, NULL},		{z80_ld8n, regB, NULL},		{z80_rlca, NULL, NULL},
	{z80_exafafd, NULL, NULL},	{z80_add16, &hl, &bc},		{z80_ld8rf, regA, regBC},	{z80_dec16, regBC, NULL},
	{z80_inc8, regC, NULL},		{z80_dec8, regC, NULL},		{z80_ld8n, regC, NULL},		{z80_rrca, NULL, NULL},

	//0x10

	{z80_djnz, NULL, NULL},		{z80_ld16nn, regD, regE},	{z80_ldrf8, regDE, regA},	{z80_inc16, regDE, NULL},
	{z80_inc8, regD, NULL},		{z80_dec8, regD, NULL},		{z80_ld8n, regD, NULL},		{z80_rla, NULL, NULL},
	{z80_jrn, NULL, NULL},		{z80_add16, &hl, &de},		{z80_ld8rf, regA, regDE},	{z80_dec16, regDE, NULL},
	{z80_inc8, regE, NULL},		{z80_dec8, regE, NULL},		{z80_ld8n, regE, NULL},		{z80_rra, NULL, NULL},

	//0x20

	{z80_jrncond, Z, NULL},		{z80_ld16nn, regH, regL},	{z80_ldrfnn16, regH, regL},	{z80_inc16, regHL, NULL},
	{z80_inc8, regH, NULL},		{z80_dec8, regH, NULL},		{z80_ld8n, regH, NULL},		{z80_daa, NULL, NULL},
	{z80_jrcond, Z, NULL},		{z80_add16, &hl, &hl},		{z80_ld16rfnn, regH, regL},	{z80_dec16, regHL, NULL},
	{z80_inc8, regL, NULL},		{z80_dec8, regL, NULL},		{z80_ld8n, regL, NULL},		{z80_cpl, NULL, NULL},

	//0x30

	{z80_jrncond, C, NULL},		{z80_ld16nn, regS, regP},	{z80_ldrfnn8, regA, NULL},	{z80_inc16, regSP, NULL},
	{z80_inc8rf, regHL, NULL},	{z80_dec8rf, regHL, NULL},	{z80_ldrfn, regHL, NULL},	{z80_scf, NULL, NULL},
	{z80_jrcond, C, NULL},		{z80_add16, &hl, &sp},		{z80_ld8rfnn, regA, NULL},	{z80_dec16, regSP, NULL},
	{z80_inc8, regA, NULL},		{z80_dec8, regA, NULL},		{z80_ld8n, regA, NULL},		{z80_ccf, NULL, NULL},

	//0x40

	{z80_ld8, regB, regB},		{z80_ld8, regB, regC},		{z80_ld8, regB, regD},		{z80_ld8, regB, regE},
	{z80_ld8, regB, regH},		{z80_ld8, regB, regL},		{z80_ld8rf, regB, regHL},	{z80_ld8, regB, regA},
	{z80_ld8, regC, regB},		{z80_ld8, regC, regC},		{z80_ld8, regC, regD},		{z80_ld8, regC, regE},
	{z80_ld8, regC, regH},		{z80_ld8, regC, regL},		{z80_ld8rf, regC, regHL},	{z80_ld8, regC, regA},

	//0x50
        
       	{z80_ld8, regD, regB},		{z80_ld8, regD, regC},		{z80_ld8, regD, regD},		{z80_ld8, regD, regE},
	{z80_ld8, regD, regH},		{z80_ld8, regD, regL},		{z80_ld8rf, regD, regHL},	{z80_ld8, regD, regA},
	{z80_ld8, regE, regB},		{z80_ld8, regE, regC},		{z80_ld8, regE, regD},		{z80_ld8, regE, regE},
	{z80_ld8, regE, regH},		{z80_ld8, regE, regL},		{z80_ld8rf, regE, regHL},	{z80_ld8, regE, regA},

	//0x60

	{z80_ld8, regH, regB},		{z80_ld8, regH, regC},		{z80_ld8, regH, regD},		{z80_ld8, regH, regE},
	{z80_ld8, regH, regH},		{z80_ld8, regH, regL},		{z80_ld8rf, regH, regHL},	{z80_ld8, regH, regA},
	{z80_ld8, regL, regB},		{z80_ld8, regL, regC},		{z80_ld8, regL, regD},		{z80_ld8, regL, regE},
	{z80_ld8, regL, regH},		{z80_ld8, regL, regL},		{z80_ld8rf, regL, regHL},	{z80_ld8, regL, regA},

	//0x70

	{z80_ldrf8, regHL, regB},	{z80_ldrf8, regHL, regC},	{z80_ldrf8, regHL, regD},	{z80_ldrf8, regHL, regE},
	{z80_ldrf8, regHL, regH},	{z80_ldrf8, regHL, regL},	{z80_halt, NULL, NULL},		{z80_ldrf8, regHL, regA},
	{z80_ld8, regA, regB},		{z80_ld8, regA, regC},		{z80_ld8, regA, regD},		{z80_ld8, regA, regE},
	{z80_ld8, regA, regH},		{z80_ld8, regA, regL},		{z80_ld8rf, regA, regHL},	{z80_ld8, regA, regA},

	//0x80

	{z80_add8, regB, NULL},		{z80_add8, regC, NULL},		{z80_add8, regD, NULL},		{z80_add8, regE, NULL},
	{z80_add8, regH, NULL},		{z80_add8, regL, NULL},		{z80_add8rf, regHL, NULL},	{z80_add8, regA, NULL},
	{z80_adc8, regB, NULL},		{z80_adc8, regC, NULL},		{z80_adc8, regD, NULL},		{z80_adc8, regE, NULL},
	{z80_adc8, regH, NULL},		{z80_adc8, regL, NULL},		{z80_adc8rf, regHL, NULL},	{z80_adc8, regA, NULL},
	
	//0x90

	{z80_sub8, regB, NULL},		{z80_sub8, regC, NULL},		{z80_sub8, regD, NULL},		{z80_sub8, regE, NULL},
	{z80_sub8, regH, NULL},		{z80_sub8, regL, NULL},		{z80_sub8rf, regHL, NULL},	{z80_sub8, regA, NULL},
	{z80_sbc8, regB, NULL},		{z80_sbc8, regC, NULL},		{z80_sbc8, regD, NULL},		{z80_sbc8, regE, NULL},
	{z80_sbc8, regH, NULL},		{z80_sbc8, regL, NULL},		{z80_sbc8rf, regHL, NULL},	{z80_sbc8, regA, NULL},

	//0xa0

	{z80_and8, regB, NULL},		{z80_and8, regC, NULL},		{z80_and8, regD, NULL},		{z80_and8, regE, NULL},
	{z80_and8, regH, NULL},		{z80_and8, regL, NULL},		{z80_and8rf, regHL, NULL},	{z80_and8, regA, NULL},
	{z80_xor8, regB, NULL},		{z80_xor8, regC, NULL},		{z80_xor8, regD, NULL},		{z80_xor8, regE, NULL},
	{z80_xor8, regH, NULL},		{z80_xor8, regL, NULL},		{z80_xor8rf, regHL, NULL},	{z80_xor8, regA, NULL},

	//0xb0

	{z80_or8, regB, NULL},		{z80_or8, regC, NULL},		{z80_or8, regD, NULL},		{z80_or8, regE, NULL},
	{z80_or8, regH, NULL},		{z80_or8, regL, NULL},		{z80_or8rf, regHL, NULL},	{z80_or8, regA, NULL},
	{z80_cp, regB, NULL},		{z80_cp, regC, NULL},		{z80_cp, regD, NULL},		{z80_cp, regE, NULL},
	{z80_cp, regH, NULL},		{z80_cp, regL, NULL},		{z80_cprf, regHL, NULL},	{z80_cp, regA, NULL},

	//0xc0

	{z80_retncond, Z, NULL},	{z80_pop, regB, regC},		{z80_jpncond, Z, NULL},		{z80_jpnn, NULL, NULL},
	{z80_callncond, Z, NULL},	{z80_push, regB, regC},		{z80_add8n, NULL, NULL},	{z80_rst, RST00h, NULL},
	{z80_retcond, Z, NULL},		{z80_ret, NULL, NULL},		{z80_jpcond, Z, NULL},		{z80_go_cb, NULL, NULL},
	{z80_callcond, Z, NULL},	{z80_call, NULL, NULL},		{z80_adc8n, NULL, NULL},	{z80_rst, RST08h, NULL},

	//0xd0

	{z80_retncond, C, NULL},	{z80_pop, regD, regE},		{z80_jpncond, C, NULL},		{z80_outna, NULL, NULL},
	{z80_callncond, C, NULL},	{z80_push, regD, regE},		{z80_sub8n, NULL, NULL},	{z80_rst, RST10h, NULL},
	{z80_retcond, C, NULL},		{z80_exx, NULL, NULL},		{z80_jpcond, C, NULL},		{z80_inna, NULL, NULL},
	{z80_callcond, C, NULL},	{z80_go_ddfd, &ix, NULL},	{z80_sbc8n, NULL, NULL},	{z80_rst, RST18h, NULL},

	//0xe0

	{z80_retncond, PA, NULL},	{z80_pop, regH, regL},		{z80_jpncond, PA, NULL},	{z80_exrfsp16, regH, regL},
	{z80_callncond, PA, NULL},	{z80_push, regH, regL},		{z80_and8n, NULL, NULL},	{z80_rst, RST20h, NULL},
	{z80_retcond, PA, NULL},	{z80_jp16, regHL, NULL},	{z80_jpcond, PA, NULL},		{z80_exdehl, NULL, NULL},
	{z80_callcond, PA, NULL},	{z80_go_ed, NULL, NULL},	{z80_xor8n, NULL, NULL},	{z80_rst, RST28h, NULL},

	//0xf0

	{z80_retncond, M, NULL},	{z80_pop, regA, regF},		{z80_jpncond, M, NULL},		{z80_di, NULL, NULL},
	{z80_callncond, M, NULL},	{z80_push, regA, regF},		{z80_or8n, NULL, NULL},		{z80_rst, RST30h, NULL},
	{z80_retcond, M, NULL},		{z80_ld16, regSP, regHL},	{z80_jpcond, M, NULL},		{z80_ei, NULL, NULL},
	{z80_callcond, M, NULL},	{z80_go_ddfd, &iy, NULL},	{z80_cpn, NULL, NULL},		{z80_rst, RST38h, NULL}
};

Z80_INSTRUCTION z80_norm_cbinst[] =
{
	{z80_rlc, regB, NULL},		{z80_rlc, regC, NULL},		{z80_rlc, regD, NULL},		{z80_rlc, regE, NULL},
	{z80_rlc, regH, NULL},		{z80_rlc, regL, NULL},		{z80_rlcrf, regHL, NULL},	{z80_rlc, regA, NULL},
	{z80_rrc, regB, NULL},		{z80_rrc, regC, NULL},		{z80_rrc, regD, NULL},		{z80_rrc, regE, NULL},
	{z80_rrc, regH, NULL},		{z80_rrc, regL, NULL},		{z80_rrcrf, regHL, NULL},	{z80_rrc, regA, NULL},

	//0x10

	{z80_rl, regB, NULL},		{z80_rl, regC, NULL},		{z80_rl, regD, NULL},		{z80_rl, regE, NULL},
	{z80_rl, regH, NULL},		{z80_rl, regL, NULL},		{z80_rlrf, regHL, NULL},	{z80_rl, regA, NULL},
	{z80_rr, regB, NULL},		{z80_rr, regC, NULL},		{z80_rr, regD, NULL},		{z80_rr, regE, NULL},
	{z80_rr, regH, NULL},		{z80_rr, regL, NULL},		{z80_rrrf, regHL, NULL},	{z80_rr, regA, NULL},

	//0x20

	{z80_sla, regB, NULL},		{z80_sla, regC, NULL},		{z80_sla, regD, NULL},		{z80_sla, regE, NULL},
	{z80_sla, regH, NULL},		{z80_sla, regL, NULL},		{z80_slarf, regHL, NULL},	{z80_sla, regA, NULL},
	{z80_sra, regB, NULL},		{z80_sra, regC, NULL},		{z80_sra, regD, NULL},		{z80_sra, regE, NULL},
	{z80_sra, regH, NULL},		{z80_sra, regL, NULL},		{z80_srarf, regHL, NULL},	{z80_sra, regA, NULL},

	//0x30

	{z80_sll, regB, NULL},		{z80_sll, regC, NULL},		{z80_sll, regD, NULL},		{z80_srl, regE, NULL},
	{z80_sll, regH, NULL},		{z80_sll, regL, NULL},		{z80_sllrf, regHL, NULL},	{z80_sll, regA, NULL},
	{z80_srl, regB, NULL},		{z80_srl, regC, NULL},		{z80_srl, regD, NULL},		{z80_srl, regE, NULL},
	{z80_srl, regH, NULL},		{z80_srl, regL, NULL},		{z80_srlrf, regHL, NULL},	{z80_srl, regA, NULL},

	//0x40

	{z80_bit8, regB, BIT0},		{z80_bit8, regC, BIT0},		{z80_bit8, regD, BIT0},		{z80_bit8, regE, BIT0},
	{z80_bit8, regH, BIT0},		{z80_bit8, regL, BIT0},		{z80_bitrf, regHL, BIT0},	{z80_bit8, regA, BIT0},
	{z80_bit8, regB, BIT1},		{z80_bit8, regC, BIT1},		{z80_bit8, regD, BIT1},		{z80_bit8, regE, BIT1},
	{z80_bit8, regH, BIT1},		{z80_bit8, regL, BIT1},		{z80_bitrf, regHL, BIT1},	{z80_bit8, regA, BIT1},

	//0x50

	{z80_bit8, regB, BIT2},		{z80_bit8, regC, BIT2},		{z80_bit8, regD, BIT2},		{z80_bit8, regE, BIT2},
	{z80_bit8, regH, BIT2},		{z80_bit8, regL, BIT2},		{z80_bitrf, regHL, BIT2},	{z80_bit8, regA, BIT2},
	{z80_bit8, regB, BIT3},		{z80_bit8, regC, BIT3},		{z80_bit8, regD, BIT3},		{z80_bit8, regE, BIT3},
	{z80_bit8, regH, BIT3},		{z80_bit8, regL, BIT3},		{z80_bitrf, regHL, BIT3},	{z80_bit8, regA, BIT3},

	//0x60

	{z80_bit8, regB, BIT4},		{z80_bit8, regC, BIT4},		{z80_bit8, regD, BIT4},		{z80_bit8, regE, BIT4},
	{z80_bit8, regH, BIT4},		{z80_bit8, regL, BIT4},		{z80_bitrf, regHL, BIT4},	{z80_bit8, regA, BIT4},
	{z80_bit8, regB, BIT5},		{z80_bit8, regC, BIT5},		{z80_bit8, regD, BIT5},		{z80_bit8, regE, BIT5},
	{z80_bit8, regH, BIT5},		{z80_bit8, regL, BIT5},		{z80_bitrf, regHL, BIT5},	{z80_bit8, regA, BIT5},

	//0x70

	{z80_bit8, regB, BIT6},		{z80_bit8, regC, BIT6},		{z80_bit8, regD, BIT6},		{z80_bit8, regE, BIT6},
	{z80_bit8, regH, BIT6},		{z80_bit8, regL, BIT6},		{z80_bitrf, regHL, BIT6},	{z80_bit8, regA, BIT6},
	{z80_bit8, regB, BIT7},		{z80_bit8, regC, BIT7},		{z80_bit8, regD, BIT7},		{z80_bit8, regE, BIT7},
	{z80_bit8, regH, BIT7},		{z80_bit8, regL, BIT7},		{z80_bitrf, regHL, BIT7},	{z80_bit8, regA, BIT7},

	//0x80

	{z80_res8, regB, BIT0},		{z80_res8, regC, BIT0},		{z80_res8, regD, BIT0},		{z80_res8, regE, BIT0},
	{z80_res8, regH, BIT0},		{z80_res8, regL, BIT0},		{z80_resrf, regHL, BIT0},	{z80_res8, regA, BIT0},
	{z80_res8, regB, BIT1},		{z80_res8, regC, BIT1},		{z80_res8, regD, BIT1},		{z80_res8, regE, BIT1},
	{z80_res8, regH, BIT1},		{z80_res8, regL, BIT1},		{z80_resrf, regHL, BIT1},	{z80_res8, regA, BIT1},

	//0x90

	{z80_res8, regB, BIT2},		{z80_res8, regC, BIT2},		{z80_res8, regD, BIT2},		{z80_res8, regE, BIT2},
	{z80_res8, regH, BIT2},		{z80_res8, regL, BIT2},		{z80_resrf, regHL, BIT2},	{z80_res8, regA, BIT2},
	{z80_res8, regB, BIT3},		{z80_res8, regC, BIT3},		{z80_res8, regD, BIT3},		{z80_res8, regE, BIT3},
	{z80_res8, regH, BIT3},		{z80_res8, regL, BIT3},		{z80_resrf, regHL, BIT3},	{z80_res8, regA, BIT3},

	//0xa0

	{z80_res8, regB, BIT4},		{z80_res8, regC, BIT4},		{z80_res8, regD, BIT4},		{z80_res8, regE, BIT4},
	{z80_res8, regH, BIT4},		{z80_res8, regL, BIT4},		{z80_resrf, regHL, BIT4},	{z80_res8, regA, BIT4},
	{z80_res8, regB, BIT5},		{z80_res8, regC, BIT5},		{z80_res8, regD, BIT5},		{z80_res8, regE, BIT5},
	{z80_res8, regH, BIT5},		{z80_res8, regL, BIT5},		{z80_resrf, regHL, BIT5},	{z80_res8, regA, BIT5},

	//0xb0

	{z80_res8, regB, BIT6},		{z80_res8, regC, BIT6},		{z80_res8, regD, BIT6},		{z80_res8, regE, BIT6},
	{z80_res8, regH, BIT6},		{z80_res8, regL, BIT6},		{z80_resrf, regHL, BIT6},	{z80_res8, regA, BIT6},
	{z80_res8, regB, BIT7},		{z80_res8, regC, BIT7},		{z80_res8, regD, BIT7},		{z80_res8, regE, BIT7},
	{z80_res8, regH, BIT7},		{z80_res8, regL, BIT7},		{z80_resrf, regHL, BIT7},	{z80_res8, regA, BIT7},

	//0xc0

	{z80_set8, regB, BIT0},		{z80_set8, regC, BIT0},		{z80_set8, regD, BIT0},		{z80_set8, regE, BIT0},
	{z80_set8, regH, BIT0},		{z80_set8, regL, BIT0},		{z80_setrf, regHL, BIT0},	{z80_set8, regA, BIT0},
	{z80_set8, regB, BIT1},		{z80_set8, regC, BIT1},		{z80_set8, regD, BIT1},		{z80_set8, regE, BIT1},
	{z80_set8, regH, BIT1},		{z80_set8, regL, BIT1},		{z80_setrf, regHL, BIT1},	{z80_set8, regA, BIT1},

	//0xd0

	{z80_set8, regB, BIT2},		{z80_set8, regC, BIT2},		{z80_set8, regD, BIT2},		{z80_set8, regE, BIT2},
	{z80_set8, regH, BIT2},		{z80_set8, regL, BIT2},		{z80_setrf, regHL, BIT2},	{z80_set8, regA, BIT2},
	{z80_set8, regB, BIT3},		{z80_set8, regC, BIT3},		{z80_set8, regD, BIT3},		{z80_set8, regE, BIT3},
	{z80_set8, regH, BIT3},		{z80_set8, regL, BIT3},		{z80_setrf, regHL, BIT3},	{z80_set8, regA, BIT3},

	//0xe0

	{z80_set8, regB, BIT4},		{z80_set8, regC, BIT4},		{z80_set8, regD, BIT4},		{z80_set8, regE, BIT4},
	{z80_set8, regH, BIT4},		{z80_set8, regL, BIT4},		{z80_setrf, regHL, BIT4},	{z80_set8, regA, BIT4},
	{z80_set8, regB, BIT5},		{z80_set8, regC, BIT5},		{z80_set8, regD, BIT5},		{z80_set8, regE, BIT5},
	{z80_set8, regH, BIT5},		{z80_set8, regL, BIT5},		{z80_setrf, regHL, BIT5},	{z80_set8, regA, BIT5},

	//0xf0

	{z80_set8, regB, BIT6},		{z80_set8, regC, BIT6},		{z80_set8, regD, BIT6},		{z80_set8, regE, BIT6},
	{z80_set8, regH, BIT6},		{z80_set8, regL, BIT6},		{z80_setrf, regHL, BIT6},	{z80_set8, regA, BIT6},
	{z80_set8, regB, BIT7},		{z80_set8, regC, BIT7},		{z80_set8, regD, BIT7},		{z80_set8, regE, BIT7},
	{z80_set8, regH, BIT7},		{z80_set8, regL, BIT7},		{z80_setrf, regHL, BIT7},	{z80_set8, regA, BIT7},
};

Z80_INSTRUCTION z80_norm_ddfdcbinst[] =
{
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_rlc, NULL},	{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_rrc, NULL},	{z80_fail, NULL, NULL},

	//0x10

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_rl, NULL},	{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_rr, NULL},	{z80_fail, NULL, NULL},


	//0x20

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_sla, NULL},	{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_sra, NULL},	{z80_fail, NULL, NULL},

	//0x30

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_sll, NULL},	{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_srl, NULL},	{z80_fail, NULL, NULL},

	//0x40

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_bit8, BIT0},	{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_bit8, BIT1},	{z80_fail, NULL, NULL},

	//0x50

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_bit8, BIT2},	{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_bit8, BIT3},	{z80_fail, NULL, NULL},

	//0x60

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_bit8, BIT4},	{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_bit8, BIT5},	{z80_fail, NULL, NULL},

	//0x70

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_bit8, BIT6},	{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_bit8, BIT7},	{z80_fail, NULL, NULL},

	//0x80

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_res8, BIT0},	{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_res8, BIT1},	{z80_fail, NULL, NULL},

	//0x90

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_res8, BIT2},	{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_res8, BIT3},	{z80_fail, NULL, NULL},

	//0xa0

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_res8, BIT4},	{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_res8, BIT5},	{z80_fail, NULL, NULL},

	//0xb0

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_res8, BIT6},	{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_res8, BIT7},	{z80_fail, NULL, NULL},

	//0xc0

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_set8, BIT0},	{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_set8, BIT1},	{z80_fail, NULL, NULL},

	//0xd0

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_set8, BIT2},	{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_set8, BIT3},	{z80_fail, NULL, NULL},

	//0xe0

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_set8, BIT4},	{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_set8, BIT5},	{z80_fail, NULL, NULL},

	//0xf0

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_set8, BIT6},	{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_pd8, z80_set8, BIT7},	{z80_fail, NULL, NULL},
};

Z80_INSTRUCTION z80_norm_ddfdinst[] =
{
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_add16, &idx, &bc},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},

	//0x10

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_add16, &idx, &de},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},

	//0x20

	{z80_fail, NULL, NULL},		{z80_ld16nn, regIDh, regIDl},	{z80_ldrfnn16, regIDh, regIDl},	{z80_inc16, regID, NULL},
	{z80_inc8, regIDh, NULL},	{z80_dec8, regIDh, NULL},	{z80_ld8n, regIDh, NULL},	{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_add16, &idx, &idx},	{z80_ld16rfnn, regIDh, regIDl},	{z80_dec16, regID, NULL},
	{z80_inc8, regIDl, NULL},	{z80_dec8, regIDl, NULL},	{z80_ld8n, regIDl, NULL},	{z80_fail, NULL, NULL},

	//0x30

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_prfd8, regID, z80_inc8},	{z80_prfd8, regID, z80_dec8},	{z80_ldpdn, regID, NULL},	{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_add16, &idx, &sp},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},

	//0x40

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_ld8, regB, regIDh},	{z80_ld8, regB, regIDl},	{z80_ld8pd, regB, regID},	{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_ld8, regC, regIDh},	{z80_ld8, regC, regIDl},	{z80_ld8pd, regC, regID},	{z80_fail, NULL, NULL},

	//0x50

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_ld8, regD, regIDh},	{z80_ld8, regD, regIDl},	{z80_ld8pd, regD, regID},	{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_ld8, regD, regIDh},	{z80_ld8, regE, regIDl},	{z80_ld8pd, regE, regID},	{z80_fail, NULL, NULL},

	//0x60

	{z80_ld8, regIDh, regB},	{z80_ld8, regIDh, regC},	{z80_ld8, regIDh, regD},	{z80_ld8, regIDh, regE},
	{z80_ld8, regIDh, regIDh},	{z80_ld8, regIDh, regIDl},	{z80_ld8pd, regH, regID},	{z80_ld8, regIDh, regA},
	{z80_ld8, regIDl, regB},	{z80_ld8, regIDl, regC},	{z80_ld8, regIDl, regD},	{z80_ld8, regIDl, regE},
	{z80_ld8, regIDl, regIDh},	{z80_ld8, regIDl, regIDl},	{z80_ld8pd, regL, regID},	{z80_ld8, regIDl, regA},

	//0x70

	{z80_ldpd8, regID, regB},	{z80_ldpd8, regID, regC},	{z80_ldpd8, regID, regD},	{z80_ldpd8, regID, regE},
	{z80_ldpd8, regID, regH},	{z80_ldpd8, regID, regL},	{z80_fail, NULL, NULL},		{z80_ldpd8, regID, regA},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_ld8, regA, regIDh},	{z80_ld8, regA, regIDl},	{z80_ld8pd, regA, regID},	{z80_fail, NULL, NULL},

	//0x80

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_add8, regIDh, NULL},	{z80_add8, regIDl, NULL},	{z80_prfd8, regID, z80_add8},	{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_adc8, regIDh, NULL},	{z80_adc8, regIDl, NULL},	{z80_prfd8, regID, z80_adc8},	{z80_fail, NULL, NULL},

	//0x90

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_sub8, regIDh, NULL},	{z80_sub8, regIDl, NULL},	{z80_prfd8, regID, z80_sub8},	{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_sbc8, regIDh, NULL},	{z80_sbc8, regIDl, NULL},	{z80_prfd8, regID, z80_sbc8},	{z80_fail, NULL, NULL},

	//0xa0

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_and8, regIDh, NULL},	{z80_and8, regIDl, NULL},	{z80_prfd8, regID, z80_and8},	{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_xor8, regIDh, NULL},	{z80_xor8, regIDl, NULL},	{z80_prfd8, regID, z80_xor8},	{z80_fail, NULL, NULL},

	//0xb0

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_or8, regIDh, NULL},	{z80_or8, regIDl, NULL},	{z80_prfd8, regID, z80_or8},	{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_cp, regIDh, NULL},		{z80_cp, regIDl, NULL},		{z80_prfd8, regID, z80_cp},	{z80_fail, NULL, NULL},

	//0xc0

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_go_ddfd_cb, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},

	//0xd0

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},

	//0xe0

	{z80_fail, NULL, NULL},		{z80_pop, regIDh, regIDl},	{z80_fail, NULL, NULL},		{z80_exrfsp16, regIDh, regIDl},
	{z80_fail, NULL, NULL},		{z80_push, regIDh, regIDl},	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_jp16, regID, NULL},	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},

	//0xf0

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_ld16, regSP, regID},	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
};

Z80_INSTRUCTION z80_norm_edinst[] =
{
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},

	//0x10

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},

	//0x20

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},

	//0x30

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},

	//0x40

	{z80_in, regB, regBC},		{z80_out, regB, regBC},		{z80_sbc16, regB, regC},	{z80_ldrfnn16, regB, regC},
	{z80_neg, NULL, NULL},		{z80_ret, NULL, NULL},		{z80_im0, NULL, NULL},		{z80_ld8, &i, regA},
	{z80_in, regC, regBC},		{z80_out, regC, regBC},		{z80_adc16, &hl, &bc},		{z80_ld16rfnn, regB, regC},
	{z80_neg, NULL, NULL},		{z80_ret, NULL, NULL},		{z80_im0, NULL, NULL},		{z80_ld8, &r, regA},

	//0x50

	{z80_in, regD, regBC},		{z80_out, regD, regBC},		{z80_sbc16, regD, regE},	{z80_ldrfnn16, regD, regE},
	{z80_neg, NULL, NULL},		{z80_ret, NULL, NULL},		{z80_im1, NULL, NULL},		{z80_ld8, regA, &i},
	{z80_in, regE, regBC},		{z80_out, regE, regBC},		{z80_adc16, &hl, &de},		{z80_ld16rfnn, regD, regE},
	{z80_neg, NULL, NULL},		{z80_ret, NULL, NULL},		{z80_im2, NULL, NULL},		{z80_ld8, regA, &r},

	//0x60

	{z80_in, regH, regBC},		{z80_out, regH, regBC},		{z80_sbc16, regH, regL},	{z80_ldrfnn16, regH, regL},
	{z80_neg, NULL, NULL},		{z80_ret, NULL, NULL},		{z80_im0, NULL, NULL},		{z80_rrd, NULL, NULL},
	{z80_in, regL, regBC},		{z80_out, regH, regBC},		{z80_adc16, &hl, &hl},		{z80_ld16rfnn, regH, regL},
	{z80_neg, NULL, NULL},		{z80_ret, NULL, NULL},		{z80_im0, NULL, NULL},		{z80_rld, NULL, NULL},

	//0x70

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_sbc16, regS, regP},	{z80_ldrfnn16, regS, regP},
	{z80_neg, NULL, NULL},		{z80_ret, NULL, NULL},		{z80_im1, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_in, regA, regBC},		{z80_out, regA, regBC},		{z80_adc16, &hl, &sp},		{z80_ld16rfnn, regS, regP},
	{z80_neg, NULL, NULL},		{z80_ret, NULL, NULL},		{z80_im2, NULL, NULL},		{z80_fail, NULL, NULL},

	//0x80

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},

	//0x90

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},

	//0xa0

	{z80_ldi, NULL, NULL},		{z80_cpi, NULL, NULL},		{z80_ini, NULL, NULL},		{z80_outi, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_ldd, NULL, NULL},		{z80_cpd, NULL, NULL},		{z80_ind, NULL, NULL},		{z80_outd, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},

	//0xb0

	{z80_ldir, NULL, NULL},		{z80_cpir, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_lddr, NULL, NULL},		{z80_cpdr, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},

	//0xc0

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},

	//0xd0

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},

	//0xe0

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},

	//0xf0

	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
	{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},		{z80_fail, NULL, NULL},
};

void z80_set_normal_opset(void)
{
	z80_baseinst = z80_norm_baseinst;
	z80_cbinst = z80_norm_cbinst;
	z80_edinst = z80_norm_edinst;
	z80_ddfdinst = z80_norm_ddfdinst;
	z80_ddfdcbinst = z80_norm_ddfdcbinst;
}
