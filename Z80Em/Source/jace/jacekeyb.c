#include <allegro.h>
#include "jacekeyb.h"
#include "jace.h"

void jace_updatekeybits(void)
{
	jace_keybits[0] =
	jace_keybits[1] =
	jace_keybits[2] =
	jace_keybits[3] =
	jace_keybits[4] =
	jace_keybits[5] =
	jace_keybits[6] =
	jace_keybits[7] = 255;

	//line 0
	if(key[KEY_LSHIFT] || key[KEY_RSHIFT])
		jace_keybits[0] &= ~1;
                
	if(key[KEY_LCONTROL])
		jace_keybits[0] &= ~2;
                
	if(key[KEY_Z])
		jace_keybits[0] &= ~4;
                
	if(key[KEY_X])
		jace_keybits[0] &= ~8;
                
	if(key[KEY_C])
		jace_keybits[0] &= ~16;
                
	//line 1
	if(key[KEY_A])
		jace_keybits[1] &= ~1;
                
	if(key[KEY_S])
		jace_keybits[1] &= ~2;
                
	if(key[KEY_D])
		jace_keybits[1] &= ~4;
                
	if(key[KEY_F])
		jace_keybits[1] &= ~8;
                
	if(key[KEY_G])
		jace_keybits[1] &= ~16;
                
	//line 2
	if(key[KEY_Q])
		jace_keybits[2] &= ~1;
                
	if(key[KEY_W])
		jace_keybits[2] &= ~2;
                
	if(key[KEY_E])
		jace_keybits[2] &= ~4;
                
	if(key[KEY_R])
		jace_keybits[2] &= ~8;
                
	if(key[KEY_T])
		jace_keybits[2] &= ~16;
                
	//line 3
	if(key[KEY_1])
		jace_keybits[3] &= ~1;
                
	if(key[KEY_2])
		jace_keybits[3] &= ~2;
                
	if(key[KEY_3])
		jace_keybits[3] &= ~4;
                
	if(key[KEY_4])
		jace_keybits[3] &= ~8;
                
	if(key[KEY_5])
		jace_keybits[3] &= ~16;
                
	//line 4
	if(key[KEY_0])
		jace_keybits[4] &= ~1;
                
	if(key[KEY_9])
		jace_keybits[4] &= ~2;
                
	if(key[KEY_8])
		jace_keybits[4] &= ~4;
                
	if(key[KEY_7])
		jace_keybits[4] &= ~8;
                
	if(key[KEY_6])
		jace_keybits[4] &= ~16;
                
	//line 5
	if(key[KEY_P])
		jace_keybits[5] &= ~1;
                
	if(key[KEY_O])
		jace_keybits[5] &= ~2;
                
	if(key[KEY_I])
		jace_keybits[5] &= ~4;
                
	if(key[KEY_U])
		jace_keybits[5] &= ~8;
                
	if(key[KEY_Y])
		jace_keybits[5] &= ~16;
                
	//line 6
	if(key[KEY_ENTER])
		jace_keybits[6] &= ~1;
                
	if(key[KEY_L])
		jace_keybits[6] &= ~2;
                
	if(key[KEY_K])
		jace_keybits[6] &= ~4;
                
	if(key[KEY_J])
		jace_keybits[6] &= ~8;
                
	if(key[KEY_H])
		jace_keybits[6] &= ~16;
                
	//line 7
	if(key[KEY_SPACE])
		jace_keybits[7] &= ~1;
                
	if(key[KEY_M])
		jace_keybits[7] &= ~2;
                
	if(key[KEY_N])
		jace_keybits[7] &= ~4;
                
	if(key[KEY_B])
		jace_keybits[7] &= ~8;
                
	if(key[KEY_V])
		jace_keybits[7] &= ~16;
}
