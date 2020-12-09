#include "../all.h"
#include "crtc.h"
#include "../fdc8272/fdc8272.h"
#include "../ay38912/ay38912.h"
#include "cpckeyb.h"
#include "../z80/z80.h"
#include "cpc.h"
unsigned char f4byte, iostat;

void set_block(int target, int source)
{
	target <<= 1;
	source <<= 14;

	if(writebanks[target] == readbanks[target])
		readbanks[target] = writebanks[target] = &mempool[source];
	else
		writebanks[target] = &mempool[source];

	target++;
	source += 8192;

	if(writebanks[target] == readbanks[target])
		readbanks[target] = writebanks[target] = &mempool[source];
	else
		writebanks[target] = &mempool[source];
}

/*unsigned char get_keyboard(void)
{
	unsigned char ret;

	ret = 255;

	switch(keylines[9])
	{
		case 0 :
			if(key[KEY_DEL_PAD])
				ret &= ~128;

			if(key[KEY_ENTER])
				ret &= ~64;

			if(key[KEY_3_PAD])
				ret &= ~32;

			if(key[KEY_6_PAD])
				ret &= ~16;

			if(key[KEY_9_PAD])
				ret &= ~8;

			if(key[KEY_DOWN])
				ret &= ~4;

			if(key[KEY_RIGHT])
				ret &= ~2;

			if(key[KEY_UP])
				ret &= ~1;
		break;

		case 1 :
			if(key[KEY_0_PAD])
				ret &= ~128;

			if(key[KEY_2_PAD])
				ret &= ~64;

			if(key[KEY_1_PAD])
				ret &= ~32;

			if(key[KEY_5_PAD])
				ret &= ~16;

			if(key[KEY_8_PAD])
				ret &= ~8;

			if(key[KEY_7_PAD])
				ret &= ~4;

			if(key[KEY_LEFT])
				ret &= ~1;
		break;

		case 2 :
			if(key[KEY_LCONTROL] || key[KEY_RCONTROL])
				ret &= ~128;

			if(key[KEY_BACKSLASH])
				ret &= ~64;

			if(key[KEY_LSHIFT] || key[KEY_RSHIFT])
				ret &= ~32;

			if(key[KEY_4_PAD])
				ret &= ~16;

			if(key[KEY_CLOSEBRACE])
				ret &= ~8;

			if(key[KEY_ENTER])
				ret &= ~4;

			if(key[KEY_OPENBRACE])
				ret &= ~2;
		break;

		case 3 :
			if(key[KEY_STOP])
				ret &= ~128;

			if(key[KEY_SLASH])
				ret &= ~64;

			if(key[KEY_ASTERISK])
				ret &= ~32;

			if(key[KEY_EQUALS])
				ret &= ~2;

			if(key[KEY_QUOTE])
				ret &= ~4;
                                
			if(key[KEY_P])
				ret &= ~8;
		break;

		case 4 :
			if(key[KEY_COMMA])
				ret &= ~128;

			if(key[KEY_M])
				ret &= ~64;

			if(key[KEY_K])
				ret &= ~32;

			if(key[KEY_L])
				ret &= ~16;

			if(key[KEY_I])
				ret &= ~8;

			if(key[KEY_O])
				ret &= ~4;

			if(key[KEY_9])
				ret &= ~2;

			if(key[KEY_0])
				ret &= ~1;
		break;

		case 5 :
			if(key[KEY_SPACE])
				ret &= ~128;

			if(key[KEY_N])
				ret &= ~64;

			if(key[KEY_J])
				ret &= ~32;

			if(key[KEY_H])
				ret &= ~16;

			if(key[KEY_Y])
				ret &= ~8;

			if(key[KEY_U])
				ret &= ~4;

			if(key[KEY_7])
				ret &= ~2;

			if(key[KEY_8])
				ret &= ~1;
		break;

		case 6 :
			if(key[KEY_V])
				ret &= ~128;

			if(key[KEY_B])
				ret &= ~64;

			if(key[KEY_F])
				ret &= ~32;

			if(key[KEY_G])
				ret &= ~16;

			if(key[KEY_T])
				ret &= ~8;

			if(key[KEY_R])
				ret &= ~4;

			if(key[KEY_5])
				ret &= ~2;

			if(key[KEY_6])
				ret &= ~1;
		break;

		case 7 :
			if(key[KEY_X])
				ret &= ~128;

			if(key[KEY_C])
				ret &= ~64;

			if(key[KEY_D])
				ret &= ~32;

			if(key[KEY_S])
				ret &= ~16;

			if(key[KEY_W])
				ret &= ~8;

			if(key[KEY_E])
				ret &= ~4;

			if(key[KEY_3])
				ret &= ~2;

			if(key[KEY_4])
				ret &= ~1;
		break;

		case 8 :
			if(key[KEY_Z])
				ret &= ~128;

			if(key[KEY_CAPSLOCK])
				ret &= ~64;

			if(key[KEY_A])
				ret &= ~32;

			if(key[KEY_TAB])
				ret &= ~16;

			if(key[KEY_Q])
				ret &= ~8;

			if(key[KEY_ESC])
				ret &= ~4;

			if(key[KEY_2])
				ret &= ~2;

			if(key[KEY_1])
				ret &= ~1;
		break;

		case 9 :
			if(key[KEY_BACKSPACE])
				ret &= ~128;
		break;
	}

	return ret;
}*/

void cpc_port_out(unsigned short portnum, unsigned char data)
{
	unsigned char oldmode;

	if(!(portnum & 32768))
	{
		switch(data & 192)
		{
			case 0 : //select a pen to change colour
				palentry = data&31;
			break;

			case 64 : //set the pen colour
				clist->tstate = z80_tstates;
				clist->end = 0;
				clist->col = palentry;
				clist->newval = (data & 31) + 1;
				clist = clist->next;
				clist->end = 1;
			break;

			case 128 : //select screen mode and rom config
				scrmode = data & 3;

				if(scrmode == 3)
					scrmode = 0;
                                
				readbanks[0] = (data & 4) ? writebanks[0] : &mempool[131072];
				readbanks[1] = (data & 4) ? writebanks[1] : &mempool[139264];

				readbanks[6] = (data & 8) ? writebanks[6] : &mempool[rombase];
				readbanks[7] = (data & 8) ? writebanks[7] : &mempool[rombase + 8192];
				toprom = data & 8;

				if(data & 16)
				{
					sccounter &= 31;
					z80_set_eicallback(NULL);
				}
			break;

			case 192 : //set the ram config (paging)
				switch(data & 7)
				{
					case 0 :
						set_block(0,0);
						set_block(1,1);
						set_block(2,2);
						set_block(3,3);
					break;

					case 1 :
						set_block(0,0);
						set_block(1,1);
						set_block(2,2);
						set_block(3,7);
					break;

					case 2 :
						set_block(0,4);
						set_block(1,5);
						set_block(2,6);
						set_block(3,7);
					break;

					case 3 :
						set_block(0,0);
						set_block(1,3);
						set_block(2,2);
						set_block(3,7);
					break;

					case 4 :
						set_block(0,0);
						set_block(1,4);
						set_block(2,2);
						set_block(3,3);
					break;

					case 5 :
						set_block(0,0);
						set_block(1,5);
						set_block(2,2);
						set_block(3,3);
					break;

					case 6 :
						set_block(0,0);
						set_block(1,6);
						set_block(2,2);
						set_block(3,3);
					break;

					case 7 :
						set_block(0,0);
						set_block(1,7);
						set_block(2,2);
						set_block(3,3);
					break;
				}
			break;
		}
	}

	if(!(portnum & 8192))
	{
		switch(data)
		{
			case 0 :
				rombase = 147456;
			break;

			case 7:
				rombase = 163840;
			break;
		}
		if(!toprom)
		{
			readbanks[6] = &mempool[rombase];
			readbanks[7] = &mempool[rombase + 8192];
		}
	}
                
	if(!(portnum & 4096)) //printer stuff
	{
	}
        
	switch(portnum)
	{
		default :
			switch(portnum >> 8)
			{
				default :
//					fprintf(stderr, "Unknown out : %04x\n", portnum);
				break;


				case 0xbc : //crtc : select register
					crtcentry = data;//portnum&255;
					if(crtcentry > 17)
						crtcentry &= 15;
				break;

				case 0xbd : //crtc : write to register
					rlist->tstate = z80_tstates;
					rlist->end = 0;
					rlist->col = crtcentry;
					fakecrtc[crtcentry] = rlist->newval = data & crtcsize[crtcentry];
					rlist = rlist->next;
					rlist->end = 1;
//					fprintf(stderr, "TO : [%d] = %d {%d} at %d\n", crtcentry, data & crtcsize[crtcentry], crtcregs[crtcentry], tstates);
//					crtcregs[crtcentry] = data&crtcsize[crtcentry];
				break;

				case 0xf4 : //straight to AY
					if(!(iostat &16))
	                                        f4byte = data;
				break;

				case 0xf6 : //set keyboard line & ay mode select
					ay_io(14, keylines[data & 15]);

					switch(data & 192)
					{
						case 0 :
							ay_setmode(AY_INACTIVE);
						break;
                                                
						case 64 :
							ay_setmode(REGISTER_READ);
							f4byte = ay_receivebyte();
						break;
                                                
						case 128 :
							ay_setmode(REGISTER_WRITE);
							ay_sendbyte(f4byte);
						break;

						case 192 :
							ay_setmode(REGISTER_SELECT);
							ay_sendbyte(f4byte);
						break;
					}
				break;

				case 0xf7 : //intel chip
					if(!(data & 128))
					{
						if(data & 1)
						{
							portc |= 1 << ((data >> 1)&7);
						}
						else
						{
							portc &= ~(1 << ((data >> 1)&7));
						}
					}
					else
					{
						iostat = data;
					}
				break;

			}
		break;

		case 0xfa7e : //disk drive motor
			fdc8272_set_motor(data);
		break;

		case 0xfb7f : //disk drive data register
			fdc8272_send(data);
		break;
	}
}

void cpc_port_in(unsigned short portnum, unsigned char *data)
{
	switch(portnum)
	{
		default :
			switch(portnum >> 8)
			{
				default : //crtc : register data
					*data = 255;
//					fprintf(stderr, "Unknown in : %04x\n", portnum);
				break;

				case 0xbf : //crtc : read from register
					*data = fakecrtc[crtcentry];
				break;

				case 0xf5 : //8255 : cassette, printer, expansion, screen, name
					if(iostat&2)
						*data = 94 | vsyn;
				break;

				case 0xf4 : //keyboard and that
					if(iostat&16)
						*data = f4byte;
				break;

				case 0xf6 : //keyboard and that
					*data = portc;
				break;
			}
		break;

		case 0xfb7e : //disk drive status register
                	*data = fdc8272_get_status();
		break;

		case 0xfb7f : //disk drive data register
			*data = fdc8272_read();
		break;
	}
}