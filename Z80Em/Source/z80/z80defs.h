#define M		&z80_jpconds[0]
#define Z		&z80_jpconds[1]
#define PA		&z80_jpconds[2]
#define C		&z80_jpconds[3]

#define regA			&af.bytes.high
#define regF			&af.bytes.low
#define regB			&bc.bytes.high
#define regC			&bc.bytes.low
#define regD			&de.bytes.high
#define regE			&de.bytes.low
#define regH			&hl.bytes.high
#define regL			&hl.bytes.low

#define regBC		&bc.full
#define regDE		&de.full
#define regHL		&hl.full
#define regSP		&sp.full

#define regBCp		&bc
#define regDEp		&de
#define regHLp		&hl

//less said about these 'registers', the better!

#define regS		&sp.bytes.high
#define regP		&sp.bytes.low
#define regIDh		&idx.bytes.high
#define regIDl		&idx.bytes.low
#define regID		&idx.full

#define RST00h		&z80_rstlist[0]
#define RST08h		&z80_rstlist[1]
#define RST10h		&z80_rstlist[2]
#define RST18h		&z80_rstlist[3]
#define RST20h		&z80_rstlist[4]
#define RST28h		&z80_rstlist[5]
#define RST30h		&z80_rstlist[6]
#define RST38h		&z80_rstlist[7]

#define BIT0		&z80_bmasks[0]
#define BIT1		&z80_bmasks[1]
#define BIT2		&z80_bmasks[2]
#define BIT3		&z80_bmasks[3]
#define BIT4		&z80_bmasks[4]
#define BIT5		&z80_bmasks[5]
#define BIT6		&z80_bmasks[6]
#define BIT7		&z80_bmasks[7]

#define inc_r()		r=((r+1)&127)|(r&128)

#define bank_no(a)	(a)>>13
#define offset(a)	(a)&8191
#define readmemory(x)	readbanks[bank_no(x)][offset(x)]
#define writememory(x)	writebanks[bank_no(x)][offset(x)]

#define z80_set_port_out_function(x) z80_port_out = x
#define z80_set_port_in_function(x) z80_port_in = x
#define z80_set_call_out_function(x) z80_callout = x
#define z80_set_memwrite_function(x) z80_writeout = x
#define z80_set_exit_function(x) z80_exitfunc = x
#define z80_set_tstate_counter(x) z80_tstates = x
#define z80_set_breakpoint(x) z80_breakpoint = x
#define z80_breakpoint() z80_breakpoint
#define z80_set_eicallback(x) z80_ei_callback = x

#define DJNZ_ADD z80_additives[0]
#define JRCC_ADD z80_additives[1]
#define JPCC_ADD z80_additives[2]
#define CALLCC_ADD z80_additives[3]
#define RETCC_ADD z80_additives[4]
#define R_ADD z80_additives[5]
