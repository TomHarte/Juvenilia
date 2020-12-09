#include "../all.h"
#include <allegro.h>
#include "../central/filesel.h"
#include "cpcports.h"
#include "cpcfile.h"
#include "../fdc8272/fdc8272.h"
#include "../fdc8272/fdc8272file.h"
#include "crtc.h"
#include "cpctimes.h"
#include "../z80/z80.h"
#include "cpc.h"
#include <time.h>

unsigned char crtcentry, palentry, crtcregs[18], crtcsize[18], fakecrtc[18];
PALETTE prepal;

int rombase;

int scrmode, oldscrmode;
unsigned char portc, toprom, keylines[10];
void do_screen(void);
unsigned char mcount;
int bcount;

void cpc_writemem(void)
{
	unsigned char  bank;
	unsigned short offs;

	switch(z80_outflag)
	{
		case 1 :
			bank = bank_no(z80_outaddr.full);
			offs = offset(z80_outaddr.full);
                        writebanks[bank][offs] = z80_outword.bytes.low;
		break;

		case 2 :
			bank = bank_no(z80_outaddr.full);
			offs = offset(z80_outaddr.full);
                        writebanks[bank][offs] = z80_outword.bytes.low;
                        
			z80_outaddr.full++;
			bank = bank_no(z80_outaddr.full);
			offs = offset(z80_outaddr.full);
                        writebanks[bank][offs] = z80_outword.bytes.high;
		break;
	}
	z80_outflag = 0;
}

void leave_cpc(void)
{
	free(mempool);
	release_bitmap(screen);
}

int list_amstrad(void)
{
	create_sub_options(3);
	name_option(2, "CPC464");
	name_option(1, "CPC664");
	name_option(0, "CPC6128");
}

void cpc_interrupt(void)
{
	char *name;

	crtc_line();

	if(key[KEY_F1])
	{
		if(name = select_file("dsk", "Amstrad CPC Drive 1", "DSK Disk Images"))
		{
			fdc8272_opendsk(disk1, name);
			free(name);
		}
//		timer_targ = uclock();
	}
        
	if(key[KEY_F2])
	{
		if(name = select_file("dsk", "Amstrad CPC Drive 2", "DSK Disk Images"))
		{
			fdc8272_opendsk(disk2, name);
			free(name);
		}
//		timer_targ = uclock();
	}
        
	if(key[KEY_F3])
	{
		if(name = select_file("sna", "Amstrad CPC", "SNA Snapshots"))
		{
			cpc_loadsna(name);
			free(name);
		}
//		timer_targ = uclock();
	}
/*	while(uclock() < timer_targ)
	{
	}
        
	timer_targ += timer_targadd;*/
        
	bcount++;
	if(bcount == 312)
	{
		bcount = 0;
		cpc_update_keylines();
	}
}

void cpc_babyinterrupt(int statenum)
{ //I go 125000 times a second
	z80_set_breakpoint(z80_breakpoint() + 32);
	ay_keepalive();
	fdc8272_keepalive();
	if(!(mcount++&7))
	{
		cpc_interrupt();
	}
}

void setup_prepal(void)
{
	int c= 0;

        #define FULLCOL 63
        #define HALFCOL 42
        #define NONECOL 0

	prepal[c].r =
	prepal[c].g =
	prepal[c].b = 0;
	c++;
        
	//0

	prepal[c].r =
	prepal[c].g =
	prepal[c].b = HALFCOL;
	c++;

	prepal[c].r =
	prepal[c].g =
	prepal[c].b = HALFCOL;
	c++;

	prepal[c].r = NONECOL;
	prepal[c].g = FULLCOL;
	prepal[c].b = HALFCOL;
	c++;

	prepal[c].r =
	prepal[c].g = FULLCOL;
	prepal[c].b = HALFCOL;
	c++;

	prepal[c].r =
	prepal[c].g = NONECOL;
	prepal[c].b = HALFCOL;
	c++;

	prepal[c].r = FULLCOL;
	prepal[c].g = NONECOL;
	prepal[c].b = HALFCOL;
	c++;

	prepal[c].r = NONECOL;
	prepal[c].g =
	prepal[c].b = HALFCOL;
	c++;

	prepal[c].r = FULLCOL;
	prepal[c].g =
	prepal[c].b = HALFCOL;
	c++;

	prepal[c].r = FULLCOL;
	prepal[c].g = NONECOL;
	prepal[c].b = HALFCOL;
	c++;

	prepal[c].r =
	prepal[c].g = FULLCOL;
	prepal[c].b = HALFCOL;
	c++;

	//10

	prepal[c].r =
	prepal[c].g = FULLCOL;
	prepal[c].b = NONECOL;
	c++;

	prepal[c].r =
	prepal[c].g =
	prepal[c].b = FULLCOL;
	c++;

	prepal[c].r = FULLCOL;
	prepal[c].g =
	prepal[c].b = NONECOL;
	c++;

	prepal[c].r = FULLCOL;
	prepal[c].g = NONECOL;
	prepal[c].b = FULLCOL;
	c++;

	prepal[c].r = FULLCOL;
	prepal[c].g = HALFCOL;
	prepal[c].b = NONECOL;
	c++;

	prepal[c].r = FULLCOL;
	prepal[c].g = HALFCOL;
	prepal[c].b = FULLCOL;
	c++;

	prepal[c].r =
	prepal[c].g = NONECOL;
	prepal[c].b = HALFCOL;
	c++;

	prepal[c].r = NONECOL;
	prepal[c].g = FULLCOL;
	prepal[c].b = HALFCOL;
	c++;

	prepal[c].r = NONECOL;
	prepal[c].g = FULLCOL;
	prepal[c].b = NONECOL;
	c++;

	prepal[c].r = NONECOL;
	prepal[c].g =
	prepal[c].b = FULLCOL;
	c++;

	//20

	prepal[c].r =
	prepal[c].g =
	prepal[c].b = NONECOL;
	c++;

	prepal[c].r =
	prepal[c].g = NONECOL;
	prepal[c].b = FULLCOL;
	c++;

	prepal[c].r = NONECOL;
	prepal[c].g = HALFCOL;
	prepal[c].b = NONECOL;
	c++;

	prepal[c].r = NONECOL;
	prepal[c].g = HALFCOL;
	prepal[c].b = FULLCOL;
	c++;

	prepal[c].r = HALFCOL;
	prepal[c].g = NONECOL;
	prepal[c].b = HALFCOL;
	c++;

	prepal[c].r = HALFCOL;
	prepal[c].g = FULLCOL;
	prepal[c].b = HALFCOL;
	c++;

	prepal[c].r = HALFCOL;
	prepal[c].g = FULLCOL;
	prepal[c].b = NONECOL;
	c++;

	prepal[c].r = HALFCOL;
	prepal[c].g =
	prepal[c].b = FULLCOL;
	c++;

	prepal[c].r = HALFCOL;
	prepal[c].g =
	prepal[c].b = NONECOL;
	c++;

	prepal[c].r = HALFCOL;
	prepal[c].g = NONECOL;
	prepal[c].b = FULLCOL;
	c++;

	//30

	prepal[c].r =
	prepal[c].g = HALFCOL;
	prepal[c].b = NONECOL;
	c++;

	prepal[c].r =
	prepal[c].g = HALFCOL;
	prepal[c].b = FULLCOL;
	c++;
	set_palette(prepal);

}

int go_amstrad(int id)
{
	int c;
	PACKFILE *romstream;

	z80_set_normal_opset();
	set_cpc_timings();
	fdc8272_setup();
	ay_setup();

        clear(screen);
	setup_prepal();
        crtc_setup();

	bcount = 0;

	mempool = malloc(176*1024);
	c = 8;
	while(c--)
	{
		writebanks[c] = readbanks[c] = &mempool[c << 13];
	}

	if(romstream = pack_fopen("cpc/roms.dat#6128ROM", "r"))
	{
		for(c = 131072; c < 163840; c++)
		{
			pack_fread(&mempool[c], 1, romstream);
		}

		pack_fclose(romstream);
	}

	if(romstream = pack_fopen("cpc/roms.dat#AMSDOS", "r"))
	{
		for(c = 163840; c < 180224; c++)
		{
			pack_fread(&mempool[c], 1, romstream);
		}

		pack_fclose(romstream);
	}

	readbanks[0] = &mempool[16 << 13];
	readbanks[1] = &mempool[17 << 13];
	readbanks[6] = &mempool[18 << 13];
	readbanks[7] = &mempool[19 << 13];
	portc = 255;

	crtcsize[0] =
	crtcsize[1] =
	crtcsize[2] = 255;
	crtcsize[3] = 255;
	crtcsize[4] = 127;
	crtcsize[5] = 31;
	crtcsize[6] =
	crtcsize[7] = 127;
	crtcsize[8] = 3;
	crtcsize[9] = 31;
	crtcsize[10] = 127;
	crtcsize[11] = 31;
	crtcsize[12] = 63;
	crtcsize[13] = 255;
	crtcsize[14] = 63;
	crtcsize[15] = 255;
	crtcsize[16] = 63;
	crtcsize[17] = 255;

	z80_set_port_out_function(cpc_port_out);
	z80_set_port_in_function(cpc_port_in);
	z80_set_exit_function(leave_cpc);
	z80_set_memwrite_function(cpc_writemem);
	z80_set_call_out_function(cpc_babyinterrupt);
	z80_set_tstate_counter(0);
	z80_set_breakpoint(256);
	z80_set_eicallback(NULL);

//	timer_targadd = UCLOCKS_PER_SEC / 15625;
//	timer_targ = uclock() + timer_targadd;

	go_z80();

	ay_end();
	fdc8272_end();
	crtc_end();
	return 0;
}
