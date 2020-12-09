#include "zxkeyboard.h"
#include "allegro.h"
#include "stdio.h"

struct keyinfo *keys;
unsigned char keybytes[8];

unsigned char keyindxs[] = { KEY_A,
                             KEY_B,
                             KEY_C,
                             KEY_D,
                             KEY_E,
                             KEY_F,
                             KEY_G,
                             KEY_H,
                             KEY_I,
                             KEY_J,
                             KEY_K,
                             KEY_L,
                             KEY_M,
                             KEY_N,
                             KEY_O,
                             KEY_P,
                             KEY_Q,
                             KEY_R,
                             KEY_S,
                             KEY_T,
                             KEY_U,
                             KEY_V,
                             KEY_W,
                             KEY_X,
                             KEY_Y,
                             KEY_Z,
                             
                             KEY_0,
                             KEY_1,
                             KEY_2,
                             KEY_3,
                             KEY_4,
                             KEY_5,
                             KEY_6,
                             KEY_7,
                             KEY_8,
                             KEY_9,
                             
                             KEY_0_PAD,
                             KEY_1_PAD,
                             KEY_2_PAD,
                             KEY_3_PAD,
                             KEY_4_PAD,
                             KEY_5_PAD,
                             KEY_6_PAD,
                             KEY_7_PAD,
                             KEY_8_PAD,
                             KEY_9_PAD,
                             
                             KEY_F1,
                             KEY_F2,
                             KEY_F3,
                             KEY_F4,
                             KEY_F5,
                             KEY_F6,
                             KEY_F7,
                             KEY_F8,
                             KEY_F9,
                             KEY_F10,
                             KEY_F11,
                             KEY_F12,
                             
                             KEY_ESC,
                             KEY_TILDE,
                             KEY_MINUS,
                             KEY_EQUALS,
                             KEY_BACKSPACE,
                             KEY_TAB,
                             
                             KEY_OPENBRACE,
                             KEY_CLOSEBRACE,
                             KEY_ENTER,
                             KEY_COLON,
                             KEY_QUOTE,
                             KEY_BACKSLASH,
                             
                             KEY_BACKSLASH2,
                             KEY_COMMA,
                             KEY_STOP,
                             KEY_SLASH,
                             KEY_SPACE,
                             KEY_INSERT,
                             
                             KEY_DEL,
                             KEY_HOME,
                             KEY_END,
                             KEY_PGUP,
                             KEY_PGDN,
                             KEY_LEFT,

                             KEY_RIGHT,
                             KEY_UP,
                             KEY_DOWN,
                             KEY_SLASH_PAD,
                             KEY_ASTERISK,
                             KEY_MINUS_PAD,
                             
                             KEY_PLUS_PAD,
                             KEY_DEL_PAD,
                             KEY_ENTER_PAD,
                             KEY_PRTSCR,
                             KEY_PAUSE,
                             
                             KEY_LSHIFT,
                             KEY_RSHIFT,
                             KEY_LCONTROL,
                             KEY_RCONTROL,
                             KEY_ALT,
                             KEY_ALTGR,
                             KEY_LWIN,
                             KEY_RWIN,
                             KEY_MENU,
                             KEY_SCRLOCK,
                             KEY_NUMLOCK,
                             KEY_CAPSLOCK};

#define NUMKEYS 105

void zx_load_keyb_file(char *fname)
{
   FILE *fstream;
   int count;

   if(fstream = fopen(fname, "rb"))
   {
      fseek(fstream, 7, SEEK_CUR);
      keys = malloc(sizeof(struct keyinfo)*NUMKEYS);

      for(count = 0; count < NUMKEYS; count++)
      {
        fread(&keys[count].nadata, 1, 1, fstream);
        fread(&keys[count].sadata, 1, 1, fstream);
        fread(&keys[count].cadata, 1, 1, fstream);
        fread(&keys[count].bdata,  1, 1, fstream);
        fread(&keys[count].shifts, 1, 1, fstream);
      }

      fclose(fstream);
   }
   
}

void zx_keyboard_setup(void)
{
  zx_load_keyb_file("zxspec/uk.skb");
}


void zx_update_keyboard_bytes(void)
{
   int count, offset=0;
   unsigned char *writeinfo, shinfo, idx;

   if(key[KEY_LSHIFT] || key[KEY_RSHIFT])
      offset |= 1;
   
   if(key[KEY_ALTGR] || key[KEY_ALT] || key[KEY_LCONTROL] || key[KEY_RCONTROL])
      offset |= 2;

   memset(keybytes, 31, 8);

   for(count = 0; count < NUMKEYS; count++)
   {
      idx = keyindxs[count];
   
      if(key[idx])
      {
      writeinfo = ((char *)&keys[count]) + offset;
      keybytes[(*writeinfo) >> 5] &= *writeinfo;

      shinfo = keys[count].shifts >> (6 - (offset << 1));
      
      //set shift
      keybytes[0] &= ~(shinfo&1);

      //set symb
      keybytes[7] &= ~(shinfo&2);
      }
   }
}
