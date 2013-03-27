#ifndef Z80TIMER_H
#define Z80TIMER_H

#include "libqboy_global.h"

class z80timer {
public:
	z80timer();
	void reset();
	void inc(int z80m);
	void setbyte(quint16 address, quint8 value);
	quint8 getbyte(quint16 address);
	bool readandclearinterrupt();
private:
	quint8 divider, counter, modulo, control;
	quint8 c_main, c_div;
	bool interrupt;
};

#endif // Z80TIMER_H
