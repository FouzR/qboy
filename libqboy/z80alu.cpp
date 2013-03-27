#include "z80alu.h"

z80alu::z80alu() {

}

void z80alu::setregisters(z80register *afregister, z80register *hlregister) {
	af = afregister;
	hl = hlregister;
}

void z80alu::add(quint8 b, bool withcarry) {
	quint8 a = af->gethi();
	quint8 carry = (withcarry && af->getflag('c')) ? 1 : 0;
	b += carry;
	quint8 res = a + b;

	af->setflag('z', res == 0);
	if (withcarry && carry == 1) {
		af->setflag('h', (a & 0x0F) + ((b - 1) & 0x0F) + 1 > 0x0F);
	} else {
		af->setflag('h', (a & 0x0F) + (b & 0x0F) > 0x0F);
	}
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
	if (withcarry && carry == 1) {
		af->setflag('h', (a & 0x0F) < ((b - 1) & 0x0F) + 1);
	} else {
		af->setflag('h', (a & 0x0F) < (b & 0x0F));
	}
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
	af->setflag('h', (a & 0x0F) < 1);
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

void z80alu::add16(quint16 b) {
	quint16 a = hl->getfull();
	quint16 res = a + b;

	af->setflag('h', (a & 0x07FF) + (b & 0x07FF) > 0x07FF);
	af->setflag('n', false);
	af->setflag('c', a > 0xFFFF - b);

	hl->setfull(res);
}

quint8 z80alu::rr(quint8 val, bool zflag) {
	quint8 bit = af->getflag('c') ? 1 : 0;
	af->setflag('c', (val & 1) == 1);
	quint8 ans = (val >> 1) | (bit << 7);

	af->setflag('h', false);
	af->setflag('n', false);
	if (zflag) af->setflag('z', ans == 0);

	return ans;
}

quint8 z80alu::rrc(quint8 val, bool zflag) {
	quint8 bit = val & 1;
	af->setflag('c', bit == 1);
	quint8 ans = (val >> 1) | (bit << 7);

	af->setflag('h', false);
	af->setflag('n', false);
	if (zflag) af->setflag('z', ans == 0);

	return ans;
}

quint8 z80alu::rl(quint8 val, bool zflag) {
	quint8 bit = af->getflag('c') ? 1 : 0;
	af->setflag('c', (val >> 7) == 1);
	quint8 ans = (val << 1) | bit;

	af->setflag('h', false);
	af->setflag('n', false);
	if (zflag) af->setflag('z', ans == 0);

	return ans;
}

quint8 z80alu::rlc(quint8 val, bool zflag) {
	quint8 bit = val >> 7;
	af->setflag('c', bit == 1);
	quint8 ans = (val << 1) | bit;

	af->setflag('h', false);
	af->setflag('n', false);
	if (zflag) af->setflag('z', ans == 0);

	return ans;
}

quint8 z80alu::sla(quint8 val) {
	af->setflag('c', (val >> 7) == 1);
	val <<= 1;

	af->setflag('z', val == 0);
	af->setflag('h', false);
	af->setflag('n', false);

	return val;
}

quint8 z80alu::sra(quint8 val) {
	af->setflag('c', (val & 1) == 1);
	int bit = val & (1 << 7);
	val = (val >> 1) | bit;

	af->setflag('z', val == 0);
	af->setflag('h', false);
	af->setflag('n', false);

	return val;
}

quint8 z80alu::srl(quint8 val) {
	af->setflag('c', (val & 1) == 1);
	val >>= 1;

	af->setflag('z', val == 0);
	af->setflag('h', false);
	af->setflag('n', false);

	return val;
}
