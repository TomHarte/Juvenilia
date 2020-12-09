struct keyinfo
{
   unsigned char nadata, sadata, cadata, bdata, shifts;
   //nadat, sadata, cadata, bdata, - low 5 bits = &mask, top 3 bits = keyboard element
   //shifts = spare : (n)symb:caps : (s)symb:caps : (c)symb:caps
};

extern struct keyinfo *keys;
extern unsigned char keybytes[8];
void zx_keyboard_setup(void);
void zx_update_keyboard_bytes(void);

