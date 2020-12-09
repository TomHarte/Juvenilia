#include <allegro.h>
#include <stdio.h>
#include "colports.h"
#include "../tms9918a/tms9918a.h"
#include "../sn76489a/sn76489a.h"
#include "../z80/z80.h"

unsigned char joymode;

void cvision_port_out(unsigned short portnum, unsigned char data)
{
	switch(portnum & 224)
	{
		case 128 : //controls - set to keypad mode
			joymode = 0;
		break;
                
		case 160 : //video, A0 used to set port 1 or 0
			tms9918a_sendbyte(data, portnum&1);
		break;
                
		case 192 : //controls - set to joystick mode
			joymode = 1;
		break;
                
		case 224 : //sound
			sn76489a_sendbyte(data);
		break;
	}
}

void cvision_port_in(unsigned short portnum, unsigned char *data)
{
	switch(portnum & 224)
	{
		case 160 : //video, A0 used to set port 1 or 0
			*data = tms9918a_receivebyte(portnum&1);
		break;
                
		case 224 : //controls - A1 used to select ctrl 1 or 2
			if(joymode) // read joystick
			{
				*data = 255;

				if(key[KEY_DOWN])
					*data &= ~1;
                                        
				if(key[KEY_UP])
					*data &= ~4;
                                        
                                        
				if(key[KEY_LEFT])
					*data &= ~8;
                                        
				if(key[KEY_RIGHT])
					*data &= ~2;
                                        
                                        
				if(key[KEY_LCONTROL])
					*data &= ~64;
			}
			else //read keypad
			{
				*data = 15;
                        
				if(key[KEY_0])
					*data = 10;
                                        
				if(key[KEY_1])
					*data = 13;
                                        
				if(key[KEY_2])
					*data = 7;
                                        
				if(key[KEY_3])
					*data = 12;
                                        
				if(key[KEY_4])
					*data = 2;
                                        
				if(key[KEY_5])
					*data = 3;
                                        
				if(key[KEY_6])
					*data = 14;
                                        
				if(key[KEY_7])
					*data = 5;
                                        
				if(key[KEY_8])
					*data = 1;
                                        
				if(key[KEY_9])
					*data = 11;
                                        
				if(key[KEY_OPENBRACE])
					*data = 9;
                                        
				if(key[KEY_CLOSEBRACE])
					*data = 6;
                                        
				*data |= 240;

				if(key[KEY_SPACE])
					*data &= ~64;
			}
		break;
	}
}

