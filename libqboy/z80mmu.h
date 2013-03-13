#ifndef MMU_H
#define MMU_H

#include "libqboy_global.h"

class z80mmu {
public:
	z80mmu();
	quint8 readbyte(quint16 address);
	quint16 readword(quint16 address);
	void writebyte(quint16 address, quint8 value);
	void writeword(quint16 address, quint16 value);
};

#endif // MMU_H
