#ifndef Z80TIMER_H
#define Z80TIMER_H

#include "libqboy_global.h"
#include "z80mmu.h"

class z80timer {
public:
	z80timer(z80mmu *mmu);
	void reset();
	void step(int z80m);
private:
	z80mmu *mmu;
	int c_main;
};

#endif // Z80TIMER_H
