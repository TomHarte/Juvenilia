#include "ay38912.h"
#include <stdio.h>

unsigned char channels[4], noises[3], ayregs[17], *aycurrent, mode;
SAMPLE *square, *noise;
unsigned char reg13;
int counter, countarg, pcounter, period;

//void (* change_tones[3])(int regbase, int chan);
//void (* change_vols[3])(int regbase, int chan);

void ay_keepalive(void)
{
	unsigned char targvol;

	counter++;
	if(counter >= countarg)
	{
		counter = 0;
		targvol = 0;
		if(!(pcounter++ & 15))
		{
			period++;
                        
			if(reg13 & 2) //alternate
				reg13 ^= 4;
			if(!(reg13 & 1) && (reg13 & 8)) //hold
                                period = 0;
		}
                
		if(!period)
		{
			if(reg13 & 4) //attack
			{
				//set all affected volumes to 'pcounter'
                                targvol = pcounter << 4;
			}
			else
			{
				//set all affected volumes to '15-pcounter'
				targvol = (15 - pcounter) << 4;
			}
		}
		else
		{
			if(!(reg13 & 8)) //continue
			{
				targvol = 0;
			}
			else
			{
				if(reg13 & 4)
					targvol = 240;
				else
					targvol = 0;
			}
		}

		if(ayregs[8]&16)
		{
			voice_set_volume(channels[0], (ayregs[7]&1) ? 0 : targvol);
			voice_set_volume(noises[0], (ayregs[7]&8) ? 0 : targvol);
                }
                
		if(ayregs[9]&16)
		{
			voice_set_volume(channels[1], (ayregs[7]&2) ? 0 : targvol);
			voice_set_volume(noises[1], (ayregs[7]&16) ? 0 : targvol);
                }
                
		if(ayregs[10]&16)
		{
			voice_set_volume(channels[2], (ayregs[7]&4) ? 0 : targvol);
			voice_set_volume(noises[2], (ayregs[7]&32) ? 0 : targvol);
                }
	}
}

void ay_end(void)
{
	deallocate_voice(channels[0]);
	deallocate_voice(channels[1]);
	deallocate_voice(channels[2]);
	
	deallocate_voice(noises[0]);
	deallocate_voice(noises[1]);
	deallocate_voice(noises[2]);

	destroy_sample(square);
	destroy_sample(noise);
}

void ay_setup(void)
{
	int c;
	unsigned char b;
        
/*
	//tone set to zero, noise set to one
	ltone_hnoise = create_sample(8, FALSE, 11025, 2048);
	for(c = 0; c < 2048; c++)
		((unsigned char *)ltone_hnoise->data)[c] = (c&16)? 255 : 0;
        
	//tone set to zero, noise set to zero
	ltone_lnoise = create_sample(8, FALSE, 11025, 2048);
	for(c = 0; c < 2048; c++)
	{
		if(!(c&15))
			b = (rand()%2)? 255 : 0;
                        
		((unsigned char *)ltone_lnoise->data)[c] = ((c&16)? 127 : 0) & b;
	}
        
	//tone set to one, noise set to zero
	ltone_lnoise = create_sample(8, FALSE, 11025, 2048);
	for(c = 0; c < 2048; c++)
	{
		if(!(c&15))
			b = (rand()%2)? 255 : 0;
                        
		((unsigned char *)htone_lnoise->data)[c] = b;
	}

*/

	square = create_sample(8, FALSE, 11025, 2048);
	for(c = 0; c < 2048; c++)
		((unsigned char *)square->data)[c] = (c&16)? 127 : 0;
        
  
	noise = create_sample(8, FALSE, 11025, 32768);
	for(c = 0; c < 32768; c++)
	{
		if(!(c&15))
			b = (rand()%2)? 63 : 0;
                        
		((unsigned char *)noise->data)[c] = b;
	}

	channels[0] = allocate_voice(square);
	voice_set_playmode(channels[0], PLAYMODE_LOOP);
	channels[1] = allocate_voice(square);
	voice_set_playmode(channels[1], PLAYMODE_LOOP);
	channels[2] = allocate_voice(square);
	voice_set_playmode(channels[2], PLAYMODE_LOOP);
        
	voice_set_volume(channels[0], 0);
	voice_start(channels[0]);
	voice_set_volume(channels[1], 0);
	voice_start(channels[1]);
	voice_set_volume(channels[2], 0);
	voice_start(channels[2]);

	noises[0] = allocate_voice(noise);
	voice_set_playmode(noises[0], PLAYMODE_LOOP | PLAYMODE_BIDIR);
	noises[1] = allocate_voice(noise);
	voice_set_playmode(noises[1], PLAYMODE_LOOP | PLAYMODE_BIDIR);
	noises[2] = allocate_voice(noise);
	voice_set_playmode(noises[2], PLAYMODE_LOOP | PLAYMODE_BIDIR);

	voice_set_volume(noises[0], 0);
	voice_start(noises[0]);
	voice_set_volume(noises[1], 0);
	voice_start(noises[1]);
	voice_set_volume(noises[2], 0);
	voice_start(noises[2]);
}

void change_tone(int regbase, int chan)
{
	unsigned short freq;

	freq = ((ayregs[regbase+1]&15) << 8) | (ayregs[regbase]);
	if(!freq)
		freq=1;

	freq = 2000000 / freq; //(sound clock / 16) / period = (1000000 / 16) / period

	voice_set_frequency(channels[chan], freq);
}

void change_volume(int regbase, int chan, int noise, int tone, unsigned char flag)
{
	if(!(ayregs[regbase]&16))
	{
		voice_set_volume(channels[chan], tone ? 0 : ((ayregs[regbase]&15) << 4));
		voice_set_volume(noises[chan], noise ? 0 : ((ayregs[regbase]&15) << 4));
	}
}

void change_noise(void)
{
	unsigned short freq;

	freq = (ayregs[6]&31);
	if(!freq)
		freq=1;

	freq =  1000000 / freq;
	voice_set_frequency(noises[0], freq);
	voice_set_frequency(noises[1], freq);
	voice_set_frequency(noises[2], freq);
}

void set_target(void)
{
	countarg = ((ayregs[12] << 8) | (ayregs[11])) * 1.127853469277;
}

void change_state(int num)
{
	switch(num)
	{
		case 0 :
		case 1 :
			change_tone(0, 0);
		break;
  
		case 2 :
		case 3 :
			change_tone(2, 1);
		break;
  
		case 4 :
		case 5 :
			change_tone(4, 2);
		break;

		case 6 :
			change_noise();
		break;

		case 8 :
			change_volume(8, 0, ayregs[7]&8, ayregs[7]&1, 1);
		break;
  
		case 9 :
			change_volume(9, 1, ayregs[7]&16, ayregs[7]&2, 2);
		break;
  
		case 10 :
			change_volume(10, 2, ayregs[7]&32, ayregs[7]&4, 4);
		break;
                
		case 11 :
		case 12 :
                        set_target();
		break;
                
		case 13 :
			period = pcounter = counter = 0;
			reg13 = ayregs[13];
		break;
	}
}

void ay_sendbyte(unsigned char byte)
{
	switch(mode)
	{
		case REGISTER_SELECT :
			aycurrent = &ayregs[byte];
		break;

		case REGISTER_WRITE :
			*aycurrent = byte;
			change_state(aycurrent - ayregs);
		break;
	}
}

unsigned char ay_receivebyte(void)
{
	if(mode == REGISTER_READ)
		return *aycurrent;
	return 0;
}

void ay_io(unsigned char rnum, unsigned char data)
{
	ayregs[rnum] = data;
}
