#include "z80.h"

#include <cassert>

z80::z80() {
	reset();
}

void z80::cycle() {
	int op = MMU.readbyte(r.pc++);
	call(op);
	clock.m += r.m;
	clock.t += r.t;
}

void z80::reset() {
	clock.m = 0;
	clock.t = 0;
	r.a = r.b = r.c = r.d = r.e = r.h = r.l = r.f = r.pc = r.sp = r.m = r.t = 0;
}

void z80::call(quint8 opcode) {
	switch (opcode) {
	case 0x00: opp_nop(); break;
	case 0x01: opp_ld_rr_nn(r.b, r.c); break;
	case 0x02: opp_ld_mm_a(r.b, r.c); break;
	case 0x03: opp_inc_rr(r.b, r.c); break;
	case 0x04: opp_inc_r(r.b); break;
	case 0x05: opp_rdca(Direction::LEFT);
	default: opp_invalid();
	}
}

void z80::addticks(quint16 ticks) {
	clock.m += ticks;
	clock.t += ticks * 4;
}

void z80::calcflag(quint8 result, bool substraction, bool carry) {
	quint8 newflag = 0;
	if (result == 0) newflag |= 0x80;
	if (substraction) newflag |= 0x40;
	if (carry) newflag |= 0x10;
	r.f = newflag;
}

void z80::opp_invalid() {
	assert(false && "Invalid or unimplemented opcode");
}

void z80::opp_nop() {
	addticks(1);
}

void z80::opp_ld_rr_nn(quint8 &ra, quint8 &rb) {
	ra = MMU.readbyte(r.pc++);
	rb = MMU.readbyte(r.pc++);
	addticks(3);
}

void z80::opp_ld_r_n(quint8 &ra) {
	ra = MMU.readbyte(r.pc++);
	addticks(2);
}

void z80::opp_ld_mm_a(quint8 &ra, quint8 &rb) {
	quint16 addr = (ra << 8) + rb;
	MMU.writebyte(addr, r.a);
	addticks(2);
}

void z80::opp_inc_rr(quint8 &ra, quint8 &rb) {
	if (++rb == 0) ra++;
	addticks(1);
}

void z80::opp_inc_r(quint8 &ra) {
	calcflag(++ra);
	addticks(1);
}

void z80::opp_dec_r(quint8 &ra) {
	calcflag(--ra);
	addticks(1);
}

void z80::opp_rdca(Direction dir) {
	int bit = 0;
	if (dir == Direction::LEFT) {
		bit = r.a & (1 << 7) ? 1 : 0;
		r.a = (r.a << 1) + bit;
	} else {
		bit = r.a & 1 ? (1 << 7) : 0;
		r.a = (r.a >> 1) + bit;
	}

	r.f &= 0xEF; // magic?
	if (bit != 0) r.f += 0x10;

	addticks(1);
}
