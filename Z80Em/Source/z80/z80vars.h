//Z80 registers
extern Z80_REGISTERPAIR	af,		bc,		de,		hl,		ix,		iy,		sp;
extern Z80_REGISTERPAIR	afd,	bcd,	ded,	hld;
extern unsigned short	pc;
extern unsigned char	i,		r;

//interrupt mode, memory and index register variables
extern unsigned char	imode,		**writebanks,	**readbanks,		*mempool;
extern Z80_REGISTERPAIR	idx;

//result, rst, bitmask and condition lookup tables
extern unsigned char	z80_sz53p[256], z80_bmasks[8];
extern unsigned short	z80_addhvc[131072], z80_subhvc[131072], z80_rstlist[8];
extern unsigned char	z80_jpconds[4];

//done flag (set to leave), and tstates counter variables
extern char done;

//memory input/output vars
extern Z80_REGISTERPAIR z80_outword, z80_outaddr;
extern unsigned char z80_outflag;

extern void (* z80_port_out)(unsigned short portnum, unsigned char data);
extern void (* z80_port_in)(unsigned short portnum, unsigned char *data);
extern void (* z80_callout)(int statenum);
extern void (* z80_writeout)(void);
extern void (* z80_exitfunc)(void);
extern void (* z80_ei_callback)(void);

extern int z80_tstates, z80_breakpoint;

extern unsigned char *z80_maincyc, *z80_cbcyc, *z80_ddfdcyc, *z80_ddfdcbcyc, *z80_edcyc, *z80_additives, z80_imchange, *z80_zones, *z80_bankzones;
extern Z80_INSTRUCTION *z80_baseinst, *z80_cbinst, *z80_ddfdinst, *z80_ddfdcbinst, *z80_edinst;
