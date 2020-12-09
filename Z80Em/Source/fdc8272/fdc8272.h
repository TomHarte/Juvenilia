#include <stdio.h>

#define LOG

void fdc8272_end(void);
void fdc8272_setup(void);
void fdc8272_keepalive(void);
unsigned char get_status(void);
unsigned char fdc8272_set_motor(char byte);
void fdc8272_send(unsigned char data);
unsigned char fdc8272_read(void);

struct fdc8272_dskfile
{
	unsigned char numtracks, numsides, cside;
	unsigned short ctrack, csect, trackadd, sectadd, tracktarg, secttarg;
	unsigned char c, h, r, n, eot, gpl, dtl, cont;
	unsigned short readlen;
	unsigned short tracksize;
	unsigned char trackinfo[256], flags;
	int tracklocs[102][2];
	FILE *file;
};

#define fdc8272_set_ctrack(x) cdisk->ctrack = x << 8
#define fdc8272_set_csect(x) cdisk->csect = x << 8
#define fdc8272_get_ctrack() cdisk->ctrack >> 8
#define fdc8272_get_csect() cdisk->ctrack >> 8
#define fdc8272_set_tracktarg(x) cdisk->tracktarg = x << 8
#define fdc8272_set_secttarg(x) cdisk->secttarg = x << 8
#define fdc8272_set_trackrate(x, y) cdisk->trackadd = (x << 8) / y
#define fdc8272_set_sectrate(x, y) cdisk->trackadd = (x << 8) / y

#define TRACKSEARCH 1
#define SECTSEARCH  2

void select_function(unsigned char byte);
unsigned char fdc8272_readclueless(void);


extern struct fdc8272_dskfile *disk1, *disk2;
extern unsigned char st0, st1, st2, msr, fdc8272_motor;
extern struct fdc8272_dskfile *cdisk;
extern unsigned short srt, tfreq, tsteps;

#define set_fdc8272_dataready()  msr |= 128
#define reset_fdc8272_dataready() msr &= ~128

#define set_fdc8272_direction()  msr |= 64
#define reset_fdc8272_direction() msr &= ~64

#define set_fdc8272_execphase() msr |= 32
#define reset_fdc8272_execphase() msr &= ~32

#define set_fdc8272_busyflag()  msr |= 16
#define reset_fdc8272_busyflag() msr &= ~16

#define set_fdc8272_3busyflag()  msr |= 8
#define reset_fdc8272_3busyflag() msr &= ~8

#define set_fdc8272_2busyflag()  msr |= 4
#define reset_fdc8272_2busyflag() msr &= ~4

#define set_fdc8272_1busyflag()  msr |= 2
#define reset_fdc8272_1busyflag() msr &= ~2

#define set_fdc8272_0busyflag()  msr |= 1
#define reset_fdc8272_0busyflag() msr &= ~1
extern void (* fdc8272_sendfunc)(unsigned char byte);
extern unsigned char (* fdc8272_readfunc)(void);
extern unsigned char firstbyte;
extern int bytenum, tbyte;

