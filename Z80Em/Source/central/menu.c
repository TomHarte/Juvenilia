#include "menu.h"

BITMAP *scr, *map;
DATAFILE *maindata;

unsigned char go_menu(void)
{
	unsigned char leave = 0;
        
	blit(scr, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
	show_mouse(screen);
	while(!leave)
	{
		if(mouse_b&1)
                	leave = getpixel(map, mouse_x, mouse_y);
	}
        
	show_mouse(NULL);
	return leave;
}
