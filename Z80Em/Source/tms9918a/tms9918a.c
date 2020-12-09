#include <allegro.h>
#include "../z80/z80.h"
#include "tms9918a.h"
#include <string.h>

unsigned char tms9918a_regs[8], tms9918a_sr, *tms9918a_mem, bytebit, rmode, rahead;
Z80_REGISTERPAIR memaddr;
extern int scancount, y;
int c;
unsigned char lowbyte, textcol, bordercol, pgand, ctand, colnum, lastnum;
unsigned char sprdim;
unsigned short pg, ct, pn, pnbase, sa, sg;

void (* tms9918a_pelline)(int scry, int logy);

struct draw_spr_info_block
{
	unsigned char spid, col;
	int x, y;
	unsigned short mempos, c, on;
};

unsigned char sprfree[32];
struct draw_spr_info_block tms9918a_sprites[4];

struct tms_spr_info_block
{
	unsigned char y, x, pattern, colour;
};

void find_next_sprite(struct draw_spr_info_block *spr, int logy)
{
	int c;
	unsigned char lowy, lowc, found;
	struct tms_spr_info_block *sprite;

	found = 0;

	while(!found)
	{
		lowc = 32;
		c = 0;
		lowy = 255;
		sprite = (struct tms_spr_info_block *)&tms9918a_mem[sa];

		while( (c<32) && (sprite->y != 208))
		{
			if((unsigned char)(sprite->y+2) < lowy && !sprfree[c])
			{
				lowy = (unsigned char)(sprite->y+2);
				lowc = c;
			}
                
			sprite++;
			c++;
		}

		sprite = (struct tms_spr_info_block *)&tms9918a_mem[sa + (lowc << 2)];

		if(lowc == 32 || sprite->y == 208)
		{
			spr->on = 0;
			found = 1;
		}
		else
		{
			found = 1;
			sprfree[lowc] = 1;

			if(tms9918a_regs[1]&2)
				spr->mempos = ((unsigned short)(sprite->pattern&252) << 3) + sg;
			else
				spr->mempos = ((unsigned short)sprite->pattern << 3) + sg;

			spr->c = sprdim;
			spr->col = sprite->colour & 15;

			spr->spid = lowc;
			spr->on = 1;

			spr->x = sprite->x;
			if(sprite->colour & 128)
				spr->x -= 32;

			spr->y = (unsigned char)(sprite->y + 2);
			if(spr->y < logy) // we missed this one - it was 5th sprite
			{
				if(!(tms9918a_sr&64))
				{
					tms9918a_sr = 64 | (tms9918a_sr & ~31) | lowc;
					lastnum = lowc;
				}

/*				if(logy - spr->y <= sprdim)
				{
					spr->mempos += tms9918a_regs[1]&1 ? (logy - spr->y >> 1) : logy - spr->y;
					spr->y = logy;
				}
				else*/
					found = 0;
			}
		}
	}
}

unsigned char find_last_sprite(void)
{
	int c;
	struct tms_spr_info_block *sprite;
        
	c = 0;
	sprite = (struct tms_spr_info_block *)&tms9918a_mem[sa];

	while(c<32 && sprite->y != 208)
	{
		c++;
		sprite++;
	}

	return c;
}

void handle_sprites(unsigned long vptr, int logy)
{
	int sprc, sprc1, sprc2; //ssprc, pelc, 
	unsigned long pelstart;// cpel;
	unsigned char byte;
	unsigned int line1, line2;

	sprc1 = 4;
	while(sprc1--)
	{

		if(tms9918a_sprites[sprc1].on && logy >= tms9918a_sprites[sprc1].y)
		{
			sprc2 = 4;
			while(sprc2--)
				if(sprc2 != sprc1)
				{
					if(tms9918a_sprites[sprc1].x - tms9918a_sprites[sprc2].x < sprdim && tms9918a_sprites[sprc1].x - tms9918a_sprites[sprc2].x >= 0 && tms9918a_sprites[sprc2].on && logy >= tms9918a_sprites[sprc2].y)
					{
						if(tms9918a_regs[1]&2)
						{
							line1 = tms9918a_mem[tms9918a_sprites[sprc1].mempos];
							line1 |= tms9918a_mem[tms9918a_sprites[sprc1].mempos + 16] << 8;
	                                        
							line2 = tms9918a_mem[tms9918a_sprites[sprc2].mempos];
							line2 |= tms9918a_mem[tms9918a_sprites[sprc2].mempos + 16] << 8;

							line1 <<= (tms9918a_sprites[sprc1].x - tms9918a_sprites[sprc2].x);
						}
						else
						{
							line1 = tms9918a_mem[tms9918a_sprites[sprc1].mempos];
							line2 = tms9918a_mem[tms9918a_sprites[sprc2].mempos];
                                        
							line1 <<= (tms9918a_sprites[sprc1].x - tms9918a_sprites[sprc2].x);
						}

						if((line1 | line2) != (line1 ^ line2))
							tms9918a_sr |= 32;
					}
				}
		}
	}

	sprc = 4;
	while(sprc--)
	{
		if(logy >= tms9918a_sprites[sprc].y && tms9918a_sprites[sprc].on)
		{
			pelstart = tms9918a_sprites[sprc].x + vptr;
                
			if(tms9918a_sprites[sprc].col)
			{
				byte = tms9918a_mem[tms9918a_sprites[sprc].mempos];
				tms9918a_sprites[sprc].mempos ++;

				if(tms9918a_regs[1]&2)
				{
					byte&128? bmp_write8(pelstart++, tms9918a_sprites[sprc].col) : pelstart++;
					byte&64?  bmp_write8(pelstart++, tms9918a_sprites[sprc].col) : pelstart++;
					byte&32?  bmp_write8(pelstart++, tms9918a_sprites[sprc].col) : pelstart++;
					byte&16?  bmp_write8(pelstart++, tms9918a_sprites[sprc].col) : pelstart++;
					byte&8?   bmp_write8(pelstart++, tms9918a_sprites[sprc].col) : pelstart++;
					byte&4?   bmp_write8(pelstart++, tms9918a_sprites[sprc].col) : pelstart++;
					byte&2?   bmp_write8(pelstart++, tms9918a_sprites[sprc].col) : pelstart++;
					byte&1?   bmp_write8(pelstart++, tms9918a_sprites[sprc].col) : pelstart++;

					byte = tms9918a_mem[tms9918a_sprites[sprc].mempos+15];

					byte&128? bmp_write8(pelstart++, tms9918a_sprites[sprc].col) : pelstart++;
					byte&64?  bmp_write8(pelstart++, tms9918a_sprites[sprc].col) : pelstart++;
					byte&32?  bmp_write8(pelstart++, tms9918a_sprites[sprc].col) : pelstart++;
					byte&16?  bmp_write8(pelstart++, tms9918a_sprites[sprc].col) : pelstart++;
					byte&8?   bmp_write8(pelstart++, tms9918a_sprites[sprc].col) : pelstart++;
					byte&4?   bmp_write8(pelstart++, tms9918a_sprites[sprc].col) : pelstart++;
					byte&2?   bmp_write8(pelstart++, tms9918a_sprites[sprc].col) : pelstart++;
					byte&1?   bmp_write8(pelstart++, tms9918a_sprites[sprc].col) : pelstart++;
				}
				else
				{
					byte&128? bmp_write8(pelstart++, tms9918a_sprites[sprc].col) : pelstart++;
					byte&64?  bmp_write8(pelstart++, tms9918a_sprites[sprc].col) : pelstart++;
					byte&32?  bmp_write8(pelstart++, tms9918a_sprites[sprc].col) : pelstart++;
					byte&16?  bmp_write8(pelstart++, tms9918a_sprites[sprc].col) : pelstart++;
					byte&8?   bmp_write8(pelstart++, tms9918a_sprites[sprc].col) : pelstart++;
					byte&4?   bmp_write8(pelstart++, tms9918a_sprites[sprc].col) : pelstart++;
					byte&2?   bmp_write8(pelstart++, tms9918a_sprites[sprc].col) : pelstart++;
					byte&1?   bmp_write8(pelstart++, tms9918a_sprites[sprc].col) : pelstart++;
				}
			}
			tms9918a_sprites[sprc].c--;
		}
                
		if((!tms9918a_sprites[sprc].c) && (tms9918a_sprites[sprc].on))
			find_next_sprite(&tms9918a_sprites[sprc], logy);
	}
}


void tms9918a_noline(int scry, int logy)
{
	textprintf_centre(screen, font, 160, 116, 8, "unknown mode : %d", (tms9918a_regs[0] & 2) | (tms9918a_regs[1] & 24));
}

void tms9918a_mode0line(int scry, int logy)
{
	unsigned long vptr, vptr2;
	int count;
	unsigned char byte, bcol, fcol;

	vptr2 = vptr = bmp_write_line(screen, scry);
	bmp_select(screen);
        
	vptr+=32;
	count = 32;
	while(count--)
	{
		byte = tms9918a_mem[ pg + (tms9918a_mem[pn] << 3) + (logy&7) ];
		bcol = tms9918a_mem[ ct + (tms9918a_mem[pn] >> 3)];
		fcol = bcol >> 4;
		bcol &= 15;

		bmp_write8(vptr++, byte&128 ? fcol : bcol);
		bmp_write8(vptr++, byte&64  ? fcol : bcol);
		bmp_write8(vptr++, byte&32  ? fcol : bcol);
		bmp_write8(vptr++, byte&16  ? fcol : bcol);
		bmp_write8(vptr++, byte&8   ? fcol : bcol);
		bmp_write8(vptr++, byte&4   ? fcol : bcol);
		bmp_write8(vptr++, byte&2   ? fcol : bcol);
		bmp_write8(vptr++, byte&1   ? fcol : bcol);

		pn++;
	}

	if(((logy+1)&7))
		pn -= 32;

	handle_sprites(vptr - 256, logy);
        
	count = 32;
	while(count--)
	{
		bmp_write8(vptr++, bordercol);
		bmp_write8(vptr2++, bordercol);
	}

	bmp_unwrite_line(screen);
}

void tms9918a_mode1line(int scry, int logy)
{
	unsigned long vptr, vptr2;
	int count;
	unsigned char byte;// bcol, fcol;

	vptr2 = vptr = bmp_write_line(screen, scry);
	bmp_select(screen);
        
	vptr+=40;
	count = 40;
	while(count--)
	{
		byte = tms9918a_mem[ pg + (tms9918a_mem[pn] << 3) + (logy&7) ];

		bmp_write8(vptr++, byte&32  ? textcol : bordercol);
		bmp_write8(vptr++, byte&16  ? textcol : bordercol);
		bmp_write8(vptr++, byte&8   ? textcol : bordercol);
		bmp_write8(vptr++, byte&4   ? textcol : bordercol);
		bmp_write8(vptr++, byte&2   ? textcol : bordercol);
		bmp_write8(vptr++, byte&1   ? textcol : bordercol);

		pn++;
	}

	if(((logy+1)&7))
		pn -= 40;

	count = 40;
	while(count--)
	{
		bmp_write8(vptr++, bordercol);
		bmp_write8(vptr2++, bordercol);
	}

	bmp_unwrite_line(screen);
}

void tms9918a_keepalive(void)
{
//	int c2;
//	unsigned char tmodenum;
//	PALETTE pal;

	if(scancount == 279)
	{
		tms9918a_sr = (tms9918a_sr & ~31) | lastnum;
		tms9918a_sr |= 128;
		if(tms9918a_regs[1]&32)
			z80_do_interrupt();

		c = scancount = 0;
		y = 24;
		pn = pnbase;
		memset(sprfree, 0, 32);
		lastnum = find_last_sprite();

		find_next_sprite(&tms9918a_sprites[0], c);
		find_next_sprite(&tms9918a_sprites[1], c);
		find_next_sprite(&tms9918a_sprites[2], c);
		find_next_sprite(&tms9918a_sprites[3], c);
        }
        
	if(scancount > 87)
	{
		tms9918a_pelline(y, c);
		c++;
		y++;
	}
	scancount++;
}

void set_tms9918a_palette(void)
{
	PALETTE pal;
	int c = 0;

	pal[c].g =
	pal[c].r =
	pal[c].b = 0;
	c++;
        
	pal[c].g =
	pal[c].r =
	pal[c].b = 0;
	c++;
        
	pal[c].g = 6 << 3;
	pal[c].r =
	pal[c].b = 1 << 3;
	c++;
        
	pal[c].g = 7 << 3;
	pal[c].r =
	pal[c].b = 3 << 3;
	c++;
        
	pal[c].g =
	pal[c].r = 1 << 3;
	pal[c].b = 7 << 3;
	c++;
        
	pal[c].g = 3 << 3;
	pal[c].r = 2 << 3;
	pal[c].b = 7 << 3;
	c++;
        
	pal[c].g = 1 << 3;
	pal[c].r = 5 << 3;
	pal[c].b = 1 << 3;
	c++;
        
	pal[c].g = 6 << 3;
	pal[c].r = 2 << 3;
	pal[c].b = 7 << 3;
	c++;
        
	pal[c].g = 1 << 3;
	pal[c].r = 7 << 3;
	pal[c].b = 1 << 3;
	c++;
        
	pal[c].g = 3 << 3;
	pal[c].r = 7 << 3;
	pal[c].b = 3 << 3;
	c++;
        
	pal[c].g = 6 << 3;
	pal[c].r = 6 << 3;
	pal[c].b = 1 << 3;
	c++;
        
	pal[c].g = 6 << 3;
	pal[c].r = 6 << 3;
	pal[c].b = 4 << 3;
	c++;
        
	pal[c].g = 4 << 3;
	pal[c].r = 1 << 3;
	pal[c].b = 1 << 3;
	c++;
        
	pal[c].g = 2 << 3;
	pal[c].r = 6 << 3;
	pal[c].b = 5 << 3;
	c++;
        
	pal[c].g = 5 << 3;
	pal[c].r = 5 << 3;
	pal[c].b = 5 << 3;
	c++;
        
	pal[c].g = 7 << 3;
	pal[c].r = 7 << 3;
	pal[c].b = 7 << 3;
	c++;

	set_palette(pal);
}

int tms9918a_setup(void)
{
	set_gfx_mode(GFX_AUTODETECT, 320, 240, 0, 0);
	set_tms9918a_palette();
	tms9918a_mem = malloc(16*1024);
	tms9918a_sr = bytebit = 0;
	scancount = 279;
	tms9918a_pelline = tms9918a_noline;
	return 0;
}

void tms9918a_end(void)
{
	free(tms9918a_mem);
}

unsigned char tms9918a_receivebyte(unsigned char port)
{
	unsigned char ret;

	if(port)
	{
		ret = tms9918a_sr;
		tms9918a_sr &= 95; //255 - 128 - 32
	}
	else
	{
		ret = rahead;
		rahead = tms9918a_mem[memaddr.full];
		inc_memaddr();
	}

	bytebit = 0;
        
	return ret;
}

void tms9918a_reevalmode(void)
{
	unsigned char modenum;

	modenum = (tms9918a_regs[0] & 2) | (tms9918a_regs[1] & 24);
	pgand = 7;
	ctand = 255;
                        
	switch(modenum)
	{
		case 0 :
			tms9918a_pelline = tms9918a_mode0line;
		break;
                        
		case 16 :
			tms9918a_pelline = tms9918a_mode1line;
		break;
                        
		case 2 :
			tms9918a_pelline = tms9918a_noline;
			fprintf(stderr, "Switched to tms mode 2\n");
		break;
                        
		case 8 :
			tms9918a_pelline = tms9918a_noline;
			fprintf(stderr, "Switched to tms mode 3\n");
		break;
	}
        
	pg = (unsigned short)(tms9918a_regs[4]&pgand) << 11;
	ct = (unsigned short)(tms9918a_regs[3]&ctand) << 6;
}

void tms9918a_changereg(unsigned char regnum, unsigned char newval)
{
//	unsigned char temp;
	tms9918a_regs[regnum] = newval;

	switch(regnum)
	{
		case 0 : //bit 1 = mode bit 2, bit 0 = b/w bit
			tms9918a_reevalmode();
		break;

		case 1 : //mode bits, sprite details (scale + size), gint bit, screen on
			tms9918a_reevalmode();
			sprdim = 8;

			if(tms9918a_regs[1]&2)
				sprdim = 16;

			if(tms9918a_regs[1]&1)
				sprdim <<= 1;

			if((tms9918a_regs[1]&32) && (tms9918a_sr & 128))
				z80_do_interrupt();
		break;

		case 2 : //pn addr
			pnbase = pn = (unsigned short)(tms9918a_regs[2]&15) << 10;
		break;

		case 3 : //ct addr
			ct = (unsigned short)(tms9918a_regs[3]&ctand) << 6;
		break;

		case 4 : //pg addr
			pg = (unsigned short)(tms9918a_regs[4]&pgand) << 11;
		break;

		case 5 : //sa addr
			sa = (unsigned short)(tms9918a_regs[5]&127) << 7;
		break;
	
		case 6 : //sg addr
			sg = (unsigned short)(tms9918a_regs[6]&7) << 11;
		break;

		case 7 : //background and foreground colours
			bordercol = newval & 15;
			textcol = newval >> 4;
		break;
	}
}

void tms9918a_sendbyte(unsigned char byte, unsigned char port)
{
	if(port) //port 1
	{
		if(bytebit)
		{
			if(byte&128) //a register write
			{
				//tms9918a_regs[byte&7] = lowbyte;
				tms9918a_changereg(byte&7, lowbyte);
			}
			else //a memory write
			{
				memaddr.bytes.low = lowbyte;
				memaddr.bytes.high = byte & 63;
                                
				if(!(byte&64))
				{
					rahead = tms9918a_mem[memaddr.full];
					inc_memaddr();
				}
			}
			bytebit = 0;
		}
		else
		{
			lowbyte = byte;
			bytebit = 1;
		}
                
//		bytebit ^= 1;
	}
	else // port 0
	{
		tms9918a_mem[memaddr.full] = byte;
		rahead = byte;
		inc_memaddr();
		bytebit = 0;
	}
}
