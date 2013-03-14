#include "z80alu.h"

z80alu::z80alu() {

}

void z80alu::setflagregister(z80register *afregister) {
	af = afregister;
}

void z80alu::add(quint8 b, bool withcarry) {
	quint8 a = af->gethi();
	quint8 carry = (withcarry && af->getflag('c')) ? 1 : 0;
	b += carry;
	quint8 res = a + b;

	af->setflag('z', res == 0);
	af->setflag('h', (a & 0x0F) + (b & 0x0F) > 0x0F);
	af->setflag('n', false);
	af->setflag('c', a > 0xFF - b);

	af->sethi(res);
}

void z80alu::sub(quint8 b, bool withcarry) {
	quint8 a = af->gethi();
	quint8 carry = (withcarry && af->getflag('c')) ? 1 : 0;
	b += carry;
	quint8 res = a - b;

	af->setflag('z', res == 0);
	af->setflag('h', (a & 0x0F) < (b & 0x0F));
	af->setflag('n', true);
	af->setflag('c', a < b);

	af->sethi(res);
}

void z80alu::land(quint8 b) {
	quint8 a = af->gethi();
	quint8 res = a & b;

	af->setflag('z', res == 0);
	af->setflag('h', true);
	af->setflag('n', false);
	af->setflag('c', false);

	af->sethi(res);
}

void z80alu::lor(quint8 b) {
	quint8 a = af->gethi();
	quint8 res = a | b;

	af->setflag('z', res == 0);
	af->setflag('h', false);
	af->setflag('n', false);
	af->setflag('c', false);

	af->sethi(res);
}

void z80alu::lxor(quint8 b) {
	quint8 a = af->gethi();
	quint8 res = a ^ b;

	af->setflag('z', res == 0);
	af->setflag('h', false);
	af->setflag('n', false);
	af->setflag('c', false);

	af->sethi(res);
}

void z80alu::cp(quint8 b) {
	quint8 a = af->gethi();
	sub(b, false);
	af->sethi(a);
}

quint8 z80alu::inc(quint8 a) {
	quint8 res = a + 1;

	af->setflag('z', res == 0);
	af->setflag('h', (a & 0x0F) + 1 > 0x0F);
	af->setflag('n', false);

	return res;
}

quint8 z80alu::dec(quint8 a) {
	quint8 res = a - 1;

	af->setflag('z', res == 0);
	af->setflag('h', a & 0x0F < 1);
	af->setflag('n', true);

	return res;
}

void z80alu::cpl() {
	af->sethi(~af->gethi());
	af->setflag('h', true);
	af->setflag('n', true);
}

void z80alu::ccf() {
	af->setflag('c', !af->getflag('c'));
	af->setflag('h', false);
	af->setflag('n', false);
}

void z80alu::scf() {
	af->setflag('c', true);
	af->setflag('h', false);
	af->setflag('n', false);
}
