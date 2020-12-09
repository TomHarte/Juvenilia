typedef void (*funcptr)(void *var1, void *var2);
typedef struct
{
	funcptr func;
	void *var1;
	void *var2;
} Z80_INSTRUCTION;

#include <allegro.h>
typedef union
{
	unsigned short full;

	struct
	{
#ifdef ALLEGRO_LITTLE_ENDIAN
		unsigned char low;
		unsigned char high;
#else
		unsigned char high;
		unsigned char low;
#endif


	} bytes;

} Z80_REGISTERPAIR;
