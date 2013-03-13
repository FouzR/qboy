#include "z80alu.h"

z80alu::z80alu() {
	reset();
}

void z80alu::reset() {
	flag.c = flag.h = flag.n = flag.pv = flag.s = flag.x1 = flag.x2 = flag.z = false;
}
