int list_amstrad(void);
int go_amstrad(int id);

extern unsigned char crtcentry, palentry, crtcregs[18], crtcsize[18], fakecrtc[18];
extern PALETTE prepal;

extern int rombase;

extern int scrmode, oldscrmode;
extern unsigned char portc, toprom, keylines[10];

extern char *cdir;
