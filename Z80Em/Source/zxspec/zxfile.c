#include "zxspec.h"
#include <stdio.h>
#include "../z80/z80.h"
#include "../ay38912/ay38912.h"

unsigned char model;

void set_model(int mdl)
{
	model = mdl;
	z80_exitfunc();

	switch(mdl)
	{
		case MODEL_48kb :
			setup_48kb();
		break;
                
		case MODEL_128kb :
			setup_128kb();
		break;
	}
}

void zxspec_loadz80(char *fname)
{
	FILE *infile;
	Z80_REGISTERPAIR sixb;
	unsigned char temp, compressed, buf[4], count, block, *specptr;
	unsigned short mempos;
	short headerlen;

	if(infile = fopen(fname, "rb"))
	{
//		gfname = malloc(strlen(fname)+1);
//		strcpy(gfname, fname);

		fread(&af.bytes.high, 1, 1, infile);
		fread(&af.bytes.low, 1, 1, infile);

		fread(regC, 1, 1, infile);
		fread(regB, 1, 1, infile);

		fread(regL, 1, 1, infile);
		fread(regH, 1, 1, infile);

		fread(&sixb.bytes.low, 1, 1, infile);
		fread(&sixb.bytes.high, 1, 1, infile);
		pc = sixb.full;

		fread(regP, 1, 1, infile);
		fread(regS, 1, 1, infile);

		fread(&i, 1, 1, infile);
		fread(&r, 1, 1, infile);
		r &= 127;

		fread(&temp, 1, 1, infile);
		if(temp == 255)
			temp = 1;
		r |= (temp << 7);
		zx_bcol = (temp >> 1) & 7;
		compressed = zx_bcol&32;

		fread(regE, 1, 1, infile);
		fread(regD, 1, 1, infile);

		fread(&bcd.bytes.low, 1, 1, infile);
		fread(&bcd.bytes.high, 1, 1, infile);

		fread(&ded.bytes.low, 1, 1, infile);
		fread(&ded.bytes.high, 1, 1, infile);

		fread(&hld.bytes.low, 1, 1, infile);
		fread(&hld.bytes.high, 1, 1, infile);

		fread(&afd.bytes.high, 1, 1, infile);
		fread(&afd.bytes.low, 1, 1, infile);

		fread(&iy.bytes.low, 1, 1, infile);
		fread(&iy.bytes.high, 1, 1, infile);

		fread(&ix.bytes.low, 1, 1, infile);
		fread(&ix.bytes.high, 1, 1, infile);

		fread(&temp, 1, 1, infile);
		imode = temp ? 128 : 0;

		fread(&temp, 1, 1, infile);
		imode |= temp ? 64 : 0;

		fread(&temp, 1, 1, infile);
		imode |= temp&3;

		if(pc)
		{//old style
			set_model(MODEL_48kb);
			mempos = 16384;

			fread(&buf[0], 1, 1, infile);
			fread(&buf[1], 1, 1, infile);
			fread(&buf[2], 1, 1, infile);
			fread(&buf[3], 1, 1, infile);

			while(!(buf[0] == 0x0 && buf[1] == 0xed && buf[2] == 0xed && buf[3] == 0x0) && !feof(infile))
			{
				if(buf[0] == 0xed && buf[1] == 0xed)
				{
					for(count = 0; count < buf[2]; count++)
					{
						writememory(mempos) = buf[3];
						mempos++;
					}

					fread(&buf[0], 1, 1, infile);
					fread(&buf[1], 1, 1, infile);
					fread(&buf[2], 1, 1, infile);
					fread(&buf[3], 1, 1, infile);
				}
				else
				{
					writememory(mempos) = buf[0];
					buf[0] = buf[1];
					buf[1] = buf[2];
					buf[2] = buf[3];
					fread(&buf[3], 1, 1, infile);
					mempos++;
				}
			}
		}
		else
		{//new style
			fread(&headerlen, 2, 1, infile);

			fread(&sixb.bytes.low, 1, 1, infile);
			fread(&sixb.bytes.high, 1, 1, infile);
			pc = sixb.full;

			fread(&temp, 1, 1, infile);

			switch(headerlen)
			{
				case 23 :
					if(temp <= 1) //48kb mode
					{
						set_model(MODEL_48kb);

						fseek(infile, 20, SEEK_CUR);
					}
					else
					{ //128kb mode
						set_model(MODEL_128kb);

						fread(&temp, 1, 1, infile);
						z80_port_out(0x7ffd, temp);

						fseek(infile, 2, SEEK_CUR);
						fread(&block, 1, 1, infile);

						for(count = 0; count < 16; count++)
						{
							fread(&ayregs[count], 1, 1, infile);
							ay_setmode(REGISTER_SELECT);
							ay_sendbyte(count);
							ay_setmode(REGISTER_WRITE);
							ay_sendbyte(ayregs[count]);
						}
                                                
						z80_port_out(0xfffd, block);
					}
				break;

				default :
				case 54 :

					if(temp <= 3) //48kb mode
					{
						set_model(MODEL_48kb);

						fseek(infile, 51, SEEK_CUR);
					}
					else
					{ //128kb mode
						set_model(MODEL_128kb);

						fread(&temp, 1, 1, infile);
						z80_port_out(0x7ffd, temp);

						fseek(infile, 2, SEEK_CUR);
						fread(&block, 1, 1, infile);

						for(count = 0; count < 16; count++)
						{
							fread(&ayregs[count], 1, 1, infile);
							ay_setmode(REGISTER_SELECT);
							ay_sendbyte(count);
							ay_setmode(REGISTER_WRITE);
							ay_sendbyte(ayregs[count]);
						}

						z80_port_out(0xfffd, block);
						fseek(infile, 31, SEEK_CUR);
					}
				break;
			}

			fread(&headerlen, 2, 1, infile);

			while(!feof(infile))
			{
				fread(&block, 1, 1, infile);

				switch(model)
				{
					case MODEL_48kb :
						switch(block)
						{
							default :
								specptr = &writememory(0);
							break;

							case 4 :
								specptr = &writememory(0x8000);
							break;

							case 5 :
								specptr = &writememory(0xc000);
							break;

							case 8 :
								specptr = &writememory(0x4000);
							break;
						}
					break;

					case MODEL_128kb :
						switch(block)
						{
							case 0 :
								specptr = &mempool[0x20000];
							break;

							case 2 :
								specptr = &mempool[0x24000];
							break;

							case 3 :
							case 4 :
							case 5 :
							case 6 :
							case 7 :
							case 8 :
							case 9 :
							case 10 :
								specptr = &mempool[(block-3) << 14];
							break;
						}
					break;
				}

				fread(&buf[0], 1, 1, infile);
				fread(&buf[1], 1, 1, infile);
				fread(&buf[2], 1, 1, infile);
				fread(&buf[3], 1, 1, infile);
				headerlen -= 4;

				while(headerlen > 0)
				{
					if(buf[0] == 0xed && buf[1] == 0xed)
					{
						for(count = 0; count < buf[2]; count++)
						{
							*specptr = buf[3];
							specptr++;
						}

						fread(&buf[0], 1, 1, infile);
						fread(&buf[1], 1, 1, infile);
						fread(&buf[2], 1, 1, infile);
						fread(&buf[3], 1, 1, infile);
						headerlen -= 4;
					}
					else
					{
						*specptr = buf[0];
						buf[0] = buf[1];
						buf[1] = buf[2];
						buf[2] = buf[3];
						fread(&buf[3], 1, 1, infile);
						specptr++;
						headerlen--;
					}
				}

				if(buf[0] == 0xed && buf[1] == 0xed)
				{
					for(count = 0; count < buf[2]; count++)
					{
						*specptr = buf[3];
						specptr++;
					}
				}
				else
				{
					*specptr++ = buf[0];
					*specptr++ = buf[1];
					*specptr++ = buf[2];
					*specptr++ = buf[3];
				}

				fseek(infile, headerlen, SEEK_CUR);
				fread(&headerlen, 2, 1, infile);
			}

		}

		fclose(infile);
	}
}

void zxspec_loadsna(char *fname)
{
	FILE *infile;
	unsigned char im;
	int c;

	if(infile = fopen(fname, "rb"))
	{
		set_model(MODEL_48kb);
//		gfname = malloc(strlen(fname)+1);
//		strcpy(gfname, fname);

		fread(&i, 1, 1, infile);

		fread(&hld.full, 2, 1, infile);
		fread(&ded.full, 2, 1, infile);
		fread(&bcd.full, 2, 1, infile);
		fread(&afd.full, 2, 1, infile);

		fread(&hl.full, 2, 1, infile);
		fread(&de.full, 2, 1, infile);
		fread(&bc.full, 2, 1, infile);
		fread(&iy.full, 2, 1, infile);
		fread(&ix.full, 2, 1, infile);

		fread(&im, 1, 1, infile);

		fread(&r, 1, 1, infile);

		fread(&af.full, 2, 1, infile);
		fread(&sp.full, 2, 1, infile);

		fread(&imode, 1, 1, infile);
		imode |= ((im << 5) | (im << 4)) & 192;

		fread(&zx_bcol, 1, 1, infile);

		for(c = 16384; c < 65536; c++)
			fread(&writememory(c), 1, 1, infile);

		z80_ret(NULL, NULL);

		fclose(infile);
	}
}

