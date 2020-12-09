#include <stdio.h>
#include <allegro.h>
#include "sn76489a.h"

extern unsigned char mode, channels[4], noises[2];
extern SAMPLE *square;
SAMPLE *noise1, *noise2;

unsigned short newtone;
unsigned char regnum, noisemode;


void sn76489a_end(void)
{
	deallocate_voice(channels[0]);
	deallocate_voice(channels[1]);
	deallocate_voice(channels[2]);
        
	deallocate_voice(noises[0]);
	deallocate_voice(noises[1]);
	
	destroy_sample(square);
	destroy_sample(noise1);
	destroy_sample(noise2);
}

void sn76489a_setup(void)
{
	int c, b;

	square = create_sample(8, FALSE, 11025, 2048);
	for(c = 0; c < 2048; c++)
		((unsigned char *)square->data)[c] = (c&16)? 63 : 0;
        
  
	noise1 = create_sample(8, FALSE, 11025, 2048);
	for(c = 0; c < 2048; c++)
	{
		if(!(c&15))
			b = (rand()%2)*255;
                        
		((unsigned char *)noise1->data)[c] = b;
	}

	noise2 = create_sample(8, FALSE, 11025, 2048);
	for(c = 0; c < 2048; c++)
	{
		if(!(c&15))
			b = (rand()%2)*255;
                        
		((unsigned char *)noise2->data)[c] = b;
	}

	channels[0] = allocate_voice(square);
	voice_set_playmode(channels[0], PLAYMODE_LOOP);
	channels[1] = allocate_voice(square);
	voice_set_playmode(channels[1], PLAYMODE_LOOP);
	channels[2] = allocate_voice(square);
	voice_set_playmode(channels[2], PLAYMODE_LOOP);
	channels[3] = allocate_voice(noise1);
	voice_set_playmode(channels[2], PLAYMODE_LOOP);
        
	voice_set_volume(channels[0], 0);
	voice_start(channels[0]);
	voice_set_volume(channels[1], 0);
	voice_start(channels[1]);
	voice_set_volume(channels[2], 0);
	voice_start(channels[2]);

	noises[0] = allocate_voice(noise1);
	voice_set_playmode(noises[0], PLAYMODE_LOOP);
	noises[1] = allocate_voice(noise2);
	voice_set_playmode(noises[1], PLAYMODE_LOOP);

	voice_set_volume(noises[0], 0);
	voice_start(noises[0]);
	voice_set_volume(noises[1], 0);
	voice_start(noises[1]);

	noisemode = mode = 0;
}

void alter_tone(unsigned char channo, unsigned short newtone)
{
	int freq;
//new frequency = (16*)3579545 / 32n = 3579545 / 512n
	if(!newtone)
		newtone = 1;

	freq = 3579545 / (newtone << 1);

	voice_set_frequency(channels[channo], freq);
}

void alter_volume(unsigned char channo, unsigned char volume)
{
	voice_set_volume(channels[channo], (15-volume) << 4);
}

void alter_noise_tone(unsigned char newtone)
{
}

void alter_noise_volume(unsigned char volume)
{
}

void sn76489a_sendbyte(unsigned char byte)
{
	if(mode) //second of tone setting bytes
	{
		mode = 0;

		newtone |= (byte&63) << 4;
		alter_tone(regnum, newtone);
	}
	else
	{
		switch(byte&112)
		{
			case 0 :
			case 32 :
			case 64 :
				regnum = (byte >> 5) & 3;
				newtone = byte & 15;
				mode = 1;
			break;

			case 16 :
			case 48 :
			case 80 :
				alter_volume((byte >> 5) & 3, byte & 15);
			break;

			case 96 :
				alter_noise_tone(byte & 7);
			break;

			case 112 :
				alter_noise_volume(byte & 15);
			break;
		}
	}
}
