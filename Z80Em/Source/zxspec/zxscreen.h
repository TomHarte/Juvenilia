extern void (* scanline_pixels)(void);
void scanline_border(void);
void scanline_nothing(void);
void scanline_restart(void);
void init_scanlines(void);
void zxscr_table_setup(void);

union zx_colentry
{
	unsigned int full;

	struct
	{
		unsigned char b1;
		unsigned char b2;
		unsigned char b3;
		unsigned char b4;

	} bytes;

	struct
	{
		unsigned short b1;
		unsigned short b2;

	} words;

};

extern unsigned char zx_flashtable[256];
extern union zx_colentry zx_coltable[32768];

