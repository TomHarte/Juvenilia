#include "jaceports.h"
#include "jace.h"

unsigned char jace_getkeybits(unsigned char top)
{
	unsigned char result;

	result = 31;

	result &= top&0x01 ? 31 : jace_keybits[0];
	result &= top&0x02 ? 31 : jace_keybits[1];
	result &= top&0x04 ? 31 : jace_keybits[2];
	result &= top&0x08 ? 31 : jace_keybits[3];
	result &= top&0x10 ? 31 : jace_keybits[4];
	result &= top&0x20 ? 31 : jace_keybits[5];
	result &= top&0x40 ? 31 : jace_keybits[6];
	result &= top&0x80 ? 31 : jace_keybits[7];

	return result;
}

void jace_port_out(unsigned short portnum, unsigned char data)
{
}

void jace_port_in(unsigned short portnum, unsigned char *data)
{
	if((portnum&255) == 0xfe)
		*data = jace_getkeybits(portnum >> 8);
}

