#include "allegro.h"

extern unsigned char channels[4], noises[3], ayregs[17], *aycurrent, mode;
extern SAMPLE *square, *noise;

#define REGISTER_WRITE 0
#define REGISTER_READ  1
#define REGISTER_SELECT 2
#define AY_INACTIVE 3

#define ay_setmode(x) mode = x

void ay_end(void);
void ay_setup(void);
void ay_sendbyte(unsigned char byte);
unsigned char ay_receivebyte(void);
void ay_keepalive(void);
void ay_io(unsigned char rnum, unsigned char data);

