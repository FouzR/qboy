#include "z80register.h"

z80register::z80register() {
	reset();
}

void z80register::reset() {
	hi = 0;
	lo = 0;
}

void z80register::setfull(qint16 val) {
	lo = val & 255;
	hi = val >> 8;
}

void z80register::setfull(qint8 hival, qint8 loval) {
	lo = loval;
	hi = hival;
}

void z80register::setlo(qint8 val) {
	lo = val;
}

void z80register::sethi(qint8 val) {
	hi = val;
}

qint16 z80register::getfull() {
	qint16 retval = hi << 8 | lo;
	return retval;
}

qint8 z80register::getlo() {
	return lo;
}

qint8 z80register::gethi() {
	return hi;
}

bool z80register::getflag(char type) {
	int bit = getflagmask(type);
	return (lo & bit);
}

void z80register::setflag(char type, bool val) {
	int bit = getflagmask(type);
	if (val) lo |= bit;
	else if (lo & bit) lo -= bit;
}

void z80register::operator+=(qint16 val) {
	setfull(getfull() + val);
}

void z80register::operator-=(qint16 val) {
	setfull(getfull() - val);
}

int z80register::getflagmask(char type) {
	int bit = 0;
	switch (type) {
	case 'c': bit = 0; break;
	case 'n': bit = 1; break;
	case 'p': bit = 2; break;
	case 'v': bit = 2; break;
	case 'h': bit = 4; break;
	case 'z': bit = 6; break;
	case 's': bit = 7; break;
	}
	return (1 << bit);
}

