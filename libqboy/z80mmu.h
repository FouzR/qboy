#ifndef MMU_H
#define MMU_H

#include "libqboy_global.h"

#include <istream>
#include <vector>

class z80mmu {
public:
	z80mmu();
	void reset();
	void load(std::istream &in);
	void outofbios();
	quint8 readbyte(quint16 address);
	quint16 readword(quint16 address);
	void writebyte(quint16 address, quint8 value);
	void writeword(quint16 address, quint16 value);

private:
	bool inbios;
	std::vector<quint8> bios, rom, eram, wram, zram;
};

#endif // MMU_H
