#include <allegro.h>

#define zx_page(x,y)	readbanks[x] = writebanks[x] = &mempool[(y)<<14];\
			bankzones[x] = zones[y];
				
#define ZX_RAM		0
#define ZX_CONTENDED	1
#define ZX_ROM		2

extern unsigned char		*roms;
extern unsigned int drawsums[192], scannum, vmem_offset;

typedef void (* scanptr)(void);
extern scanptr scrfuncs[312];

int list_sinclair(void);
int go_sinclair(int id);
int setup_128kb(void);
int setup_pl3(void);
int setup_48kb(void);
int setup_16kb(void);

#define MODEL_16kb 0
#define MODEL_48kb 1
#define MODEL_128kb 2
#define MODEL_PL3 3

extern SAMPLE *zxtoneh, *zxtonel;
extern unsigned char zxchan, zx_bcol;
