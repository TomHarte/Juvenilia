typedef struct
{
	void (*decode_addr)(void);
	unsigned char (*getfunc)(void);
	unsigned char (*operate)(unsigned char data);
	void (*putfunc)(unsigned char data);
} M6502_INSTRUCTION;

typedef union
{
	unsigned short full;

	struct
	{
		unsigned char low;
		unsigned char high;

	} bytes;

} M6502_WORD;

extern M6502_INSTRUCTION *m6502_inst;
extern unsigned char m6502_x, m6502_y, m6502_a, m6502_f, m6502_sp, *sztable, m6502_done, *m6502_maincyc;
extern unsigned short m6502_pc, m6502_addr;
extern int m6502_tstates, m6502_breakpoint;

extern void (* real_p_mem)(unsigned char data);
extern void (* m6502_callout)(int statenum);
extern void (* m6502_exitfunc)(void);


#define NOTSZ 125
#define NOTSZC 124
#define NOTNVZ 61


#define m6502_readmemory(x) 3
#define m6502_displace(x) (signed char)x
