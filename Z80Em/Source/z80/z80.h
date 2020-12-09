#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "z80types.h"
#include "z80defs.h"
#include "z80vars.h"

//INIT.C
void z80_setup(void);
void hard_reset(void);


//z80main.c
void go_z80(void);
void fail(void *var1, void *var2);
void z80_do_interrupt(void);


//z80ed.c
void z80_go_ed(void *var1, void *var2);


//z80ddfd.c
void z80_go_ddfd(void *var1, void *var2);


//z80cb.c
void z80_go_cb(void *var1, void *var2);

//normtimes.h
void z80_set_normal_timings(void);

//norminst.h
void z80_set_normal_opset(void);

