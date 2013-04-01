#include "z80timer.h"

z80timer::z80timer(z80mmu *mmu) {
	this->mmu = mmu;
	reset();
}

void z80timer::reset() {
	divider = counter = modulo = control = 0;
	c_main = c_div = 0;
	interrupt = false;
}

void z80timer::step(int z80m) {
	divider = mmu->readbyte(0xFF04);
	counter = mmu->readbyte(0xFF05);
	modulo = mmu->readbyte(0xFF06);
	control = mmu->readbyte(0xFF07);
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

	mmu->writebyte(0xFF04, divider);
	mmu->writebyte(0xFF05, counter);
	mmu->writebyte(0xFF06, modulo);
	mmu->writebyte(0xFF07, control);

	if (interrupt) {
		mmu->writebyte(0xFF0F, mmu->readbyte(0xFF0F) | 0x4);
	}
	interrupt = false;
}


bool z80timer::readandclearinterrupt() {
	if (interrupt) {
		interrupt = false;
		return true;
	}
	return false;
}
