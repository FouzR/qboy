#ifndef LIBQBOY_H
#define LIBQBOY_H

#include "libqboy_global.h"

#include "z80.h"
#include "z80mmu.h"
#include "gbgpu.h"

#include <istream>

class LIBQBOYSHARED_EXPORT libqboy {
public:
	libqboy();
	void reset();
	quint8 *getLCD();
	void loadgame(std::istream &in);
	void cycle();
	int get_elapsed_time();
	void keyup(GBKeypadKey key);
	void keydown(GBKeypadKey key);
private:
	z80 cpu;
	z80mmu mmu;
	z80timer timer;
	gbgpu gpu;
	gbkeypad keypad;
};

#endif // LIBQBOY_H
