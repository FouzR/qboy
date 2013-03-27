#include "z80alu.h"

#include <cassert>

z80alu::z80alu() {
}

void z80alu::setregisters(z80register *afregister, z80register *hlregister) {
	af = afregister;
	hl = hlregister;
}

void z80alu::add(quint8 b, bool withcarry) {
	quint8 a = af->gethi();
	quint8 carry = (withcarry && af->getflag('c')) ? 1 : 0;
	quint8 res = a + b + carry;

	af->setflag('z', res == 0);
	af->setflag('h', (a & 0x0F) + (b & 0x0F) + carry > 0x0F);
	af->setflag('n', false);
	af->setflag('c', a > 0xFF - b - carry);

	af->sethi(res);
}

void z80alu::sub(quint8 b, bool withcarry) {
	quint8 a = af->gethi();
	quint8 carry = (withcarry && af->getflag('c')) ? 1 : 0;
	quint8 res = a - (b + carry);

	af->setflag('z', res == 0);
	af->setflag('h', (a & 0x0F) < (b & 0x0F) + carry);
	af->setflag('n', true);
	af->setflag('c', a < b + carry);

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
	af->setflag('c', val & 0x80);
	quint8 ans = (val << 1) | bit;

	af->setflag('h', false);
	af->setflag('n', false);
	if (zflag) af->setflag('z', ans == 0);

	return ans;
}

quint8 z80alu::rlc(quint8 val, bool zflag) {
	quint8 bit = val >> 7;
	assert (bit == 0 || bit == 1); // TODO: REMOVE ME
	af->setflag('c', bit == 1);
	quint8 ans = (val << 1) | bit;

	af->setflag('h', false);
	af->setflag('n', false);
	if (zflag) af->setflag('z', ans == 0);

	return ans;
}

quint8 z80alu::sla(quint8 val) {
	af->setflag('c', val & 0x80);
	val <<= 1;

	af->setflag('z', val == 0);
	af->setflag('h', false);
	af->setflag('n', false);

	return val;
}

quint8 z80alu::sra(quint8 val) {
	af->setflag('c', (val & 1) == 1);
	val = (val >> 1) | (val & 0x80);

	af->setflag('z', val == 0);
	af->setflag('h', false);
	af->setflag('n', false);

	return val;
}

quint8 z80alu::srl(quint8 val) {
	af->setflag('c', val & 1);
	val >>= 1;

	af->setflag('z', val == 0);
	af->setflag('h', false);
	af->setflag('n', false);

	return val;
}

void z80alu::daa() {
	quint8 a = af->gethi();
	quint16 adjust = af->getflag('c') ? 0x60 : 0x00;
	if(af->getflag('h'))
		adjust |= 0x06;
	if(!af->getflag('n')) {
		if((a & 0x0F) > 0x09)
			adjust |= 0x06;
		if(a > 0x99)
			adjust |= 0x60;
		a += adjust;
	} else {
		a -= adjust;
	}

	af->setflag('c', adjust >= 0x60);
	af->setflag('h', false);
	af->setflag('z', a == 0);

	af->sethi(a);
}
