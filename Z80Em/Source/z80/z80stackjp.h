void z80_push(void *var1, void *var2);
void z80_pop(void *var1, void *var2);

void z80_call(void *var1, void *var2);
void z80_callcond(void *var1, void *var2);
void z80_callncond(void *var1, void *var2);
void z80_rst(void *var1, void *var2);
void z80_ret(void *var1, void *var2);
void z80_retcond(void *var1, void *var2);
void z80_retncond(void *var1, void *var2);

void z80_jp16(void *var1, void *var2);
void z80_jpnn(void *var1, void *var2);
void z80_jrn(void *var1, void *var2);

void z80_jpcond(void *var1, void *var2);
void z80_jrcond(void *var1, void *var2);
void z80_jpncond(void *var1, void *var2);
void z80_jrncond(void *var1, void *var2);

void z80_djnz(void *var1, void *var2);