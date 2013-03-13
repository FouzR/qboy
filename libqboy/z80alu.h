#ifndef Z80ALU_H
#define Z80ALU_H

#include "libqboy_global.h"

class z80alu {
public:
	z80alu();
	void reset();
private:
	struct flags {
		int s : 1;
		int z : 1;
		int x1 : 1;
		int n : 1;
		int x2: 1;
		int pv : 1;
		int h : 1;
		int c : 1;
	} flag;
};

#endif // Z80ALU_H
