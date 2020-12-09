#include <stdio.h>
#include "coleco.h"
#include "../z80/z80.h"
#include "colports.h"
#include "../tms9918a/tms9918a.h"
#include "../sn76489a/sn76489a.h"

#define COLECO_ROM 0
#define COLECO_RAM 1
#define COLECO_EXPANSION 2

void coleco_writemem(void)
{
	switch(z80_outflag)
	{
		case 1 :
			if(z80_outaddr.full > 0x5fff && z80_outaddr.full < 0x8000)
				mempool[z80_outaddr.full] = z80_outword.bytes.low;
		break;

		case 2 :
			if(z80_outaddr.full > 0x5fff && z80_outaddr.full < 0x8000)
				mempool[z80_outaddr.full] = z80_outword.bytes.low;
                        
			z80_outaddr.full++;
			if(z80_outaddr.full > 0x5fff && z80_outaddr.full < 0x8000)
				mempool[z80_outaddr.full] = z80_outword.bytes.high;
		break;
	}
	z80_outflag = 0;
}

int list_coleco(void)
{
	create_sub_options(1);
	name_option(0, "Vision");
	return 0;
}

void leave_coleco(void)
{
	tms9918a_end();
	sn76489a_end();
	free(mempool);
}

void coleco_next_scanline(int statenum)
{
	z80_set_breakpoint(z80_breakpoint() + 215);
	tms9918a_keepalive();
}

void load_cart(void)
{
	FILE *str;
	int c;
        
	if(str = fopen("d:/games/coleco/carnival.rom", "rb"))
	{
		c= 0x8000;
		while(!feof(str))
		{
			fread(&mempool[c], 1, 1, str);
			c++;
		}
        
		fclose(str);
	}
	else
		fprintf(stderr, "can't load\n");
        
}

int go_coleco(int id)
{
	int c;

	z80_set_normal_timings();
	z80_set_eicallback(NULL);
	z80_set_normal_opset();
	z80_set_port_out_function(cvision_port_out);
	z80_set_port_in_function(cvision_port_in);
	z80_set_exit_function(leave_coleco);
	z80_set_memwrite_function(coleco_writemem);
	z80_set_call_out_function(coleco_next_scanline);
	z80_set_tstate_counter(0);
	z80_set_breakpoint(0);
	tms9918a_setup();
	sn76489a_setup();
	mempool = malloc(64*1024);

	c = 8;
	while(c--)
	{
		writebanks[c] = readbanks[c] = &mempool[c << 13];
	}

	if(load_8kb_rom("coleco/roms.dat#VISIONROM", 0))
	{
		fprintf(stderr, "ROM load failed\n");
		return 1;
	}

	load_cart();

	go_z80();
	return 0;
}
