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
private:
	z80 cpu;
	z80mmu mmu;
	gbgpu gpu;
};

#endif // LIBQBOY_H
