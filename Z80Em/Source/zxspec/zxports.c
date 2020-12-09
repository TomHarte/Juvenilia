#include "../ay38912/ay38912.h"
#include "zxspec.h"
#include "../fdc8272/fdc8272.h"
#include "zxports.h"
#include "zxkeyboard.h"
#include "../z80/z80.h"
int romno;

unsigned char get_keyboard_byte(unsigned char top)
{
	unsigned char result;

	result = 31;

	result &= top&0x01 ? 31 : keybytes[0];
	result &= top&0x02 ? 31 : keybytes[1];
	result &= top&0x04 ? 31 : keybytes[2];
	result &= top&0x08 ? 31 : keybytes[3];
	result &= top&0x10 ? 31 : keybytes[4];
	result &= top&0x20 ? 31 : keybytes[5];
	result &= top&0x40 ? 31 : keybytes[6];
	result &= top&0x80 ? 31 : keybytes[7];

	return result | 160;
}

void zxspec_48port_out(unsigned short portnum, unsigned char data)
{
	switch(portnum)
	{
		default :
			switch(portnum & 255)
			{
				case 0xfe : //set border colour, speaker on/off and tape output on/off
					zx_bcol = data&7;
/*					if(((data&16) << 3) != (beep & 128))
					{
						beep = ((beep &127) + 1) | ((data&16) << 3);
					}*/
//					voice_set_volume(zxchan, data&16 ? 255 : 0);
				break;
			}
		break;
	}
}

void zxspec_48port_in(unsigned short portnum, unsigned char *data)
{
	switch(portnum)
	{
		default :
			switch(portnum & 255)
			{
				case 0xfe : //read keyboard
					*data = get_keyboard_byte(portnum >> 8);
				break;

				default :
					*data = 0;
				break;
			}
		break;
	}
}

void zx_page_in(int dest, int source)
{
	dest <<= 1;
	source <<= 1;

	z80_bankzones[dest] = z80_zones[source];
	z80_bankzones[dest+1] = z80_zones[source+1];
        
	source <<= 13;
	writebanks[dest] = readbanks[dest] = &mempool[source];
	source += 8192;
	dest++;
	writebanks[dest] = readbanks[dest] = &mempool[source];
}

void zxspec_128port_out(unsigned short portnum, unsigned char data)
{
//	fprintf(stderr, "out (%04x), %d : ", portnum, data);

	if((portnum & 255) == 0xfe)
	{
		zx_bcol = data&7;
/*		reallocate_voice(zxchan, data&16? zxtoneh : zxtonel);
		voice_set_playmode(zxchan, PLAYMODE_LOOP);
		voice_start(zxchan);
		if(((data&16) << 3) != (beep & 128))
		{
			beep = ((beep &127) + 1) | ((data&16) << 3);
		}*/
//		voice_set_volume(zxchan, data&16 ? 255 : 0);
//		fprintf(stderr, "speaker note : %s [%d]\n", data&16 ? "zxtoneh" : "zxtonel", tstates);
//		fprintf(stderr, "ULA out (%04x), %d : ", portnum, data);
//		fprintf(stderr, "ULA\n");
	}
        
	if(!(portnum&2))
	{
		switch(portnum >> 14)
		{
			case 1 :
				zx_page_in(3, data&7);

				vmem_offset = (data&8) ? 7 : 5;
				vmem_offset <<= 14;

				zx_page_in(0, (data&16) ? 9 : 8);

				if(data&32)
					z80_set_port_out_function(zxspec_48port_out);
//				fprintf(stderr, "mem page\n");
			break;
                
			case 2 :
				ay_setmode(REGISTER_WRITE);
				ay_sendbyte(data);
//				fprintf(stderr, "register write\n");
			break;

			case 3 :
				ay_setmode(REGISTER_SELECT);
				ay_sendbyte(data&15);
//				fprintf(stderr, "register select\n");
			break;
		}
	}
}

void zxspec_128port_in(unsigned short portnum, unsigned char *data)
{
	*data = 0;

	if(!(portnum&1))
	{
		*data = get_keyboard_byte(portnum >> 8);
	}

	if(!(portnum&2))
	{
		switch(portnum >> 14)
		{
			case 3 :
				ay_setmode(REGISTER_READ);
				*data = ay_receivebyte();
			break;
		}
	}
}

void zxspec_pl3port_out(unsigned short portnum, unsigned char data)
{
	switch(portnum)
	{
		default :
			switch(portnum & 255)
			{
				case 0xfe : //set border colour, speaker on/off and tape output on/off
					zx_bcol = data&7;
/*					if(((data&16) << 3) != (beep & 128))
					{
						beep = ((beep &127) + 1) | ((data&16) << 3);
					}*/
					//voice_set_volume(zxchan, data&16 ? 255 : 0);
				break;
			}
		break;

		case 0x1ffd : //+2a/+3 memory paging
			fdc8272_set_motor(data&8);
			if(data&1)
			{//special mode
				switch(data&6)
				{
					case 0 :
						zx_page_in(0, 0);
						zx_page_in(1, 1);
						zx_page_in(2, 2);
						zx_page_in(3, 3);
					break;

					case 2 :
						zx_page_in(0, 4);
						zx_page_in(1, 5);
						zx_page_in(2, 6);
						zx_page_in(3, 7);
					break;

					case 4 :
						zx_page_in(0, 4);
						zx_page_in(1, 5);
						zx_page_in(2, 6);
						zx_page_in(3, 3);
					break;

					case 6 :
						zx_page_in(0, 4);
						zx_page_in(1, 7);
						zx_page_in(2, 6);
						zx_page_in(3, 3);
					break;
				}
			}
			else
			{//normal mode
				romno = (romno&1) | ((data&4) >> 1);
				zx_page_in(0, 8+romno);
			}
		break;

		case 0x3ffd :
			fdc8272_send(data);
                break;

		case 0x7ffd : //ram paging
			zx_page_in(3, data&7);

			vmem_offset = (data&8) ? 7 : 5;
			vmem_offset <<= 14;

			romno = romno&2 | ((data&16) >> 4);
			zx_page_in(0, 8+romno);

			if(data&32)
				z80_set_port_out_function(zxspec_48port_out);
		break;
                
		case 0xbffd : //ay register write
			ay_setmode(REGISTER_WRITE);
			ay_sendbyte(data);
		break;
                
		case 0xfffd : //ay register select
			ay_setmode(REGISTER_SELECT);
			ay_sendbyte(data);
		break;
	}
}

void zxspec_pl3port_in(unsigned short portnum, unsigned char *data)
{
	switch(portnum)
	{
		default :
			switch(portnum & 255)
			{
				default :
                                        *data = 255;
				break;

				case 0xfe : //read keyboard
					*data = get_keyboard_byte(portnum >> 8);
				break;
			}
		break;

		case 0x2ffd :
			*data = fdc8272_get_status();
                break;

		case 0x3ffd :
			*data = fdc8272_read();
                break;
                
		case 0xfffd : //ay register read
			ay_setmode(REGISTER_READ);
			*data = ay_receivebyte();
		break;
	}
}
