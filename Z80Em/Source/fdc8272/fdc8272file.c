#include "fdc8272.h"
#include "../z80/z80.h"

void fdc8272_closedsk(struct fdc8272_dskfile *disk)
{
	if(disk->file)
	{
		fclose(disk->file);
		disk->file = NULL;
	}
}

int fdc8272_opendsk(struct fdc8272_dskfile *disk, char *name)
{
	char checktext[9];
	Z80_REGISTERPAIR tracksize;
	int tcount, cpos;
	unsigned char success;
        	
	if(disk->file)
		fdc8272_closedsk(disk);

	success = 0;

	if(disk->file = fopen(name, "rb"))
	{
		fread(checktext, 8, 1, disk->file);
		checktext[8] = '\0';

		if(!strcmp("MV - CPC", checktext))
		{
			fseek(disk->file, 0x30, SEEK_SET);
			fread(&disk->numtracks, 1, 1, disk->file);
			fread(&disk->numsides, 1, 1, disk->file);

			fread(&tracksize.bytes.low, 1, 1, disk->file);
			fread(&tracksize.bytes.high, 1, 1, disk->file);
			disk->tracksize = tracksize.full;

			for(tcount = 0; tcount < disk->numtracks; tcount++)
			{
				disk->tracklocs[tcount][0] = (tcount*disk->tracksize*disk->numsides)+0x100;
				disk->tracklocs[tcount][1] = (tcount*disk->tracksize*disk->numsides)+0x100+disk->tracksize;
			}
                
			success = 1;
		}
                
		if(!strcmp("EXTENDED", checktext))
		{
			fseek(disk->file, 0x30, SEEK_SET);
			fread(&disk->numtracks, 1, 1, disk->file);
			fread(&disk->numsides, 1, 1, disk->file);

			fseek(disk->file, 0x34, SEEK_SET);
			cpos = 0x100;
			tracksize.bytes.low = 0;

			for(tcount = 0; tcount < disk->numtracks; tcount++)
			{
				fread(&tracksize.bytes.high, 1, 1, disk->file);
//				fprintf(stderr, "S:%d CPOS:%d\n", tracksize.full, cpos);
				if(tracksize.full)
				{
					disk->tracklocs[tcount][0] = cpos;
					cpos += tracksize.full;
				}
				else
					disk->tracklocs[tcount][0] = 0;

				if(disk->numsides == 2)
				{
					fread(&tracksize.bytes.high, 1, 1, disk->file);
					if(tracksize.full)
					{
						disk->tracklocs[tcount][1] = cpos;
						cpos += tracksize.full;
					}
					else
						disk->tracklocs[tcount][1] = 0;
				}
			}
                
			success = 1;
		}
                
		if(!success)
		{
			fclose(disk->file);
			disk->file = NULL;
		}
		else
		{
			disk->trackadd =
			disk->sectadd = 0;
			disk->cside =
			disk->csect =
			disk->flags =
			disk->ctrack = 0;
			find_track(disk);
		}
	}

	return 0;
}
