#include "z80register.h"

z80register::z80register() {
	isaf = false;
	reset();
}

void z80register::setaf() {
	isaf = true;
}

void z80register::reset() {
	hi = 0;
	lo = 0;
}

void z80register::setfull(quint16 val) {
	lo = val & 0xFF;
	hi = val >> 8;
	if (isaf) lo &= 0xF0;
}

void z80register::setlo(quint8 val) {
	lo = val;
	if (isaf) lo &= 0xF0;
}

void z80register::sethi(quint8 val) {
	hi = val;
}

quint16 z80register::getfull() {
	quint16 retval = hi;
	retval <<= 8;
	retval |= lo;
	return retval;
}

quint8 z80register::getlo() {
	return lo;
}

quint8 z80register::gethi() {
	return hi;
}

bool z80register::getflag(char type) {
	int bit = getflagmask(type);
	return (lo & bit);
}

void z80register::setflag(char type, bool val) {
	int bit = getflagmask(type);
	if (val) lo |= bit;
	else lo &= ~bit;
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
	case 'c': bit = 4; break;
	case 'h': bit = 5; break;
	case 'n': bit = 6; break;
	case 'z': bit = 7; break;
	}
	return (1 << bit);
}

