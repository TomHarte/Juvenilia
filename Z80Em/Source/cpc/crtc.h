void crtc_end(void);
void crtc_setup(void);
void setup_screen(void);
void crtc_line(void);

extern unsigned char vsyn, sccounter;
void change_pal(unsigned char col, unsigned char newcol);

#include <allegro.h>
union colentry
{
	unsigned int full;

	struct
	{
		unsigned char b1;
		unsigned char b2;
		unsigned char b3;
		unsigned char b4;
		/* 2005: doesn't seem to be endian dependent??? */
	} bytes;

};

extern unsigned char paltable[256];
extern union colentry mode0table[256], mode1table[256], mode2table[256], bcol;

struct palchange
{
	unsigned char col, newval, end;
	int tstate;
	struct palchange *next;
};

extern struct palchange *clist, *rlist;

