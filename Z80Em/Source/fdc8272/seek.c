#include "fdc8272.h"

//RECALIBRATE

void fdc8272_recalibrate(unsigned char byte)
{
	switch(bytenum)
        {
        	case 0 :
			#ifdef LOG
			fprintf(stderr, "RECALIBRATED drive ");
			#endif
			set_fdc8272_0busyflag();
			set_fdc8272_1busyflag();
			set_fdc8272_2busyflag();
			set_fdc8272_3busyflag();
		break;

		case 1:

			switch(byte & 3)
			{
				case 0 :
					#ifdef LOG
					fprintf(stderr, "1\n");
					#endif
					cdisk = disk1;
					st0 |= 1;
				break;

				case 1 :
					#ifdef LOG
					fprintf(stderr, "2\n");
					#endif
					cdisk = disk2;
					st0 |= 2;
				break;
			}
		break;
	}
	bytenum++;
        
	if(bytenum == 2)
	{
		set_fdc8272_execphase();
		fdc8272_sendfunc = select_function;
		cdisk->flags |= TRACKSEARCH;
		cdisk->flags |= 64;
		cdisk->tracktarg = 0;
		cdisk->trackadd = -256;
	}
}

//SEEK track

void fdc8272_seek(unsigned char byte)
{
	switch(bytenum)
        {
		case 0 :
			set_fdc8272_0busyflag();
			set_fdc8272_1busyflag();
			set_fdc8272_2busyflag();
			set_fdc8272_3busyflag();
                break;

		case 1 :
			#ifdef LOG
			fprintf(stderr, "SEEK drive ");
			#endif

                        if(byte&3)
			{
				#ifdef LOG
				fprintf(stderr, "2\n");
				#endif
				cdisk = disk2;
				st0 |= 2;
			}
			else
	               	{
				#ifdef LOG
				fprintf(stderr, "1\n");
				#endif
				cdisk = disk1;
				st0 |= 1;
			}
			tsteps = 0;
                break;

		case 2 :
			cdisk->flags |= TRACKSEARCH;
			cdisk->flags |= 64;
			cdisk->tracktarg = byte << 8;
                        
			if(cdisk->tracktarg < cdisk->ctrack)
				cdisk->trackadd = -256;
			else
				cdisk->trackadd = 256;
			#ifdef LOG
			fprintf(stderr, "Seeking track %d [from %d - += %d]\n", cdisk->tracktarg >> 8, cdisk->ctrack >> 8, cdisk->trackadd >> 8);
			#endif
                break;

	}
	bytenum++;

	if(bytenum == 3)
	{
		fdc8272_sendfunc = select_function;
		set_fdc8272_execphase();
        }
}
