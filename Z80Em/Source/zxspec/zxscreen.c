#include <allegro.h>
#include "../z80/z80.h"
#include "../all.h"
#include "zxspec.h"
#include "zxscreen.h"

unsigned char zx_flashtable[256];
union zx_colentry zx_coltable[32768];
void (* scanline_pixels)(void);

#define get_checksum()	specptr = &mempool[scrlines[cy]+vmem_offset];\
			shift = 2;\
			newcheck = 0;\
			cx = 32;\
			while(cx--)\
			{\
				newcheck += (*specptr++) * shift;\
				newcheck += (*colptr)&128 ? flash&16 : 0;\
				newcheck += (*colptr++) * (34+shift);\
				shift = (shift + newcheck) & 65535;\
			}\
			colptr -= 32;

int specols[16];

unsigned short scrlines[] = {
	0x0000, 0x0100, 0x0200, 0x0300, 0x0400, 0x0500, 0x0600, 0x0700,
	0x0020, 0x0120, 0x0220, 0x0320, 0x0420, 0x0520, 0x0620, 0x0720,
	0x0040, 0x0140, 0x0240, 0x0340, 0x0440, 0x0540, 0x0640, 0x0740,
	0x0060, 0x0160, 0x0260, 0x0360, 0x0460, 0x0560, 0x0660, 0x0760,
	0x0080, 0x0180, 0x0280, 0x0380, 0x0480, 0x0580, 0x0680, 0x0780,
	0x00a0, 0x01a0, 0x02a0, 0x03a0, 0x04a0, 0x05a0, 0x06a0, 0x07a0,
	0x00c0, 0x01c0, 0x02c0, 0x03c0, 0x04c0, 0x05c0, 0x06c0, 0x07c0,
	0x00e0, 0x01e0, 0x02e0, 0x03e0, 0x04e0, 0x05e0, 0x06e0, 0x07e0,

	0x0800, 0x0900, 0x0a00, 0x0b00, 0x0c00, 0x0d00, 0x0e00, 0x0f00,
	0x0820, 0x0920, 0x0a20, 0x0b20, 0x0c20, 0x0d20, 0x0e20, 0x0f20,
	0x0840, 0x0940, 0x0a40, 0x0b40, 0x0c40, 0x0d40, 0x0e40, 0x0f40,
	0x0860, 0x0960, 0x0a60, 0x0b60, 0x0c60, 0x0d60, 0x0e60, 0x0f60,
	0x0880, 0x0980, 0x0a80, 0x0b80, 0x0c80, 0x0d80, 0x0e80, 0x0f80,
	0x08a0, 0x09a0, 0x0aa0, 0x0ba0, 0x0ca0, 0x0da0, 0x0ea0, 0x0fa0,
	0x08c0, 0x09c0, 0x0ac0, 0x0bc0, 0x0cc0, 0x0dc0, 0x0ec0, 0x0fc0,
	0x08e0, 0x09e0, 0x0ae0, 0x0be0, 0x0ce0, 0x0de0, 0x0ee0, 0x0fe0,

	0x1000, 0x1100, 0x1200, 0x1300, 0x1400, 0x1500, 0x1600, 0x1700,
	0x1020, 0x1120, 0x1220, 0x1320, 0x1420, 0x1520, 0x1620, 0x1720,
	0x1040, 0x1140, 0x1240, 0x1340, 0x1440, 0x1540, 0x1640, 0x1740,
	0x1060, 0x1160, 0x1260, 0x1360, 0x1460, 0x1560, 0x1660, 0x1760,
	0x1080, 0x1180, 0x1280, 0x1380, 0x1480, 0x1580, 0x1680, 0x1780,
	0x10a0, 0x11a0, 0x12a0, 0x13a0, 0x14a0, 0x15a0, 0x16a0, 0x17a0,
	0x10c0, 0x11c0, 0x12c0, 0x13c0, 0x14c0, 0x15c0, 0x16c0, 0x17c0,
	0x10e0, 0x11e0, 0x12e0, 0x13e0, 0x14e0, 0x15e0, 0x16e0, 0x17e0
};



unsigned char flash;
unsigned char *specptr, *colptr;
int cy;
extern int y;

void scanline_pixels_8(void)
{
	int cx, newcheck, shift;
	unsigned long vptr;
	unsigned char *specptr, col, pels;

	if(cy&7)
		colptr -= 32;

	get_checksum();

	specptr = &mempool[scrlines[cy]+vmem_offset];
	vptr = bmp_write_line(screen, y);
	bmp_select(screen);

	cx = 32;
	while(cx--)

		bmp_write8(vptr++, zx_bcol);



	if(newcheck != drawsums[cy])

	{

		drawsums[cy] = newcheck;



		cx = 32;

		while(cx--)

		{

			col = *colptr;



			if(flash&16)

				col = zx_flashtable[col];



			col &= 127;

			pels = *specptr;



			bmp_write32(vptr, zx_coltable[(col << 8) | pels].full);

			pels <<= 4;

			vptr += 4;

			bmp_write32(vptr, zx_coltable[(col << 8) | pels].full);

			vptr += 4;



			specptr++;

			colptr++;

		}

	}

	else

	{

		vptr += 256;

		colptr +=32;

	}



	cx = 32;

	while(cx--)

		bmp_write8(vptr++, zx_bcol);



	bmp_unwrite_line(screen);

	cy++;

	y++;

}



void scanline_pixels_16(void)

{

	int cx, newcheck, shift;

        unsigned short  bcol; //paper, ink,

	unsigned long vptr;

	unsigned char *specptr, col, pels;



	if(cy&7)

		colptr -= 32;



	get_checksum();



	specptr = &mempool[scrlines[cy]+vmem_offset];

	vptr = bmp_write_line(screen, y);

	bmp_select(screen);



	bcol = specols[zx_bcol];



	cx = 32;

	while(cx--)

	{

		bmp_write16(vptr, bcol);

		vptr += 2;

	}



	if(newcheck != drawsums[cy])

	{

		drawsums[cy] = newcheck;



		cx = 32;

		while(cx--)

		{

			col = *colptr;



			if(flash&16)

				col = zx_flashtable[col];



			col &= 127;

			pels = *specptr;



			bmp_write32(vptr, zx_coltable[(col << 8) | pels].full);

			pels <<= 2;

			vptr += 4;

			bmp_write32(vptr, zx_coltable[(col << 8) | pels].full);

			pels <<= 2;

			vptr += 4;

			bmp_write32(vptr, zx_coltable[(col << 8) | pels].full);

			pels <<= 2;

			vptr += 4;

			bmp_write32(vptr, zx_coltable[(col << 8) | pels].full);

			vptr += 4;

                        



			specptr++;

			colptr++;

		}

	}

	else

	{

		vptr += 512;

		colptr +=32;

	}



	cx = 32;

	while(cx--)

	{

		bmp_write16(vptr, bcol);

		vptr += 2;

	}



	bmp_unwrite_line(screen);

	cy++;

	y++;

}



void scanline_pixels_24(void)

{

	int cx, newcheck, shift;

        unsigned int paper, ink, bcol;

	unsigned long vptr;

	unsigned char *specptr, col;// pels;



	if(cy&7)

		colptr -= 32;



	get_checksum();



	specptr = &mempool[scrlines[cy]+vmem_offset];

	vptr = bmp_write_line(screen, y);

	bmp_select(screen);



	bcol = specols[zx_bcol];



	cx = 32;

	while(cx--)

	{

		bmp_write24(vptr, bcol);

		vptr += 3;

	}



	if(newcheck != drawsums[cy])

	{

		drawsums[cy] = newcheck;



		cx = 32;

		while(cx--)

		{



			col = *colptr;

			if(flash&16)

				col = zx_flashtable[col];

                                

			if(col&64)

			{

				paper = specols[((*colptr >> 3) & 7) + 8];

				ink = specols[((*colptr) & 7) + 8];

			}

			else

			{

				paper = specols[((*colptr >> 3) & 7)];

				ink = specols[((*colptr) & 7)];

			}





			bmp_write24(vptr, (*specptr)&128	? ink : paper);

			vptr += 3;

			bmp_write24(vptr, (*specptr)&64		? ink : paper);

			vptr += 3;

			bmp_write24(vptr, (*specptr)&32		? ink : paper);

			vptr += 3;

			bmp_write24(vptr, (*specptr)&16		? ink : paper);

			vptr += 3;

			bmp_write24(vptr, (*specptr)&8		? ink : paper);

			vptr += 3;

			bmp_write24(vptr, (*specptr)&4		? ink : paper);

			vptr += 3;

			bmp_write24(vptr, (*specptr)&2		? ink : paper);

			vptr += 3;

			bmp_write24(vptr, (*specptr)&1		? ink : paper);

			vptr += 3;



			specptr++;

			colptr++;

		}

	}

	else

	{

		vptr += 768;

		colptr +=32;

	}



	cx = 32;

	while(cx--)

	{

		bmp_write24(vptr, bcol);

		vptr += 3;

	}



	bmp_unwrite_line(screen);

	cy++;

	y++;

}



void scanline_pixels_32(void)

{

	int cx, newcheck, shift;

        unsigned int paper, ink, bcol;

	unsigned long vptr;

	unsigned char *specptr, col;// pels;



	if(cy&7)

		colptr -= 32;



	get_checksum();



	specptr = &mempool[scrlines[cy]+vmem_offset];

	vptr = bmp_write_line(screen, y);

	bmp_select(screen);



	bcol = specols[zx_bcol];



	cx = 32;

	while(cx--)

	{

		bmp_write32(vptr, bcol);

		vptr += 4;

	}



	if(newcheck != drawsums[cy])

	{

		drawsums[cy] = newcheck;



		cx = 32;

		while(cx--)

		{



			col = *colptr;

			if(flash&16)

				col = zx_flashtable[col];

                                

			if(col&64)

			{

				paper = specols[((*colptr >> 3) & 7) + 8];

				ink = specols[((*colptr) & 7) + 8];

			}

			else

			{

				paper = specols[((*colptr >> 3) & 7)];

				ink = specols[((*colptr) & 7)];

			}





			bmp_write32(vptr, (*specptr)&128	? ink : paper);

			vptr += 4;

			bmp_write32(vptr, (*specptr)&64		? ink : paper);

			vptr += 4;

			bmp_write32(vptr, (*specptr)&32		? ink : paper);

			vptr += 4;

			bmp_write32(vptr, (*specptr)&16		? ink : paper);

			vptr += 4;

			bmp_write32(vptr, (*specptr)&8		? ink : paper);

			vptr += 4;

			bmp_write32(vptr, (*specptr)&4		? ink : paper);

			vptr += 4;

			bmp_write32(vptr, (*specptr)&2		? ink : paper);

			vptr += 4;

			bmp_write32(vptr, (*specptr)&1		? ink : paper);

			vptr += 4;



			specptr++;

			colptr++;

		}

	}

	else

	{

		vptr += 1024;

		colptr +=32;

	}



	cx = 32;

	while(cx--)

	{

		bmp_write32(vptr, bcol);

		vptr += 4;

	}



	bmp_unwrite_line(screen);

	cy++;

	y++;

}



void scanline_border(void)

{

	hline(screen, 0, y++, 320, specols[zx_bcol]);

}



void scanline_nothing(void)

{

}



void scanline_restart(void)

{

	scannum = -1;

	release_bitmap(screen);

}

void init_scanlines(void)
{
	flash++;

	colptr = &mempool[vmem_offset+6144];
	cy = y = 0;
	z80_do_interrupt();
	acquire_bitmap(screen);
}

void set_sinclair_gfxmode(void)

{

	PALETTE cols;

	int a = 0;



	set_gfx_mode(GFX_AUTODETECT, 320, 240, 0, 0);



	if((coldepth & 63) == 8)

	{

		cols[a].r	= 0;

		cols[a].g	= 0;

		cols[a].b	= 0;

		a++;



		cols[a].r	= 0;

		cols[a].g	= 0;

		cols[a].b	= 164/ 4; //41

		a++;



		cols[a].r	= 224/ 4; //56

		cols[a].g	= 0;

		cols[a].b	= 0;

		a++;



		cols[a].r	= 232/ 4; //58

		cols[a].g	= 0;

		cols[a].b	= 184/ 4; //46

		a++;



		cols[a].r	= 0;

		cols[a].g	= 216/ 4; //54

		cols[a].b	= 0;

		a++;



		cols[a].r	= 0;

		cols[a].g	= 216/ 4; //54

		cols[a].b	= 216/ 4; //54

		a++;



		cols[a].r	= 212/ 4; //53

		cols[a].g	= 212/ 4; //53

		cols[a].b	= 0;

		a++;



		cols[a].r	= 204/ 4; //51

		cols[a].g	= 204/ 4; //51

		cols[a].b	= 204/ 4; //51

		a++;



		cols[a].r	= 0;

		cols[a].g	= 0;

		cols[a].b	= 0;

		a++;



		cols[a].r	= 0;

		cols[a].g	= 0;

		cols[a].b	= 176/ 4; //44

		a++;



		cols[a].r	= 244/ 4; //61

		cols[a].g	= 0;

		cols[a].b	= 0;

		a++;



		cols[a].r	= 255/ 4; //63

		cols[a].g	= 0;

		cols[a].b	= 224/ 4; //56

		a++;



		cols[a].r	= 0;

		cols[a].g	= 244/ 4; //61

		cols[a].b	= 0;

		a++;



		cols[a].r	= 0;

		cols[a].g	= 255/ 4; //63

		cols[a].b	= 255/ 4; //63

		a++;



		cols[a].r	= 255/ 4; //63

		cols[a].g	= 255/ 4; //63

		cols[a].b	= 0;

		a++;



		cols[a].r	= 255/ 4; //63

		cols[a].g	= 255/ 4; //63

		cols[a].b	= 255/ 4; //63

		a++;

		set_palette(cols);



		while(a--)

			specols[a] = a;

	}

	else

	{

		specols[0]  = makecol(0, 0, 0);

		specols[1]  = makecol(0, 0, 164);

		specols[2]  = makecol(224, 0, 0);

		specols[3]  = makecol(232, 0, 184);

		specols[4]  = makecol(0, 216, 0);

		specols[5]  = makecol(0, 216, 216);

		specols[6]  = makecol(212, 212, 0);

		specols[7]  = makecol(204, 204, 204);

		specols[8]  = makecol(0, 0, 0);

		specols[9]  = makecol(0, 0, 176);

		specols[10] = makecol(244, 0, 0);

		specols[11] = makecol(255, 0, 224);

		specols[12] = makecol(0, 244, 0);

		specols[13] = makecol(0, 255, 255);

		specols[14] = makecol(255, 255, 0);

		specols[15] = makecol(255, 255, 255);

	}

}



void zxscr_table_setup(void)
{
	unsigned short col, byte;

	set_sinclair_gfxmode();

	switch(coldepth & 63)
	{
		case 8 :
			col = 128;
		        while(col--)
			{
				if(col & 64)
				{
					byte = 256;
					while(byte--)
					{
						zx_coltable[(col << 8) | byte].bytes.b1 = (byte&128 ? (col & 7) : ((col >> 3) & 7)) + 8;
						zx_coltable[(col << 8) | byte].bytes.b2 = (byte&64  ? (col & 7) : ((col >> 3) & 7)) + 8;
						zx_coltable[(col << 8) | byte].bytes.b3 = (byte&32  ? (col & 7) : ((col >> 3) & 7)) + 8;
						zx_coltable[(col << 8) | byte].bytes.b4 = (byte&16  ? (col & 7) : ((col >> 3) & 7)) + 8;
					}
				}
				else
				{
					byte = 256;
					while(byte--)
					{
						zx_coltable[(col << 8) | byte].bytes.b1 = byte&128 ? (col & 7) : ((col >> 3) & 7);
						zx_coltable[(col << 8) | byte].bytes.b2 = byte&64  ? (col & 7) : ((col >> 3) & 7);
						zx_coltable[(col << 8) | byte].bytes.b3 = byte&32  ? (col & 7) : ((col >> 3) & 7);
						zx_coltable[(col << 8) | byte].bytes.b4 = byte&16  ? (col & 7) : ((col >> 3) & 7);
					}
				}
			}

			scanline_pixels = scanline_pixels_8;
		break;
                
		case 15 :
		case 16 :
			col = 128;
		        while(col--)
			{
				if(col & 64)
				{
					byte = 256;
					while(byte--)
					{
 						zx_coltable[(col << 8) | byte].words.b1 = specols[(byte&128  ? (col & 7) : ((col >> 3) & 7)) + 8];
						zx_coltable[(col << 8) | byte].words.b2 = specols[(byte&64  ? (col & 7) : ((col >> 3) & 7)) + 8];
					}
				}
				else
				{
					byte = 256;
					while(byte--)
					{
						zx_coltable[(col << 8) | byte].words.b1 = specols[byte&128 ? (col & 7) : ((col >> 3) & 7)];
						zx_coltable[(col << 8) | byte].words.b2 = specols[byte&64  ? (col & 7) : ((col >> 3) & 7)];
					}
				}
			}
			scanline_pixels = scanline_pixels_16;
			fflush(stderr);
		break;

		case 24 :
			scanline_pixels = scanline_pixels_24;
		break;

		case 32 :
			scanline_pixels = scanline_pixels_32;
		break;
	}

	col = 256;
	while(col--)
		if(col & 128)
			zx_flashtable[col] = (col & 192) | ((col >> 3) & 7) | ((col & 7) << 3);
		else
			zx_flashtable[col] = col;

}

