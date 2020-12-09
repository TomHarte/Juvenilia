#include "jace.h"
#include "jaceports.h"
#include "../central/filesel.h"
#include "../z80/z80.h"
#include "../zxspec/zxspec.h"
#include "jacescr.h"
unsigned char jace_keybits[8];

void jace_writemem(void)
{
	unsigned char  bank;
	unsigned short offs;

	switch(z80_outflag)
	{
		case 1 :
			bank = bank_no(z80_outaddr.full);
			offs = offset(z80_outaddr.full);
			switch(z80_bankzones[bank])
			{
				case JACE_RAM :
					writebanks[bank][offs] = z80_outword.bytes.low;
				break;
			}
		break;

		case 2 :
			bank = bank_no(z80_outaddr.full);
			offs = offset(z80_outaddr.full);

			switch(z80_bankzones[bank])
			{
				case JACE_RAM :
					writebanks[bank][offs] = z80_outword.bytes.low;
				break;
			}
                        
			z80_outaddr.full++;
			bank = bank_no(z80_outaddr.full);
			offs = offset(z80_outaddr.full);

			switch(z80_bankzones[bank])
			{
				case JACE_RAM :
					writebanks[bank][offs] = z80_outword.bytes.high;
				break;
			}
		break;
	}
	z80_outflag = 0;
}

void jace_next_scanline(int statenum)
{
	z80_set_breakpoint(z80_breakpoint() + 208); //I don't know!
	scrfuncs[scannum]();
	scannum++;
}

int list_jace(void)
{
	create_sub_options(1);
	name_option(0, "Ace");
}

void leave_jace(void)
{
	free(mempool);
	free(z80_bankzones);
}

void set_jace_palette(void)
{
	PALETTE pal;
	int c;

	c = 256;
	while(c--)
	{
		pal[c].r =
		pal[c].g =
		pal[c].b = 63;
	}

	pal[0].r =
        pal[0].g =
        pal[0].b = 0;

	set_palette(pal);
}

int go_jace(int id)
{
	int c;

	set_gfx_mode(GFX_AUTODETECT, 320, 240, 0, 0);
	set_jace_palette();

	z80_set_normal_timings();
	z80_set_eicallback(NULL);
	z80_set_normal_opset();

	mempool = malloc(64*1024);
	z80_bankzones = malloc(8);
	z80_bankzones[0] = JACE_ROM;
	z80_bankzones[1] =
	z80_bankzones[2] = z80_bankzones[3] =
	z80_bankzones[4] = z80_bankzones[5] =
	z80_bankzones[6] = z80_bankzones[7] = JACE_RAM;
	scannum = 0;
        
	c = 8;
	while(c--)
	{
		writebanks[c] = readbanks[c] = &mempool[c << 13];
	}

	if(load_8kb_rom("jace/roms.dat#ROM", 0))
	{
		fprintf(stderr, "ROM load failed\n");
		return 1;
	}

        
	z80_set_port_out_function(jace_port_out);
	z80_set_port_in_function(jace_port_in);
	z80_set_exit_function(leave_jace);
	z80_set_memwrite_function(jace_writemem);
	z80_set_call_out_function(jace_next_scanline);
	z80_set_tstate_counter(0);
	z80_set_breakpoint(0);

	c = 0;
	scrfuncs[c] = jace_init_scanlines;
	c++;

	for(; c < 40; c++)
		scrfuncs[c] = jace_scanline_nothing;

	for(; c < 64; c++)
		scrfuncs[c] = jace_scanline_border;

	for(; c < 256; c++)
		scrfuncs[c] = jace_scanline_pixels;
                
	for(; c < 280; c++)
		scrfuncs[c] = jace_scanline_border;

	for(; c < 311; c++)
		scrfuncs[c] = jace_scanline_nothing;
                
	scrfuncs[c] = jace_scanline_restart;
        
	go_z80();
}
