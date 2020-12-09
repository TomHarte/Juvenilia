#include <allegro.h>
#include "jacescr.h"
#include "../z80/z80.h"
#include "../zxspec/zxspec.h"
#include "jacekeyb.h"

int y, scancount;

void jace_scanline_nothing(void)
{
}

void jace_scanline_border(void)
{
	y++;
}

void jace_scanline_restart(void)
{
	scancount = -1;
	scannum = -1;
	release_bitmap(screen);
	jace_updatekeybits();
}

void jace_init_scanlines(void)
{
	y = 0;
	z80_do_interrupt();
	acquire_bitmap(screen);
	bmp_select(screen);
}

void jace_scanline_pixels(void)
{
	unsigned char *cptr, charline;
	int c;
	unsigned long vptr;

	scancount++;

	cptr = &mempool[0x2400 + ((scancount&~7) << 2)];

	vptr = bmp_write_line(screen, y) + 32;
	c = 32;
	while(c--)
	{
		charline = mempool[0x2c00 + ((unsigned short)((*cptr)&127) << 3) + (scancount&7)];
		if((*cptr)&128)
			charline = ~charline;

		bmp_write8(vptr++, charline&128);
		bmp_write8(vptr++, charline&64);
		bmp_write8(vptr++, charline&32);
		bmp_write8(vptr++, charline&16);
		bmp_write8(vptr++, charline&8);
		bmp_write8(vptr++, charline&4);
		bmp_write8(vptr++, charline&2);
		bmp_write8(vptr++, charline&1);

		cptr++;
	}

	bmp_unwrite_line(screen);
	y++;
}
