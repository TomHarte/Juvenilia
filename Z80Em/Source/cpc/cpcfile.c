#include "../all.h"
#include "../ay38912/ay38912.h"
#include <stdio.h>
#include "../z80/z80.h"
#include "cpc.h"

char cpc_loadsna(char *name)
{
	FILE *instr;
	char checktext[9], ver;
        unsigned char temp, palentry, pal;
        Z80_REGISTERPAIR snapsize;
        int count;

	if(instr = fopen(name, "rb"))
	{
		fread(checktext, 8, 1, instr);
		checktext[8] = '\0';

		if(strcmp("MV - SNA", checktext))
			return 0;

		fseek(instr, 0x10, SEEK_SET);

		fread(&ver, 1, 1, instr);
		#ifdef LOG
		fprintf(stderr, "Ver : %d\n", ver);
		#endif

		fread(&af.bytes.low, 1, 1, instr);
		fread(&af.bytes.high, 1, 1, instr);
		fread(&bc.bytes.low, 1, 1, instr);
		fread(&bc.bytes.high, 1, 1, instr);
		fread(&de.bytes.low, 1, 1, instr);
		fread(&de.bytes.high, 1, 1, instr);
		fread(&hl.bytes.low, 1, 1, instr);
		fread(&hl.bytes.high, 1, 1, instr);
		fread(&r, 1, 1, instr);
		fread(&i, 1, 1, instr);

		fread(&imode, 1, 1, instr);
		imode <<= 7;
		fread(&temp, 1, 1, instr);
		imode |= (temp << 6);

		fread(&ix.bytes.low, 1, 1, instr);
		fread(&ix.bytes.high, 1, 1, instr);
		fread(&iy.bytes.low, 1, 1, instr);
		fread(&iy.bytes.high, 1, 1, instr);
		fread(&sp.bytes.low, 1, 1, instr);
		fread(&sp.bytes.high, 1, 1, instr);

		fread(&temp, 1, 1, instr);
		pc = temp;
		fread(&temp, 1, 1, instr);
		pc |= temp << 8;

		fread(&temp, 1, 1, instr);
		imode |= temp;

		fread(&afd.bytes.low, 1, 1, instr);
		fread(&afd.bytes.high, 1, 1, instr);
		fread(&bcd.bytes.low, 1, 1, instr);
		fread(&bcd.bytes.high, 1, 1, instr);
		fread(&ded.bytes.low, 1, 1, instr);
		fread(&ded.bytes.high, 1, 1, instr);
		fread(&hld.bytes.low, 1, 1, instr);
		fread(&hld.bytes.high, 1, 1, instr);

                fseek(instr, 0x2e, SEEK_SET);
		fread(&palentry, 1, 1, instr);
		for(temp = 0; temp < 17; temp++)
		{
			fread(&pal, 1, 1, instr);
			z80_port_out(0x7f00, temp);
			z80_port_out(0x7f00, pal | 64);
		}
		z80_port_out(0x7f00, pal);

		fread(&temp, 1, 1, instr);
                z80_port_out(0x7f80, temp);
                
                fseek(instr, 0x42, SEEK_SET);
		fread(&crtcentry, 1, 1, instr);
                for(temp = 0; temp < 18; temp++)
                {
			fread(&crtcregs[temp], 1, 1, instr);
                }

		/*fread(&temp, 1, 1, instr);
                readbanks[0] = writebanks[0];
                readbanks[1] = writebanks[1];
                readbanks[6] = writebanks[6];
                readbanks[7] = writebanks[7];
                fprintf(stderr, "%d\n", temp);*/

                fseek(instr, 0x5a, SEEK_SET);
		fread(&temp, 1, 1, instr);
		aycurrent = &ayregs[temp];
                for(temp = 0; temp < 16; temp++)
                {
			fread(&ayregs[temp], 1, 1, instr);
			ay_setmode(REGISTER_SELECT);
			ay_sendbyte(temp);
			ay_setmode(REGISTER_WRITE);
			ay_sendbyte(ayregs[temp]);
                }

                fseek(instr, 0x6b, SEEK_SET);
		fread(&snapsize.bytes.low, 1, 1, instr);
		fread(&snapsize.bytes.high, 1, 1, instr);

                fseek(instr, 0x100, SEEK_SET);
                for(count = 0; count < (snapsize.full)*1024; count++)
                {
			fread(&mempool[count], 1, 1, instr);
                }
                
		crtc_refresh();
		fclose(instr);
	}
	else
		return 0;
}
