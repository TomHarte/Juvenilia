int tms9918a_setup(void);
void tms9918a_end(void);
void tms9918a_keepalive(void);
unsigned char tms9918a_receivebyte(unsigned char port);
void tms9918a_sendbyte(unsigned char byte, unsigned char port);

extern unsigned char tms9918a_regs[8], tms9918a_sr, *tms9918a_mem, bytebit, rmode, rahead;

#define inc_memaddr() memaddr.full = (memaddr.full+1) & 16383

