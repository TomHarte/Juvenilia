#include "../z80/z80.h"

static unsigned char cpc_maincyc[] = {
   4,12,16, 8, 4, 4, 8, 4, 4,12, 8, 8, 4, 4, 8, 4,
  12,12,16, 8, 4, 4, 8, 4,12,12, 8, 8, 4, 4, 8, 4,
   8,12,20, 8, 4, 4, 8, 4, 8,12,20, 8, 4, 4, 8, 4,
   8,12,16, 8,12,12,12, 4, 8,12,16, 8, 4, 4, 8, 4,
   4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
   4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
   4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
   8, 8, 8, 8, 8, 8, 4, 8, 4, 4, 4, 4, 4, 4, 8, 4,
   4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
   4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
   4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
   4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
   8,16,16,12,12,16, 8,16, 8,12,16, 0,12,20, 8,16,
   8,16,16,16,12,16, 8,16, 8, 4,16,16,12, 0, 8,16,
   8,16,16,24,12,16, 8,16, 8, 4,16, 4,12, 0, 8,16,
   8,16,16, 4,12,16, 8,16, 8, 8,16, 4,12, 0, 8,16
};

static unsigned char cpc_cbcyc[] = {
   8, 8, 8, 8, 8, 8,16, 8, 8, 8, 8, 8, 8, 8,16, 8,
   8, 8, 8, 8, 8, 8,16, 8, 8, 8, 8, 8, 8, 8,16, 8,
   8, 8, 8, 8, 8, 8,16, 8, 8, 8, 8, 8, 8, 8,16, 8,
   8, 8, 8, 8, 8, 8,16, 8, 8, 8, 8, 8, 8, 8,16, 8,
   8, 8, 8, 8, 8, 8,12, 8, 8, 8, 8, 8, 8, 8,12, 8,
   8, 8, 8, 8, 8, 8,12, 8, 8, 8, 8, 8, 8, 8,12, 8,
   8, 8, 8, 8, 8, 8,12, 8, 8, 8, 8, 8, 8, 8,12, 8,
   8, 8, 8, 8, 8, 8,12, 8, 8, 8, 8, 8, 8, 8,12, 8,
   8, 8, 8, 8, 8, 8,16, 8, 8, 8, 8, 8, 8, 8,16, 8,
   8, 8, 8, 8, 8, 8,16, 8, 8, 8, 8, 8, 8, 8,16, 8,
   8, 8, 8, 8, 8, 8,16, 8, 8, 8, 8, 8, 8, 8,16, 8,
   8, 8, 8, 8, 8, 8,16, 8, 8, 8, 8, 8, 8, 8,16, 8,
   8, 8, 8, 8, 8, 8,16, 8, 8, 8, 8, 8, 8, 8,16, 8,
   8, 8, 8, 8, 8, 8,16, 8, 8, 8, 8, 8, 8, 8,16, 8,
   8, 8, 8, 8, 8, 8,16, 8, 8, 8, 8, 8, 8, 8,16, 8,
   8, 8, 8, 8, 8, 8,16, 8, 8, 8, 8, 8, 8, 8,16, 8
};

static unsigned char cpc_ddfdcbcyc[] =
{
  20,20,20,20,20,20,20,28,20,20,20,20,20,20,20,28,
  20,20,20,20,20,20,20,28,20,20,20,20,20,20,20,28,
  20,20,20,20,20,20,20,28,20,20,20,20,20,20,20,28,
  20,20,20,20,20,20,20,28,20,20,20,20,20,20,20,28,
  16,16,16,16,16,16,16,24,16,16,16,16,16,16,16,24,
  16,16,16,16,16,16,16,24,16,16,16,16,16,16,16,24,
  16,16,16,16,16,16,16,24,16,16,16,16,16,16,16,24,
  16,16,16,16,16,16,16,24,16,16,16,16,16,16,16,24,
  20,20,20,20,20,20,20,28,20,20,20,20,20,20,20,28,
  20,20,20,20,20,20,20,28,20,20,20,20,20,20,20,28,
  20,20,20,20,20,20,20,28,20,20,20,20,20,20,20,28,
  20,20,20,20,20,20,20,28,20,20,20,20,20,20,20,28,
  20,20,20,20,20,20,20,28,20,20,20,20,20,20,20,28,
  20,20,20,20,20,20,20,28,20,20,20,20,20,20,20,28,
  20,20,20,20,20,20,20,28,20,20,20,20,20,20,20,28,
};

static unsigned char cpc_ddfdcyc[] =
{
   0, 0, 0, 0, 0, 0, 0, 0, 0,16, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0,16, 0, 0, 0, 0, 0, 0,
   0,16,24,12, 8, 8, 8, 0, 0,16,24,12, 8, 8, 8, 0,
   0, 0, 0, 0,24,24,24, 0, 0,16, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 8, 8,20, 0, 0, 0, 0, 0, 8, 8,20, 0,
   0, 0, 0, 0, 8, 8,20, 0, 0, 0, 0, 0, 8, 8,20, 0,
   8, 8, 8, 8, 8, 8,20, 8, 8, 8, 8, 8, 8, 8,20, 8,
  20,20,20,20,20,20, 0,20, 0, 0, 0, 0, 8, 8,20, 0,
   0, 0, 0, 0, 8, 8,20, 0, 0, 0, 0, 0, 8, 8,20, 0,
   0, 0, 0, 0, 8, 8,20, 0, 0, 0, 0, 0, 8, 8,20, 0,
   0, 0, 0, 0, 8, 8,20, 0, 0, 0, 0, 0, 8, 8,20, 0,
   0, 0, 0, 0, 8, 8,20, 0, 0, 0, 0, 0, 8, 8,20, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0,20, 0,28, 0,20, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0,12, 0, 0, 0, 0, 0, 0
};

static unsigned char cpc_edcyc[] =
{
   8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
   8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
   8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
   8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  16,16,16,20, 8,12, 8,12,16,16,16,20, 8,12, 8,12,
  16,16,16,20, 8,12, 8,12,16,16,16,20, 8,12, 8,12,
  16,16,16,20, 8,12, 8,20,16,16,16,20, 8,12, 8,20,
  12,12,16,20, 8,12, 8, 8,16,16,16,20, 8,12, 8, 8,
   8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
   8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  20,16,16,20, 8, 8, 8, 8,20,16,16,20, 8, 8, 8, 8,
  20,16,16,20, 8, 8, 8, 8,16,16,16,20, 8, 8, 8, 8,
   8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
   8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
   8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
   8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
};

/* 0 : DJNZ, 1 : JR COND, 2 : JP COND, 3 : CALL COND, 4 : RET COND
   5 : ???R
*/
static unsigned char cpc_additives[] =
{
  4, 4, 4, 8, 8, 4 //4,4,0,8,8,4
};

void set_cpc_timings(void)
{
	z80_maincyc = cpc_maincyc;
	z80_cbcyc = cpc_cbcyc;
	z80_ddfdcyc = cpc_ddfdcyc;
	z80_ddfdcbcyc = cpc_ddfdcbcyc;
	z80_edcyc = cpc_edcyc;
	z80_additives = cpc_additives;
}
