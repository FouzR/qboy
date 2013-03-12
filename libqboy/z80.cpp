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
	r.a = r.b = r.c = r.d = r.e = r.h = r.l = r.pc = r.sp = r.m = r.t = 0;
	r.f.s = r.f.z = r.f.x1 = r.f.n = r.f.x2 = r.f.pv = r.f.h = r.f.c = 0;
}

reg_s_t z80::alu_add(reg_s_t &a, reg_s_t &b) {
	qint16 ans = a + b;
	r.f.s = (ans & (1 << 7)) ? 1 : 0;
	r.f.z = (ans && 0xFF) ? 0 : 1;
	r.f.h = (a & 15) + (b & 15) > 15 ? 1 : 0;
	r.f.n = 0;
	r.f.c = (ans & (1 << 8)) ? 1 : 0;
	return ans;
}

void z80::call(reg_s_t opcode) {
	switch (opcode) {
	case 0x00: opp_nop(); break;
	case 0x01: opp_ld_rr_nn(r.b, r.c); break;
	case 0x02: opp_ld_ss_a(r.b, r.c); break;
	case 0x03: opp_inc_rr(r.b, r.c); break;
	case 0x04: opp_inc_r(r.b); break;
	case 0x05: opp_dec_r(r.b); break;
	case 0x06: opp_ld_r_n(r.b); break;
	case 0x07: opp_rdca(Direction::LEFT); break;
	case 0x08: opp_ld_mm_sp(); break;
	case 0x09: opp_add_hl_rr(r.b, r.c); break;
	default: opp_invalid();
	}
}

void z80::addticks(reg_l_t m, reg_l_t t) {
	clock.m += m;
	clock.t += t;
}

void z80::opp_invalid() {
	assert(false && "Invalid or unimplemented opcode");
}

void z80::opp_nop() {
	addticks(1, 4);
}

void z80::opp_ld_rr_nn(reg_s_t &ra, reg_s_t &rb) {
	ra = MMU.readbyte(r.pc++);
	rb = MMU.readbyte(r.pc++);
	addticks(3, 12);
}

void z80::opp_ld_r_n(reg_s_t &ra) {
	ra = MMU.readbyte(r.pc++);
	addticks(2, 8);
}

void z80::opp_ld_ss_a(reg_s_t &ra, reg_s_t &rb) {
	quint16 addr = (ra << 8) + rb;
	MMU.writebyte(addr, r.a);
	addticks(2, 8);
}

void z80::opp_inc_rr(reg_s_t &ra, reg_s_t &rb) {
	if (++rb == 0) ra++;
	addticks(1, 4);
}

void z80::opp_inc_r(reg_s_t &ra) {
	quint16 res = ++ra;
	r.f.s = 0; // Can never be negative?
	r.f.z = (ra == 0) ? 1 : 0;
	if (res >= (1 << 8)) r.f.c = 1;
	addticks(1, 4);
}

void z80::opp_dec_r(reg_s_t &ra) {
	quint16 res = --ra;
	if (ra == 0) r.f.z = 1;
	r.f.n = 1;
	addticks(1, 4);
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

	r.f.z = 0; // reset 0x10 bit
	if (bit != 0) r.f.z = 1;

	addticks(1, 4);
}

void z80::opp_ld_mm_sp() {
	assert(false && "You DO need to implement opp_ld_mm_sp()");
}

void z80::opp_add_hl_rr(reg_s_t &ra, reg_s_t &rb) {
	quint32 hl = (r.h << 8) + r.l;
	hl += (ra << 8) + rb;
	if (hl >= (1 << 16))

	addticks(3, 12);
}
