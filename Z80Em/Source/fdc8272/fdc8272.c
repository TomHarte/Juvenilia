#include "fdc8272.h"
#include "seek.h"
#include "../z80/z80.h"
#include <stdio.h>

//#define LOG
struct fdc8272_dskfile *disk1, *disk2;
unsigned char st0, st1, st2, msr, fdc8272_motor;
struct fdc8272_dskfile *cdisk;
unsigned short srt, tfreq, tsteps;
void (* fdc8272_sendfunc)(unsigned char byte);
unsigned char (* fdc8272_readfunc)(void);
unsigned char firstbyte;
int bytenum, tbyte;

void find_track(struct fdc8272_dskfile *disk)
{
	reset_fdc8272_execphase();
        
	fprintf(stderr, "%d becomes ", msr);
	reset_fdc8272_0busyflag();
	reset_fdc8272_1busyflag();
	reset_fdc8272_2busyflag();
	reset_fdc8272_3busyflag();
	fprintf(stderr, "%d\n", msr);

	disk->flags &= 63;
	st0 = 0;
                
	if(disk->file)
	{
		fprintf(stderr, "Jumped to track %d\n", disk->ctrack >> 8);
		fseek(disk->file, disk->tracklocs[disk->ctrack >> 8][disk->cside], SEEK_SET);
		fread(disk->trackinfo, 1, 256, disk->file);
		st0 |= 32;
	}
	else
	{
		st0 |= 72;
	}
}

int find_sector(struct fdc8272_dskfile *disk)
{
	int count;

	fprintf(stderr, "SEARCHING : %d ", disk->r);

	for(count = 0; count <= 18; count++)
	{
		if(disk->trackinfo[0x18 + (count << 3)+2] == disk->r)
		{
			fprintf(stderr, "Found : %d at %d\n", disk->trackinfo[0x18 + (count << 3)+2], count);
                
			fseek(disk->file, disk->tracklocs[disk->ctrack >> 8][disk->cside]+(count*(1 << (7+disk->trackinfo[0x18 + (count << 3)+3]))) + 0x100, SEEK_SET);
			disk->c = disk->trackinfo[0x18 + (count << 3)];
			disk->h = disk->trackinfo[0x18 + (count << 3)+1];
			disk->n = disk->trackinfo[0x18 + (count << 3)+3];
			disk->csect = count << 8;
			count = 19;
			st1 = disk->trackinfo[0x18 + (count << 3)+4];
			st2 = disk->trackinfo[0x18 + (count << 3)+5];
		}
	}
        
	if(count != 20)
	{
		st0 |= 64;
		st1 |= 4;
		fprintf(stderr, "Not Found.\n");

		fseek(disk->file, disk->tracklocs[disk->ctrack >> 8][disk->cside] + 0x100, SEEK_SET);
		disk->csect = 0;
		disk->cont = 0;
		disk->c = disk->trackinfo[0x18];
		disk->h = disk->trackinfo[0x18+1];
		disk->r = disk->trackinfo[0x18+2];
		disk->n = disk->trackinfo[0x18+3];

		return 1;
	}

	return 0;
        
}


void fdc8272_keepalive(void)
{
	tsteps++;
	if(tsteps >= tfreq) //to get the step rate correct
	{
		tsteps = 0;

		if(cdisk->flags & TRACKSEARCH && cdisk->file)
		{
			if((cdisk->ctrack & 65280) == cdisk->tracktarg)
			{
				cdisk->ctrack &= 65280;
				fprintf(stderr, "Track %d found\n", cdisk->ctrack >> 8);
				cdisk->flags &= ~TRACKSEARCH;
				find_track(cdisk);
			}
			else
			{
				cdisk->ctrack += cdisk->trackadd;
                        
				if((cdisk->ctrack >> 8) >= cdisk->numtracks) //unsigned data type, so this handles '< 0' also
					cdisk->ctrack -= (cdisk->numtracks << 8);
			}
		}
	}
}

void fdc8272_end(void)
{
	fdc8272_closedsk(disk1);
	fdc8272_closedsk(disk2);
	free(disk1);
	free(disk2);
}

void fdc8272_setup(void)
{
	reset_fdc8272_direction();
	reset_fdc8272_execphase();
	reset_fdc8272_busyflag();
	reset_fdc8272_1busyflag();
	reset_fdc8272_0busyflag();
	set_fdc8272_dataready();
        
	fdc8272_sendfunc = select_function;
	fdc8272_readfunc = fdc8272_readclueless;
        
	disk1 = malloc(sizeof(struct fdc8272_dskfile));
	disk2 = malloc(sizeof(struct fdc8272_dskfile));

	disk1->file = NULL;
	disk2->file = NULL;

	cdisk = disk1;
}

unsigned char fdc8272_get_status(void)
{
	return msr;
}

unsigned char fdc8272_set_motor(char byte)
{
	fdc8272_motor = byte;
}

//SPECIFY

void fdc8272_specify(unsigned char byte)
{
	switch(bytenum)
	{
		case 0 :
			reset_fdc8272_direction();
			set_fdc8272_busyflag();
		break;
                
		case 1 :
			srt = byte >> 4;
			tfreq = 125 * srt; //srt = in miliseconds = in 1/1000ths of a second = srt*125000/1000 ticks = 125*srt
		break;
	}

	bytenum++;
	if(bytenum == 3)
	{
		reset_fdc8272_busyflag();
		fdc8272_sendfunc = select_function;
		fprintf(stderr, "MSR : %d\n", msr);
	}
}

//SENSE INTERRUPT STATUS

unsigned char fdc8272_status0_read(void)
{
	unsigned char ret;

	switch(bytenum)
	{
		case 0 :
			ret = st0 | (cdisk->flags&192);
			st0 = 0x80;
			cdisk->flags &= 63;
		break;

		case 1 :
			ret = cdisk->ctrack >> 8;
		break;
	}
	bytenum++;
	if(bytenum == 2)
	{
		fdc8272_readfunc = fdc8272_readclueless;
		reset_fdc8272_direction();
//		reset_fdc8272_execphase();
		reset_fdc8272_busyflag();
	}
	
	return ret;
}

void fdc8272_status0(unsigned char byte)
{
	fdc8272_sendfunc = select_function;
	fdc8272_readfunc = fdc8272_status0_read;
	set_fdc8272_direction();
	set_fdc8272_busyflag();

	#ifdef LOG
	fprintf(stderr, "STATUS 0 (%d - %d [MSR : %d])\n", st0 | (cdisk->flags&192), cdisk->ctrack >> 8, msr);
	#endif
//	set_fdc8272_execphase();
}

//SENSE DRIVE STATUS

unsigned char fdc8272_status3_read(void)
{
	unsigned char ret;

	ret = 32 | st0;

	if(!(cdisk->ctrack >> 8))
		ret |= 16;

	fdc8272_readfunc = fdc8272_readclueless;
	reset_fdc8272_busyflag();
	reset_fdc8272_execphase();
	reset_fdc8272_direction();
	
	return ret;
}

void fdc8272_status3(unsigned char byte)
{

	if(byte&3)
	{
		cdisk = disk2;
                st0 |= 2;
	}
	else
	{
		cdisk = disk1;
                st0 |= 1;
        }

	fdc8272_sendfunc = select_function;
	fdc8272_readfunc = fdc8272_status3_read;
	set_fdc8272_direction();
	set_fdc8272_execphase();
}

//READ SECTOR ID

unsigned char fdc8272_sectorid_read(void)
{
	unsigned char ret;

	switch(bytenum)
	{
		case 0 :
			ret = st0;
		break;

		case 1 :
			ret = st1;
		break;

		case 2 :
			ret = st2;
		break;

		case 3 :
		case 4 :
		case 5 :
		case 6 :
			ret = cdisk->trackinfo[0x18 + ((cdisk->csect >> 5) & ~7) + (bytenum-3)];
		break;
	}
	bytenum++;
	if(bytenum == 7)
	{
		fdc8272_readfunc = fdc8272_readclueless;
		reset_fdc8272_busyflag();
		reset_fdc8272_execphase();
		reset_fdc8272_direction();
	}
	
	return ret;
}

void fdc8272_sectorid(unsigned char byte)
{
	#ifdef LOG
	fprintf(stderr, "SECTOR ID requested\n");
	#endif

	if(byte&3)
	{
		cdisk = disk2;
                st0 |= 2;
	}
	else
	{
		cdisk = disk1;
                st0 |= 1;
        }

	fdc8272_sendfunc = select_function;
	fdc8272_readfunc = fdc8272_sectorid_read;
	set_fdc8272_direction();
//	reset_fdc8272_busyflag();
	set_fdc8272_execphase();
}

//READ DATA

unsigned char fdc8272_readdata_read(void)
{
	unsigned char ret;

	if(tbyte < cdisk->readlen)
	{
		fread(&ret, 1, 1, cdisk->file);
		tbyte++;
	}
	else
	{
		switch(bytenum)
		{
			case 0 :
				reset_fdc8272_0busyflag();
				reset_fdc8272_1busyflag();
				reset_fdc8272_execphase();
				ret = st0;
			break;

			case 1 :
				ret = st1;
			break;

			case 2 :
				ret = st2;
			break;

			case 3 :
				ret = cdisk->c;
                        break;
                        
			case 4 :
				ret = cdisk->h;
                        break;
                        
			case 5 :
				ret = cdisk->r;
                        break;
                        
			case 6 :
				ret = cdisk->n;
//				ret = cdisk->trackinfo[0x18 + ((cdisk->csect >> 5) & ~7) + (bytenum-3)];
			break;
		}
		bytenum++;
		if(bytenum == 7)
		{
			if(cdisk->cont && cdisk->r != cdisk->eot)
			{
				cdisk->r++;
				find_sector(disk1);
				tbyte = bytenum = 0;
				fprintf(stderr, "R : %d EOT : %d\n", cdisk->r, cdisk->eot);
				ret = fdc8272_readdata_read();
			}
	                else
			{
				fdc8272_readfunc = fdc8272_readclueless;
				reset_fdc8272_direction();
				reset_fdc8272_busyflag();
			}
		}
        }
	
	return ret;
}

void fdc8272_readdata(unsigned char byte)
{
	switch(bytenum)
	{
		case 0 :
			set_fdc8272_busyflag();
		break;
        
		case 1 :
			#ifdef LOG
			fprintf(stderr, "READ DATA requested ");
			#endif
			if(byte&3)
			{
				cdisk = disk2;
				st0 |= 2;
				set_fdc8272_1busyflag();
			}
			else
			{
				cdisk = disk1;
                		st0 |= 1;
				set_fdc8272_0busyflag();
			}
		break;

		case 2 :
                        cdisk->c = byte;
		break;

		case 3 :
                        cdisk->h = byte;
		break;

		case 4 :
                        cdisk->r = byte;
		break;

		case 5 :
                        cdisk->n = byte;
		break;

		case 6 :
                        cdisk->eot = byte;
		break;

		case 7 :
                        cdisk->gpl = byte;
		break;

		case 8 :
                        cdisk->dtl = byte;
		break;

        }
        bytenum++;
        if(bytenum == 9)
        {
		fdc8272_sendfunc = select_function;
		fdc8272_readfunc = fdc8272_readdata_read;
                
		set_fdc8272_direction();
		set_fdc8272_execphase();
                
		tbyte = bytenum = 0;
                
		if(cdisk->n)
			cdisk->readlen = (1 << (cdisk->n+7));
		else
			cdisk->readlen = cdisk->dtl;

		if(cdisk->eot == cdisk->r)
			cdisk->cont = 0;
		else
			cdisk->cont = 1;
                        
		#ifdef LOG
		fprintf(stderr, "C:%d H:%d R:%d N:%d EOT:%d GPL:%d DTL:%d readlen:%d\n", cdisk->c, cdisk->h, cdisk->r, cdisk->n, cdisk->eot, cdisk->gpl, cdisk->dtl, cdisk->readlen);
		#endif
                
		if(find_sector(cdisk))
		{
//			reset_fdc8272_direction();
			reset_fdc8272_execphase();
//			reset_fdc8272_busyflag();
			cdisk->readlen = 0;
			st0 |= 64;
			fprintf(stderr, "[C:%d H:%d R:%d N:%d]\n", cdisk->c, cdisk->h, cdisk->r, cdisk->n);
		}
        }
}


//MAIN PARTS
void select_function(unsigned char byte)
{
	firstbyte = byte;
	bytenum = 0;
        
	switch(byte & 31)
	{
		default:
			st0 = 128;
			#ifdef LOG
			fprintf(stderr, "INVALID requested\n");
			#endif
		break;

		case 2:
			st0 = st1 = st2 = 0;
			#ifdef LOG
			fprintf(stderr, "READ TRACK requested\n");
			#endif
		break;

		case 3:
			fdc8272_sendfunc = fdc8272_specify;
		break;

		case 4:
			fdc8272_sendfunc = fdc8272_status3;
		break;

		case 5:
			st0 = st1 = st2 = 0;
			#ifdef LOG
			fprintf(stderr, "WRITE DATA requested\n");
			#endif
		break;

		case 6:
			st0 = st1 = st2 = 0;
			fdc8272_sendfunc = fdc8272_readdata;
		break;

		case 7:
			st0 = st1 = st2 = 0;
			fdc8272_sendfunc = fdc8272_recalibrate;
		break;

		case 8:
			fdc8272_sendfunc = fdc8272_status0;
		break;

		case 9:
			st0 = st1 = st2 = 0;
			#ifdef LOG
			fprintf(stderr, "WRITE DELETED DATA requested\n");
			#endif
		break;

		case 10:
			st0 = st1 = st2 = 0;
			fdc8272_sendfunc = fdc8272_sectorid;
		break;

		case 12:
			st0 = st1 = st2 = 0;
			#ifdef LOG
			fprintf(stderr, "READ DELETED DATA requested\n");
			#endif
		break;

		case 13:
			st0 = st1 = st2 = 0;
			#ifdef LOG
			fprintf(stderr, "FORMAT TRACK requested\n");
			#endif
		break;

		case 15:
			st0 = st1 = st2 = 0;                        
			fdc8272_sendfunc = fdc8272_seek;
		break;

		case 17:
			st0 = st1 = st2 = 0;
			#ifdef LOG
			fprintf(stderr, "SCAN EQUAL requested\n");
			#endif
		break;

		case 25:
			st0 = st1 = st2 = 0;
			#ifdef LOG
			fprintf(stderr, "SCAN LOW OR EQUAL requested\n");
			#endif
		break;

		case 29:
			st0 = st1 = st2 = 0;
			#ifdef LOG
			fprintf(stderr, "SCAN HIGH OR EQUAL requested\n");
			#endif
		break;
	}

	fdc8272_sendfunc(byte);
}

void fdc8272_send(unsigned char data)
{
	//fprintf(stderr, "fdc8272 sent : %d\n", data);
	fdc8272_sendfunc(data);
}

unsigned char fdc8272_readclueless(void)
{
	st0 |= 128;
	reset_fdc8272_direction();
	return 0;
}

unsigned char fdc8272_read(void)
{
	//fprintf(stderr, "fdc8272 data read (%04x)\n",pc);
	return fdc8272_readfunc();
}
