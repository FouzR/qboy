#include "z80alu.h"

z80alu::z80alu() {
	reset();
}

void z80alu::reset() {
	FC = FH = FN = FPV = FS = FX1 = FX2 = FZ = false;
}
