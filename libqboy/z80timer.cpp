#include "z80timer.h"

z80timer::z80timer() {
	reset();
}

void z80timer::reset() {
	divider = counter = modulo = control = 0;
	c_main = c_div = 0;
	interrupt = false;
}

void z80timer::inc(int z80m) {
	while (z80m > 0) {
		z80m--;

		c_main++;
		if (c_main % 4 == 0) divider++;
		if (++c_div == 16) {
			c_div = 0;
			divider++;
		}

		if (control & 4) {
			quint8 step = 1 << (2* (control & 3));
			if (c_main % step == 0) {
				counter++;
				if (counter == 0) {
					counter = modulo;
					interrupt = true;
				}
			}
		}
	}
}

void z80timer::setbyte(quint16 address, quint8 value) {
	switch (address & 0xFF) {
	case 4:
		divider = value; break;
	case 5:
		counter = value; break;
	case 6:
		modulo = value; break;
	case 7:
		control = value; break;
	}
}

quint8 z80timer::getbyte(quint16 address) {
	switch (address & 0xFF) {
	case 4:
		return divider;
	case 5:
		return counter;
	case 6:
		return modulo;
	case 7:
		return control;
	}
	return 0;
}

bool z80timer::readandclearinterrupt() {
	if (interrupt) {
		interrupt = false;
		return true;
	}
	return false;
}
