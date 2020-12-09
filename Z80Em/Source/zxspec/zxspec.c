#include "zxfile.h"
#include "../central/filesel.h"
#include "zxspec.h"
#include "zxports.h"
#include "zxscreen.h"
#include "../fdc8272/fdc8272.h"
#include "../fdc8272/fdc8272file.h"
#include "../ay38912/ay38912.h"
#include "../z80/z80.h"

unsigned char		*roms;
unsigned int drawsums[192], scannum, vmem_offset;
scanptr scrfuncs[312];
SAMPLE *zxtoneh, *zxtonel;
unsigned char zxchan, zx_bcol;

unsigned char zxcounter, zxcounter2;

void zxspec_endsound(void)
{
	deallocate_voice(zxchan);
	destroy_sample(zxtonel);
	destroy_sample(zxtoneh);
}

void zxspec_setupsound(void)
{
	int c;
        
	zxtoneh = create_sample(8, FALSE, 44100, 2048);
	for(c = 0; c < 2048; c++)
		((unsigned char *)zxtoneh->data)[c] = 255;//(c&8)*255;
                
	zxtonel = create_sample(8, FALSE, 44100, 2048);
	for(c = 0; c < 2048; c++)
		((unsigned char *)zxtonel->data)[c] = 0;
                
	zxchan = allocate_voice(zxtonel);
	voice_set_playmode(zxchan, PLAYMODE_LOOP);
	voice_start(zxchan);
}

void zxspec_writemem(void)
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
				case ZX_CONTENDED :
				case ZX_RAM :
					writebanks[bank][offs] = z80_outword.bytes.low;
				break;
			}
		break;

		case 2 :
			bank = bank_no(z80_outaddr.full);
			offs = offset(z80_outaddr.full);

			switch(z80_bankzones[bank])
			{
				case ZX_CONTENDED :
				case ZX_RAM :
					writebanks[bank][offs] = z80_outword.bytes.low;
				break;
			}
                        
			z80_outaddr.full++;
			bank = bank_no(z80_outaddr.full);
			offs = offset(z80_outaddr.full);

			switch(z80_bankzones[bank])
			{
				case ZX_CONTENDED :
				case ZX_RAM :
					writebanks[bank][offs] = z80_outword.bytes.high;
				break;
			}
		break;
	}
	z80_outflag = 0;
}


int list_sinclair(void)
{
	create_sub_options(4);
	name_option(3, "ZX Spectrum 16kb");
	name_option(2, "ZX Spectrum 48kb");
	name_option(1, "ZX Spectrum 128kb");
	name_option(0, "ZX Spectrum +3");
}

void leave_zxspec(void)
{
	free(mempool);
	free(z80_bankzones);
	if(z80_zones)
		free(z80_zones);

	release_bitmap(screen);
}

void zxspec_next_48scanline(int statenum)
{
	char *name;

	z80_set_breakpoint(z80_breakpoint() + 224);
	scrfuncs[scannum]();
	scannum++;
	zx_update_keyboard_bytes();
        
	if(key[KEY_F3])
	{
		if(name = select_file("sna", "ZX Spectrum", "SNA Snapshots"))
		{
			zxspec_loadsna(name);
			free(name);
		}
	}
        
	if(key[KEY_F4])
	{
		if(name = select_file("z80", "ZX Spectrum", "Z80 Snapshots"))
		{
			zxspec_loadz80(name);
			free(name);
		}
 	}
/*	zxcounter2++;
	if(zxcounter2&64)
	{
		zxcounter2 = 0;
		voice_set_frequency(zxchan, (beep&127)*8000000);
		fprintf(stderr, "f:[%d] - %d\n", beep, (beep&127)*8000000);
		beep &= 128;
	}*/
}

void zxspec_next_128scanline(void)
{
	char *name;

	scrfuncs[scannum]();
	scannum++;
	zx_update_keyboard_bytes();

	if(key[KEY_F1])
	{
		if(name = select_file("dsk", "ZX Spectrum Drive 1", "DSK Disk Images"))
		{
			fdc8272_opendsk(disk1, name);
			free(name);
		}
	}
        
	if(key[KEY_F2])
	{
		if(name = select_file("dsk", "ZX Spectrum Drive 2", "DSK Disk Images"))
		{
			fdc8272_opendsk(disk2, name);
			free(name);
		}
	}
        
	if(key[KEY_F3])
	{
		if(name = select_file("sna", "ZX Spectrum", "SNA Snapshots"))
		{
			zxspec_loadsna(name);
			free(name);
		}
	}
        
	if(key[KEY_F4])
	{
		if(name = select_file("z80", "ZX Spectrum", "Z80 Snapshots"))
		{
			zxspec_loadz80(name);
			free(name);
		}
	}
//	voice_set_frequency(zxchan, (beep&127)*15625);
//	beep &= 128;
}

void zxspec_next_128line(int statenum)
{
	z80_set_breakpoint(z80_breakpoint() + 28);
	fdc8272_keepalive();
	ay_keepalive();
	if(!(zxcounter++ & 7))
		zxspec_next_128scanline();
}

int common_setup(void)
{
	zxscr_table_setup();
	z80_set_normal_timings();
	zx_keyboard_setup();
	z80_set_eicallback(NULL);
	z80_set_normal_opset();
	return 0;
}

int common_4816setup(void)
{
	int c;
	PACKFILE *romstream;

	common_setup();

	mempool = malloc(64*1024);
	z80_zones = NULL;
	z80_bankzones = malloc(8);

	c = 8;
	while(c--)
	{
		writebanks[c] = readbanks[c] = &mempool[c << 13];
	}

	if(load_16kb_rom("zxspec/roms.dat#48ROM", 0))
	{
		fprintf(stderr, "ROM load failed\n");
		return 1;
	}

	vmem_offset = 16384;
	scannum = 0;

	z80_set_port_out_function(zxspec_48port_out);
	z80_set_port_in_function(zxspec_48port_in);
	z80_set_exit_function(leave_zxspec);
	z80_set_memwrite_function(zxspec_writemem);
	z80_set_call_out_function(zxspec_next_48scanline);
	z80_set_tstate_counter(0);
	z80_set_breakpoint(0);
        
	c = 0;
	scrfuncs[c] = init_scanlines;
	c++;

	for(; c < 40; c++)
		scrfuncs[c] = scanline_nothing;

	for(; c < 64; c++)
		scrfuncs[c] = scanline_border;

	for(; c < 256; c++)
		scrfuncs[c] = scanline_pixels;
                
	for(; c < 280; c++)
		scrfuncs[c] = scanline_border;

	for(; c < 311; c++)
		scrfuncs[c] = scanline_nothing;
                
	scrfuncs[c] = scanline_restart;

	return 0;
}

int setup_48kb(void)
{
	if(common_4816setup())
		return 1;
	z80_bankzones[0] = z80_bankzones[1] = ZX_ROM;
	z80_bankzones[2] = z80_bankzones[3] = ZX_CONTENDED;
	z80_bankzones[4] = z80_bankzones[5] =
	z80_bankzones[6] = z80_bankzones[7] = ZX_RAM;
}

int setup_16kb(void)
{
	int c;
	if(common_4816setup())
		return 1;
	z80_bankzones[0] = z80_bankzones[1] = ZX_ROM;
	z80_bankzones[2] = z80_bankzones[3] = ZX_CONTENDED;
	z80_bankzones[4] = z80_bankzones[5] =
	z80_bankzones[6] = z80_bankzones[7] = ZX_ROM;

        for(c = 32768; c < 65536; c++)
        	mempool[c] = 1;
}

void common_128setup(void)
{
	int c;

	common_setup();
	writebanks[0] = readbanks[0] = &mempool[131072];
	writebanks[1] = readbanks[1] = &mempool[139264];
	z80_bankzones[0] = z80_bankzones[1] = ZX_ROM;

	writebanks[2] = readbanks[2] = &mempool[5 << 14];
	writebanks[3] = readbanks[3] = &mempool[(5 << 14) + 8192];
	z80_bankzones[2] = z80_bankzones[3] = ZX_CONTENDED;

	writebanks[4] = readbanks[4] = &mempool[2 << 14];
	writebanks[5] = readbanks[5] = &mempool[(2 << 14) + 8192];
	z80_bankzones[4] = z80_bankzones[5] = ZX_RAM;

	writebanks[6] = readbanks[6] = &mempool[0 << 14];
	writebanks[7] = readbanks[7] = &mempool[(0 << 14) + 8192];
	z80_bankzones[6] = z80_bankzones[7] = ZX_RAM;

	z80_zones[0]  = z80_zones[1]  =
	z80_zones[2]  = z80_zones[3]  =
	z80_zones[4]  = z80_zones[5]  =
	z80_zones[6]  = z80_zones[7]  = ZX_RAM;
	z80_zones[8]  = z80_zones[9]  =
	z80_zones[10] = z80_zones[11] =
	z80_zones[12] = z80_zones[13] =
	z80_zones[14] = z80_zones[15] = ZX_CONTENDED;
	z80_zones[16] = z80_zones[17] =
	z80_zones[18] = z80_zones[19] = ZX_ROM;

	vmem_offset = 5 << 14;
	scannum = 0;
	z80_set_exit_function(leave_zxspec);
	z80_set_memwrite_function(zxspec_writemem);
	z80_set_call_out_function(zxspec_next_128line);
	z80_set_tstate_counter(0);
	z80_set_breakpoint(0);
        
	c = 0;
	scrfuncs[c] = init_scanlines;
	c++;

        for(; c < 40; c++)
        	scrfuncs[c] = scanline_nothing;

        for(; c < 63; c++)
        	scrfuncs[c] = scanline_border;

        for(; c < 255; c++)
        	scrfuncs[c] = scanline_pixels;
                
        for(; c < 279; c++)
        	scrfuncs[c] = scanline_border;

        for(; c < 310; c++)
        	scrfuncs[c] = scanline_nothing;
                
	scrfuncs[c] = scanline_restart;
	romno = 0;
}

int setup_128kb(void)
{
	int c;
	PACKFILE *romstream;

	mempool = malloc(160*1024);
	z80_zones = malloc(20);
	z80_bankzones = malloc(8);

	common_128setup();

	if(load_16kb_rom("zxspec/roms.dat#128ROM1", 131072))
		return 1;

	if(load_16kb_rom("zxspec/roms.dat#128ROM2", 147456))
		return 1;

	z80_set_port_out_function(zxspec_128port_out);
	z80_set_port_in_function(zxspec_128port_in);
}

int setup_pl3(void)
{
	int c;

	mempool = malloc(192*1024);
	z80_zones = malloc(24);
	z80_bankzones = malloc(8);
	z80_zones[20] = z80_zones[21] =
	z80_zones[22] = z80_zones[23] =  ZX_ROM;

	common_128setup();

	if(load_16kb_rom("zxspec/roms.dat#PL3ROM1", 131072))
		return 1;

	if(load_16kb_rom("zxspec/roms.dat#PL3ROM2", 147456))
		return 1;

	if(load_16kb_rom("zxspec/roms.dat#PL3ROM3", 163840))
		return 1;

	if(load_16kb_rom("zxspec/roms.dat#PL3ROM4", 180224))
		return 1;

	z80_set_port_out_function(zxspec_pl3port_out);
	z80_set_port_in_function(zxspec_pl3port_in);
}

int go_sinclair(int id)
{
	fdc8272_setup();
	ay_setup();
	zxspec_setupsound();
	switch(id)
	{
		case 0 : setup_pl3(); break;
		case 1 : setup_128kb(); break;
		case 2 : setup_48kb(); break;
		case 3 : setup_16kb(); break;
	}
	go_z80();
	zxspec_endsound();
	fdc8272_closedsk(disk1);
	fdc8272_end();
	ay_end();
}

