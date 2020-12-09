#include "m6502.h"

void d_none(void)
{
}

void d_zero(void)
{
	m6502_addr = m6502_readmemory(m6502_pc++);
}

void d_zerox(void)
{
	m6502_addr = m6502_readmemory(m6502_pc++);
	m6502_addr += m6502_x;
}

void d_zeroy(void)
{
	m6502_addr = m6502_readmemory(m6502_pc++);
	m6502_addr += m6502_x;
}

void d_abs(void)
{
	m6502_addr = m6502_readmemory(m6502_pc++);
	m6502_addr |= (unsigned short)m6502_readmemory(m6502_pc++) << 8;
}

void d_absx(void)
{
	m6502_addr = m6502_readmemory(m6502_pc++);
	m6502_addr |= (unsigned short)m6502_readmemory(m6502_pc++) << 8;
	m6502_addr += m6502_x;
}

void d_absy(void)
{
	m6502_addr = m6502_readmemory(m6502_pc++);
	m6502_addr |= (unsigned short)m6502_readmemory(m6502_pc++) << 8;
	m6502_addr += m6502_y;
}

void d_indr(void)
{
	unsigned short addr;

	addr = m6502_readmemory(m6502_pc++);
	addr |= m6502_readmemory(m6502_pc++) << 8;

	m6502_addr = m6502_readmemory(addr++);
	m6502_addr |= m6502_readmemory(addr) << 8;
}

void d_indrx(void)
{
	unsigned short addr;

	addr = m6502_readmemory(m6502_pc++);
	addr += m6502_x;

	m6502_addr = m6502_readmemory(addr++);
	m6502_addr |= (unsigned short)m6502_readmemory(addr++) << 8;
}

void d_indry(void)
{
	unsigned short addr;

	addr = m6502_readmemory(m6502_pc++);

	m6502_addr = m6502_readmemory(addr++);
	m6502_addr |= (unsigned short)m6502_readmemory(addr++) << 8;
	m6502_addr += m6502_y;
}

unsigned char g_none(void)
{
	return 0;
}

unsigned char g_immd(void)
{
	return m6502_readmemory(m6502_pc++);
}

unsigned char g_mem(void)
{
	return m6502_readmemory(m6502_addr);
}

unsigned char g_a(void)
{
	return m6502_a;
}

unsigned char g_sp(void)
{
	return m6502_sp;
}

unsigned char g_x(void)
{
	return m6502_x;
}

unsigned char g_y(void)
{
	return m6502_y;
}

unsigned char g_f(void)
{
	return m6502_f;
}

unsigned char g_b0(void)
{
	return 1;
}

unsigned char g_b1(void)
{
	return 2;
}

unsigned char g_b2(void)
{
	return 4;
}

unsigned char g_b3(void)
{
	return 8;
}

unsigned char g_b4(void)
{
	return 16;
}

unsigned char g_b5(void)
{
	return 32;
}

unsigned char g_b6(void)
{
	return 64;
}

unsigned char g_b7(void)
{
	return 128;
}

void p_none(unsigned char data)
{
}

void p_mem(unsigned char data)
{
	real_p_mem(data);
}

void p_a(unsigned char data)
{
	m6502_a = data;
}

void p_sp(unsigned char data)
{
	m6502_sp = data;
}

void p_x(unsigned char data)
{
	m6502_x = data;
}

void p_y(unsigned char data)
{
	m6502_y = data;
}

void p_f(unsigned char data)
{
	m6502_f = data;
}

