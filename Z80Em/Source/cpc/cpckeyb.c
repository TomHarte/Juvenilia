#include <allegro.h>
#include "cpckeyb.h"

void cpc_update_keylines(void)
{
	keylines[0] = keylines[1] = keylines[2] = keylines[3] = keylines[4] =
	keylines[5] = keylines[6] = keylines[7] = keylines[8] = keylines[9] = 255;
        
	if(key[KEY_DEL_PAD])
		keylines[0] &= ~128;

	if(key[KEY_ENTER])
		keylines[0] &= ~64;

	if(key[KEY_3_PAD])
		keylines[0] &= ~32;

	if(key[KEY_6_PAD])
		keylines[0] &= ~16;

	if(key[KEY_9_PAD])
		keylines[0] &= ~8;

	if(key[KEY_DOWN])
		keylines[0] &= ~4;

	if(key[KEY_RIGHT])
		keylines[0] &= ~2;

	if(key[KEY_UP])
		keylines[0] &= ~1;

                
	if(key[KEY_0_PAD])
		keylines[1] &= ~128;

	if(key[KEY_2_PAD])
		keylines[1] &= ~64;

	if(key[KEY_1_PAD])
		keylines[1] &= ~32;

	if(key[KEY_5_PAD])
		keylines[1] &= ~16;

	if(key[KEY_8_PAD])
		keylines[1] &= ~8;

	if(key[KEY_7_PAD])
		keylines[1] &= ~4;

	if(key[KEY_INSERT])
		keylines[1] &= ~2;

	if(key[KEY_LEFT])
		keylines[1] &= ~1;

                
	if(key[KEY_LCONTROL] || key[KEY_RCONTROL])
		keylines[2] &= ~128;

	if(key[KEY_BACKSLASH])
		keylines[2] &= ~64;

	if(key[KEY_LSHIFT] || key[KEY_RSHIFT])
		keylines[2] &= ~32;

	if(key[KEY_4_PAD])
		keylines[2] &= ~16;

	if(key[KEY_CLOSEBRACE])
		keylines[2] &= ~8;

	if(key[KEY_ENTER])
		keylines[2] &= ~4;

	if(key[KEY_OPENBRACE])
		keylines[2] &= ~2;

	if(key[KEY_DEL])
		keylines[2] &= ~1;

                
	if(key[KEY_STOP])
		keylines[3] &= ~128;

	if(key[KEY_SLASH])
		keylines[3] &= ~64;

	if(key[KEY_ASTERISK])
		keylines[3] &= ~32;

/*	if(key[KEY_EQYA])
		keylines[3] &= ~16;*/
                
	if(key[KEY_P])
		keylines[3] &= ~8;
                
	if(key[KEY_EQUALS])
		keylines[3] &= ~2;
                
	if(key[KEY_QUOTE])
		keylines[3] &= ~4;


	if(key[KEY_COMMA])
		keylines[4] &= ~128;

	if(key[KEY_M])
		keylines[4] &= ~64;

	if(key[KEY_K])
		keylines[4] &= ~32;

	if(key[KEY_L])
		keylines[4] &= ~16;

	if(key[KEY_I])
		keylines[4] &= ~8;

	if(key[KEY_O])
		keylines[4] &= ~4;

	if(key[KEY_9])
		keylines[4] &= ~2;

	if(key[KEY_0])
		keylines[4] &= ~1;


	if(key[KEY_SPACE])
		keylines[5] &= ~128;

	if(key[KEY_N])
		keylines[5] &= ~64;

	if(key[KEY_J])
		keylines[5] &= ~32;

	if(key[KEY_H])
		keylines[5] &= ~16;

	if(key[KEY_Y])
		keylines[5] &= ~8;

	if(key[KEY_U])
		keylines[5] &= ~4;

	if(key[KEY_7])
		keylines[5] &= ~2;

	if(key[KEY_8])
		keylines[5] &= ~1;


	if(key[KEY_V])
		keylines[6] &= ~128;

	if(key[KEY_B])
		keylines[6] &= ~64;

	if(key[KEY_F])
		keylines[6] &= ~32;

	if(key[KEY_G])
		keylines[6] &= ~16;

	if(key[KEY_T])
		keylines[6] &= ~8;

	if(key[KEY_R])
		keylines[6] &= ~4;

	if(key[KEY_5])
		keylines[6] &= ~2;

	if(key[KEY_6])
		keylines[6] &= ~1;


	if(key[KEY_X])
		keylines[7] &= ~128;

	if(key[KEY_C])
		keylines[7] &= ~64;

	if(key[KEY_D])
		keylines[7] &= ~32;

	if(key[KEY_S])
		keylines[7] &= ~16;

	if(key[KEY_W])
		keylines[7] &= ~8;

	if(key[KEY_E])
		keylines[7] &= ~4;

	if(key[KEY_3])
		keylines[7] &= ~2;

	if(key[KEY_4])
		keylines[7] &= ~1;


	if(key[KEY_Z])
		keylines[8] &= ~128;

	if(key[KEY_CAPSLOCK])
		keylines[8] &= ~64;

	if(key[KEY_A])
		keylines[8] &= ~32;

	if(key[KEY_TAB])
		keylines[8] &= ~16;

	if(key[KEY_Q])
		keylines[8] &= ~8;

	if(key[KEY_ESC])
		keylines[8] &= ~4;

	if(key[KEY_2])
		keylines[8] &= ~2;

	if(key[KEY_1])
		keylines[8] &= ~1;


	if(key[KEY_BACKSPACE])
		keylines[9] &= ~128;
}
