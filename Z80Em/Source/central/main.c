#include <stdio.h>
#include "../all.h"
#include "data.h"
#include "menu.h"
#include "../cpc/cpc.h"
#include "../zxspec/zxspec.h"
//#include "../gameboy/gb.h"
#include "../jace/jace.h"
#include "../coleco/coleco.h"
//#include "../msx/msx.h"
#include <string.h>
#include "../z80/z80.h"

int timer_targ, timer_targadd;
char *cdir;
unsigned char coldepth;

#ifdef DJGPP
BEGIN_GFX_DRIVER_LIST
	GFX_DRIVER_VBEAF
	GFX_DRIVER_VGA
	GFX_DRIVER_VESA3
	GFX_DRIVER_VESA2L
	GFX_DRIVER_VESA2B
	GFX_DRIVER_VESA1
//	GFX_DRIVER_MODEX
END_GFX_DRIVER_LIST
#endif

typedef int (*menufuncptr)(void);
typedef int (*gofuncptr)(int type);
typedef struct
{
	menufuncptr listfunc;
	char *name;
	gofuncptr gofunc;
	char *manshortcut;
} MANUFACTURER_OPTION;

MANUFACTURER_OPTION optionlist[] =
{
	{list_amstrad, "Amstrad", go_amstrad, "amstrad"},
//	{list_ascii, "Ascii", go_ascii},
	{list_coleco, "CoLeCo", go_coleco, "coleco"},
	{list_jace, "Jupiter Cantab", go_jace, "jace"},
//	{list_nintendo, "Nintendo", go_nintendo},
	{list_sinclair, "Sinclair", go_sinclair, "sinclair"},
	{NULL}
};

char **subchoices;
unsigned char numchoices;

void create_sub_options(unsigned char num)
{
	numchoices = num;
	subchoices = malloc(sizeof(char *)*num);
}

void name_option(unsigned char id, char *text)
{
	subchoices[id] = strdup(text);
}

void add_manufacturer_list(int *num)
{
	int x, y, c;

	x = 160;
	y = 80;
	c = 0;
        
	acquire_bitmap(screen);
	while(optionlist[c].listfunc)
	{
		textout_centre(scr, maindata[MainFont].dat, optionlist[c].name, x, y, 0);
		rectfill(map, x - (text_length(maindata[MainFont].dat, optionlist[c].name) >> 1), y, x + (text_length(maindata[MainFont].dat, optionlist[c].name) >> 1), y+text_height(maindata[MainFont].dat), c | 64);
		y += text_height(maindata[MainFont].dat);
		c++;
	}
	*num = c;

	x = 480;
	y = 80;
	c = numchoices;
	while(c--)
	{
		textout_centre(scr, maindata[MainFont].dat, subchoices[c], x, y, 0);
		rectfill(map, x - (text_length(maindata[MainFont].dat, subchoices[c]) >> 1), y, x + (text_length(maindata[MainFont].dat, subchoices[c]) >> 1), y+text_height(maindata[MainFont].dat), c | 128);
		y += text_height(maindata[MainFont].dat);
	}
	release_bitmap(screen);
}

void prepare_manufacturer_list(int *num)
{
	set_palette(maindata[MainPalette].dat);
	clear(map);
	rectfill(scr, 0, 0, SCREEN_W, SCREEN_H, 255);
	draw_sprite(scr, maindata[Title].dat, 0, 0);
	draw_sprite(scr, maindata[SelectMachine].dat, (SCREEN_W >> 1) - (((BITMAP *)maindata[SelectMachine].dat)->w >> 1), 52);
	draw_sprite(scr, maindata[StaticCircles].dat, 0, 72);
	draw_sprite(scr, maindata[QuitButton].dat, 0, SCREEN_H - ((BITMAP *)maindata[QuitButton].dat)->h);
	draw_sprite(map, maindata[ButtonMap].dat, 0, SCREEN_H - ((BITMAP *)maindata[QuitButton].dat)->h);
	add_manufacturer_list(num);
}

void do_manufacturer_menu(void)
{
	int clickid, cllist, num, done = 0;

	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
	scr = create_bitmap(SCREEN_W, SCREEN_H);
	map = create_bitmap(SCREEN_W, SCREEN_H);
        
	prepare_manufacturer_list(&num);

	while(!done)
	{
		switch(clickid = go_menu())
		{
			default :
				if(clickid & 64)
				{
					clickid &= 63;
					optionlist[clickid].listfunc();
                                        cllist = clickid;
				}
				else
				{
					clickid &= 63;
					hard_reset();
					optionlist[cllist].gofunc(clickid);
					
					set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
					set_palette(maindata[MainPalette].dat);
				}
			break;
                
			case 1 :
				done = 1;
			break;
		}
		prepare_manufacturer_list(&num);
	}

	destroy_bitmap(scr);
	destroy_bitmap(map);
}

char *strip_directory(char *full)
{
	char *path, *temp;
        
	temp = strdup(full);
	path = temp + strlen(temp) - 1;
	while(*path != '/' && path >= temp)
		path--;
	*path = '\0';
        
	return temp;
}

void assess_graphics_modes(void)
{
	char *name, *orig;
	unsigned char c;

	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 320, 240, 0, 0);

	coldepth = 8;
	if(is_planar_bitmap(screen))
	{
		coldepth |= 64;
		fprintf(stderr, "Must deal with planar\n");
	}

	orig = name = strdup(gfx_driver->desc); //look for "X-Windows"
	name[9] = '\0';

	if(!strcmp(name, "X-Windows"))
	{
		c = 0;
                
                while(c<2)
		{
			if(*name == ',')
				c++;

			name++;
		}

		name++;

		if(*name == '1')
			switch(name[1])
			{
				case '5' :
					coldepth = (coldepth&~63)|15;
				break;
                                
				case '6' :
					coldepth = (coldepth&~63)|16;
				break;
			}
                        
		if(*name == '2')
			coldepth = (coldepth&~63)|24;
                        
		if(*name == '3')
			coldepth = (coldepth&~63)|32;
                        
//		fprintf(stderr, "effective bitdepth : %d\n", coldepth&63, *name, name[1]);
        }
        
	set_color_depth(coldepth&63);
	free(orig);
}

int main(int argc, char **argv)
{
	allegro_init();
	cdir = strip_directory(argv[0]);

	assess_graphics_modes();
//	uclock();
	z80_setup();
	text_mode(-1);
	install_keyboard();
	install_mouse();
        install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL);
	maindata = load_datafile("main.dat");
	numchoices = 0;
	do_manufacturer_menu();
	unload_datafile(maindata);
	free(cdir);

	return 0;
}

END_OF_MAIN();