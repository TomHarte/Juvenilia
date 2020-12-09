#include "../all.h"
#include "crtc.h"
#include <allegro.h>
#include "cpc.h"
#include "../z80/z80.h"

unsigned char vsyn, sccounter;
unsigned char paltable[256];
union colentry mode0table[256], mode1table[256], mode2table[256], bcol;
struct palchange *clist, *rlist;

#define inc_yc() {\
if(scry >= startscry+(crtcregs[5] << (bshift - 3)))\
{\
			yline = (yline + 2048) & 14336;\
			addr = baddr | yline;\
			ychar++;\
			vsyncount++;\
			if(ychar > crtcregs[9])\
			{\
				yc++;\
				oroffs += crtcregs[1];\
				ychar =\
				yline = 0;\
			}\
}\
		}


int baddr, oroffs, roffs, counter2, hsyn, xc, vsyncount;
void (*cpc_doblock)(void);
unsigned char ychar, yc;
unsigned short yline, vsynd;
short scry, startscry;
int addr;
unsigned char oldmode, bshift, badd;
long mptr;
unsigned long vptr;
struct palchange *plist, *reglist;

void change_pal(unsigned char entry, unsigned char newcol)
{
	int c;
	unsigned char c2, c3, c4;
	union colentry col;

	c = 256;
	while(c--)
	{
		c2 = c << 1;
		c3 = c2 << 1;
		c4 = c3 << 1;
        
		//do mode 0 bytes
		col = mode0table[c];
		col.bytes.b1 = col.bytes.b2 = paltable[c&170] == entry ? newcol : col.bytes.b2;
		col.bytes.b3 = col.bytes.b4 = paltable[c2&170] == entry ? newcol : col.bytes.b4;
		mode0table[c] = col;
                
		//do mode 1 bytes
		col = mode1table[c];
		col.bytes.b1 = paltable[c&136] == entry ? newcol : col.bytes.b1;
		col.bytes.b2 = paltable[c2&136] == entry ? newcol : col.bytes.b2;
		col.bytes.b3 = paltable[c3&136] == entry ? newcol : col.bytes.b3;
		col.bytes.b4 = paltable[c4&136] == entry ? newcol : col.bytes.b4;
		mode1table[c] = col;
                
		//do mode 2 bytes
		col = mode2table[c];
		col.bytes.b1 = paltable[c&128] == entry ? newcol : col.bytes.b1;
		col.bytes.b2 = paltable[c2&128] == entry ? newcol : col.bytes.b2;
		col.bytes.b3 = paltable[c3&128] == entry ? newcol : col.bytes.b3;
		col.bytes.b4 = paltable[c4&128] == entry ? newcol : col.bytes.b4;
		mode2table[c] = col;
                
	}
}

void do_no_block(void)
{
}

void do_mode0_block(void)
{
	unsigned char byte;
//	unsigned short col;

	if(xc < crtcregs[1] && !hsyn)
	{
		byte = mempool[addr | ((roffs << 1) & 2047)];
		bmp_write32(vptr, mode0table[byte].full);
		vptr += 4;
                
		byte = mempool[addr | (((roffs << 1)+1) & 2047)];
		bmp_write32(vptr, mode0table[byte].full);
		vptr += 4;
	}
	else
	{
		bmp_write32(vptr, bcol.full);
		vptr += 4;
		bmp_write32(vptr, bcol.full);
		vptr += 4;
	}
}

void do_mode1_block(void)
{
	unsigned char byte;
	if(xc < crtcregs[1] && !hsyn)
	{
		byte = mempool[addr | ((roffs << 1) & 2047)];
		bmp_write32(vptr, mode1table[byte].full);
		vptr += 4;
                
		byte = mempool[addr | (((roffs << 1)+1) & 2047)];
		bmp_write32(vptr, mode1table[byte].full);
		vptr += 4;
	}
	else
	{
		bmp_write32(vptr, bcol.full);
		vptr += 4;
		bmp_write32(vptr, bcol.full);
		vptr += 4;
	}
}

void do_mode2_block(void)
{
	unsigned char byte;
	if(xc < crtcregs[1] && !hsyn)
	{
        
		byte = mempool[addr | ((roffs << 1) & 2047)];
		bmp_write32(vptr, mode2table[byte].full);
		vptr += 4;
		byte <<= 4;
		bmp_write32(vptr, mode2table[byte].full);
		vptr += 4;

		byte = mempool[addr | (((roffs << 1) + 1) & 2047)];
		bmp_write32(vptr, mode2table[byte].full);
		vptr += 4;
		byte <<= 4;
		bmp_write32(vptr, mode2table[byte].full);
		vptr += 4;
	}
	else
	{
		bmp_write32(vptr, bcol.full);
		vptr += 4;
		bmp_write32(vptr, bcol.full);
		vptr += 4;
		bmp_write32(vptr, bcol.full);
		vptr += 4;
		bmp_write32(vptr, bcol.full);
		vptr += 4;
	}
}

void crtc_refresh(void)
{
	unsigned char smode;
        
	if(oldmode != scrmode)
	{
		switch(scrmode)
		{
			case 0 :
				cpc_doblock = do_mode0_block;
			break;

			case 1 :
				cpc_doblock = do_mode1_block;
			break;

			case 2 :
				cpc_doblock = do_mode2_block;
			break;

		}

		smode = scrmode >> 1;

		if(smode != (oldmode >> 1))
		{
			switch(smode)
			{
				case 0 :
					set_gfx_mode(GFX_AUTODETECT, 400, 300, 0, 0);
					bshift = 3;
					badd = 8;
				break;

				case 1 :
					set_gfx_mode(GFX_AUTODETECT, 800, 600, 0, 0);
					bshift = 4;
					badd = 16;
				break;
			}
			set_palette(prepal);
		}
                
		oldmode = scrmode;
	}
}

struct palchange *create_palentry(struct palchange *last)
{
	struct palchange *newpal;
        
	newpal = malloc(sizeof(struct palchange));
	newpal->end = 1;

	if(last)
		last->next = newpal;

	return newpal;
}

void crtc_destroy_list(struct palchange *list)
{
	struct palchange *first, *old;

	first = list;
	list = list->next;

	while(list != first)
	{
		old = list->next;
		free(list);
		list = old;
	}

	free(list);
}

void crtc_end(void)
{
	crtc_destroy_list(clist);
	crtc_destroy_list(rlist);
}

void crtc_setup(void)
{
	struct palchange *one;
	int c;

	setup_prepal();
	cpc_doblock = do_no_block;
	vsyn = 1;
	oldmode = 8;

	one = clist = create_palentry(NULL);
	c = 63;
	while(c--)
	{
		clist = create_palentry(clist);
	}
	clist->next = one;
	plist = clist;
        
	one = rlist = create_palentry(NULL);
	c = 63;
	while(c--)
	{
		rlist = create_palentry(rlist);
	}
	rlist->next = one;
	reglist = rlist;

	c = 256;
	while(c--)
	{
		paltable[c] = ((c&2) << 2) | ((c & 8) >> 2) | ((c & 32) >> 3) | ((c & 128) >> 7);
	}
}

void crtc_pel_line(void)
{
	int c = 0, oldtstates;
	unsigned long bvptr;
	unsigned char byte;//, col;
        
	bvptr = bmp_write_line(screen, scry);
        
	oldtstates = z80_tstates;
	z80_tstates -= 256;

/*	while(!plist->end && plist->tstate < tstates)
	{
		if(plist->col & 16)
			bcol = plist->newval;
		else
		{
			coltable[((plist->col & 1) << 7) | ((plist->col & 2) << 2) | ((plist->col & 4) << 3) | ((plist->col & 8) >> 2)] = plist->newval;
			bcoltable[((plist->col & 1) << 7) | ((plist->col & 2) << 2) | ((plist->col & 4) << 3) | ((plist->col & 8) >> 2)] = plist->newval;// | ((unsigned short)plist->newval << 8);
		}
		plist = plist->next;
	}
                
	while(!reglist->end && reglist->tstate < tstates)
	{
		crtcregs[reglist->col] = reglist->newval;
		reglist = reglist->next;
	}*/

	do
	{
		if(!plist->end && z80_tstates >= plist->tstate)
		{
			if(plist->col & 16)
				bcol.bytes.b1 = bcol.bytes.b2 = bcol.bytes.b3 = bcol.bytes.b4 = plist->newval;
			else
			{
				change_pal(plist->col, plist->newval);
			}
			plist = plist->next;
		}
                
		if(!reglist->end && z80_tstates >= reglist->tstate)
		{
			crtcregs[reglist->col] = reglist->newval;
			reglist = reglist->next;
		}
                
		if(mptr >= 0 && mptr < SCREEN_W)
		{
			vptr = mptr + bvptr;
			cpc_doblock();
		}
                
		roffs ++;
		xc++;
		mptr += badd;
		z80_tstates += 4;
		c++;
                
		if(xc > crtcregs[0])
		{
			inc_yc();
			xc = 0;
			roffs = oroffs;
		}
                
		if(xc > crtcregs[2])
		{
			if(!hsyn)
			{
				hsyn = 1;
				mptr = -(12 << bshift) - (((50 << bshift)-SCREEN_W) >> 1);
			}
		}
		else
		{
			hsyn = 0;
		}
                
	}
	while(xc);// < 64)
        
	bmp_unwrite_line(screen);
	z80_tstates = oldtstates;
}

void crtc_border_line(void)
{
	int c;
        
	inc_yc();
	hline(screen, 0, scry, SCREEN_W, bcol.bytes.b1);
        
	z80_tstates -= 256;
                
	c = 64;
	while(c--)
	{
		if(!plist->end && z80_tstates >= plist->tstate)
		{
			if(plist->col & 16)
				bcol.bytes.b1 = bcol.bytes.b2 = bcol.bytes.b3 = bcol.bytes.b4 = plist->newval;
			else
			{
				change_pal(plist->col, plist->newval);
			}
			plist = plist->next;
		}
                
		if(!reglist->end && z80_tstates >= reglist->tstate)
		{
			crtcregs[reglist->col] = reglist->newval;
			reglist = reglist->next;
		}
		z80_tstates += 4;
	}
}

void crtc_do_interrupt(void)
{
	sccounter &= 31;
	z80_set_eicallback(NULL);
	z80_do_interrupt();
}

void crtc_line(void)
{
	bmp_select(screen);
        
	if(scry < SCREEN_H && scry >= 0 && yc < crtcregs[6])
	{
		crtc_pel_line();
	}
	else
		crtc_border_line();
//		inc_yc();

	if((scrmode >> 1) == (oldmode >> 1))// && scrmode != oldmode)
		switch(scrmode)
		{
			case 0 :
				cpc_doblock = do_mode0_block;
			break;

			case 1 :
				cpc_doblock = do_mode1_block;
			break;

			case 2 :
				cpc_doblock = do_mode2_block;
			break;

		}
                
	scry++;
	scry += (scrmode >> 1);

	if(!scry)
	{
		acquire_bitmap(screen);
	}

	if(scry == SCREEN_H)
		release_bitmap(screen);
        
	if(yc > crtcregs[4])
	{
		yc = 0;
		vsynd = 0;
		baddr = (crtcregs[12]&48) << 10;
		roffs = oroffs = crtcregs[13] | ((crtcregs[12]&3) << 8);
	}
        
	if(yc >= crtcregs[7])
	{
		if(!vsynd)
		{
			vsyn = 1;
			vsynd = 1;
			counter2 = 1;
			startscry = scry = (SCREEN_H >> 1) - (40 << (bshift-1));
			crtc_refresh();
			vsyncount = 0;
		}
	}

	if(vsyncount >= (crtcregs[3] >> 4))
		vsyn = 0;
        
	if(sccounter == 52)
	{
		sccounter = 0;
		if(imode & 192)
			z80_do_interrupt();
		else
			z80_set_eicallback(crtc_do_interrupt);
	}

	if(counter2 & 4)
	{
		sccounter = 0;
	}
        
	sccounter++;
	counter2 <<= 1;
}
