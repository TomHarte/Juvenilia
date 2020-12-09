#include "m6502.h"

#define pop() readmemory(0x100 | (m6502_sp++))

unsigned char m6502_push(unsigned char data)
{
	m6502_addr = 0x100 | m6502_sp;
	m6502_sp--;

	return data;
}

unsigned char m6502_pop(unsigned char data)
{
	m6502_sp++;
	return m6502_readmemory((0x100 | m6502_sp));
}

unsigned char m6502_brk(unsigned char data)
{
	m6502_pc++;
        
	m6502_addr = 0x100 | m6502_sp;
	m6502_sp--;
	real_p_mem(m6502_pc >> 8);
	m6502_addr = 0x100 | m6502_sp;
	m6502_sp--;
	real_p_mem(m6502_pc & 255);

	m6502_f |= 16;

	m6502_addr = 0x100 | m6502_sp;
	m6502_sp--;
	real_p_mem(m6502_f);

	m6502_f |= 4;
	m6502_pc = ((unsigned short)m6502_readmemory(65535) << 8) | m6502_readmemory(65534);
        
	return 0;
}

unsigned char m6502_rti(unsigned char data)
{
	m6502_sp++;
	m6502_addr = 0x100 | m6502_sp;
	m6502_f = m6502_readmemory(m6502_addr);
        
	m6502_sp++;
	m6502_addr = 0x100 | m6502_sp;
	m6502_pc = m6502_readmemory(m6502_addr);
	m6502_sp++;
	m6502_addr = 0x100 | m6502_sp;
	m6502_pc |= m6502_readmemory(m6502_addr) << 8;
        
	return 0;
}

unsigned char m6502_rts(unsigned char data)
{
	m6502_sp++;
	m6502_addr = 0x100 | m6502_sp;
	m6502_pc = m6502_readmemory(m6502_addr);
	m6502_sp++;
	m6502_addr = 0x100 | m6502_sp;
	m6502_pc |= m6502_readmemory(m6502_addr) << 8;

	m6502_pc++;
        
	return 0;
}

unsigned char m6502_jsr(unsigned char data)
{
	unsigned short addr;

	addr = m6502_addr;

	m6502_pc--;
        
	m6502_addr = 0x100 | m6502_sp;
	m6502_sp--;
	real_p_mem(m6502_pc >> 8);
	m6502_addr = 0x100 | m6502_sp;
	m6502_sp--;
	real_p_mem(m6502_pc & 255);

	m6502_pc = addr;
        
	return 0;
}

unsigned char m6502_jmp(unsigned char data)
{
	m6502_pc = m6502_addr;

	return 0;
}

unsigned char m6502_bnset(unsigned char data)
{
	if(!(m6502_f & data))
	{
		m6502_pc += m6502_displace(data);
		m6502_tstates += ((m6502_pc & 65280) != (m6502_pc & 65280)) ? 2 : 1;
	}

	return 0;
}

unsigned char m6502_bset(unsigned char data)
{
	if(m6502_f & data)
	{
		m6502_pc += m6502_displace(data);
		m6502_tstates += ((m6502_pc & 65280) != (m6502_pc & 65280)) ? 2 : 1;
	}

	return 0;
}
