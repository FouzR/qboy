#include "gbkeypad.h"

gbkeypad::gbkeypad() {
	reset();
}

void gbkeypad::keydown(GBKeypadKey key) {
	switch (key) {
	case GBKeypadKey_RIGHT: row1 &= 0xE; break;
	case GBKeypadKey_LEFT: row1 &= 0xD; break;
	case GBKeypadKey_UP: row1 &= 0xB; break;
	case GBKeypadKey_DOWN: row1 &= 0x7; break;
	case GBKeypadKey_A: row0 &= 0xE; break;
	case GBKeypadKey_B: row0 &= 0xD; break;
	case GBKeypadKey_SELECT: row0 &= 0xB; break;
	case GBKeypadKey_START: row0 &= 0x7; break;
	}
	interrupt = true;
}

void gbkeypad::keyup(GBKeypadKey key) {
	switch (key) {
	case GBKeypadKey_RIGHT: row1 |= 0x1; break;
	case GBKeypadKey_LEFT: row1 |= 0x2; break;
	case GBKeypadKey_UP: row1 |= 0x4; break;
	case GBKeypadKey_DOWN: row1 |= 0x8; break;
	case GBKeypadKey_A: row0 |= 0x1; break;
	case GBKeypadKey_B: row0 |= 0x2; break;
	case GBKeypadKey_SELECT: row0 |= 0x4; break;
	case GBKeypadKey_START: row0 |= 0x8; break;
	}
}

void gbkeypad::reset() {
	row0 = row1 = 0x0F;
	column = 0;
	interrupt = false;
}

void gbkeypad::writebyte(quint16, quint8 value) {
	column = value & 0x30;
}

quint8 gbkeypad::readbyte(quint16) {
	switch (column) {
	case 0x10:
		return row0;
	case 0x20:
		return row1;
	}
	return 0;
}

bool gbkeypad::readandclearinterrupt() {
	if (interrupt) {
		interrupt = false;
		return true;
	}
	return false;
}
