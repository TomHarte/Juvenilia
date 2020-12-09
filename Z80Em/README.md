# Z80Em

My first multimachine emulator; makes inaccurate approximations of:
* the ZX Spectrum;
* the Amstrad CPC;
* the Jupiter ACE; and
* the ColecoVision.

It originally depended upon Allegro 3.x; this version has been modified for Allegro 4.x.

A later version that added the MSX and Sega Master System is now lost. Since the implemented ColecoVision has a hard-coded source ROM I think this snapshot is probably from during its addition. Notably the SN76489 is incomplete.
 
Such as I recall, this emulator was never offered for public consumption.

Observations:
* despite the name, it includes my first-ever stab at a 6502 emulation, which is completely unused and completely untested;
* machines are emulated at scan-line precision, with inaccurate internal CPU time calculation;
* AY output is achieved by the very coarse means of generating static square waves in advance and changing their playback frequency in real time;
* the commentary in the Z80 implies it was originally from my very earliest emulator, a standalone ZX Spectrum effort;
* the means of abstraction appears to be permitting custom function pointers to be supplied for:
  * port in and out; and
  * an end-of-line event;
* memory mapping seems to be implemented directly in the Z80, permitting 8kb pages.
