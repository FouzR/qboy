#ifndef Z80ALU_H
#define Z80ALU_H

#include "libqboy_global.h"

class z80alu {
public:
	z80alu();
	void reset();
private:
	// flags
	bool FS;
	bool FZ;
	bool FX1;
	bool FN;
	bool FX2;
	bool FPV;
	bool FH;
	bool FC;
};

#endif // Z80ALU_H
