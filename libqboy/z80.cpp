#include "z80.h"

#include <cassert>

z80::z80() {
	reset();
}

void z80::cycle() {
	int op = mmu.readbyte(r.pc++);
	call(op);

	if (r.m == 0 || r.t == 0) assert(false && "No ticks added!");
	clock.m += r.m;
	clock.t += r.t;
	r.m = r.t = 0;
}

void z80::reset() {
	clock.m = 0;
	clock.t = 0;
	r.a = r.b = r.c = r.d = r.e = r.h = r.l = r.pc = r.s = r.p = r.m = r.t = 0;
	alu.reset();
}

reg_s_t* z80::getregister(int code) {
	switch (code) {
	case 7: return &r.a;
	case 0: return &r.b;
	case 1: return &r.c;
	case 2: return &r.d;
	case 3: return &r.e;
	case 4: return &r.h;
	case 5: return &r.l;
	}
	return NULL;
}

reg_s_t* z80::getregisterpair(int code, bool first) {
	switch (code) {
	case 0: return (first) ? &r.b : & r.c;
	case 1: return (first) ? &r.d : & r.e;
	case 2: return (first) ? &r.h : & r.l;
	case 3: return (first) ? &r.s : & r.p;
	}
	return NULL;
}

reg_l_t z80::getSPval() {
	reg_l_t val = (r.s << 8) | r.p;
	return val;
}

reg_s_t z80::getbytearg() {
	return mmu.readbyte(r.pc++);
}

void z80::call(reg_s_t opcode) {
	int hi2 = opcode >> 6;
	int mid3 = (opcode >> 3) & 7;
	int low3 = opcode & 7;
	int mid2 = (opcode >> 4) & 3;
	int low4 = opcode & 15;

	switch (hi2) {
	case 0:
		switch (low3) {
		case 1:
			if ((mid3 & 1) == 0) op_ld_dd_nn(mid2);
			break;
		case 2:
			op_ld_ahl_nn(mid3);
			break;
		case 6:
			op_ld_r_n(mid3);
			break;
		}
		break;
	case 1:
		if (mid3 == 6 && low3 == 6) {} //HALT
		else op_ld_r_r(mid3, low3);
		break;
	case 3:
		switch (low4) {
		case 1:
//			op_pop_qq(mid2);
			break;
		case 5:
			op_push_qq(mid2);
			break;
		}

		if (opcode == 0xF9) op_ld_sp_hl(); // 11 11 1001
		break;
	}

	op_invalid();

	return;
}

void z80::addticks(reg_l_t m, reg_l_t t) {
	r.m = m;
	r.t = t;
}

void z80::op_invalid() {
	assert(false && "Invalid or unimplemented opcode");
}

void z80::op_nop() {
	addticks(1, 4);
}

void z80::op_ld_r_r(int arg1, int arg2) {
	reg_s_t *ra = getregister(arg1),
			*rb = getregister(arg2);
	if (arg1 == 6) {
		// (HL) <- r
		reg_l_t addr = (r.h << 8) | r.l;
		mmu.writebyte(addr, *rb);
		addticks(2, 7);
	} else if (arg2 == 6) {
		// r <- (HL)
		reg_l_t addr = (r.h << 8) | r.l;
		*ra = mmu.readbyte(addr);
		addticks(2, 7);
	} else {
		// r <- r'
		*ra = *rb;
		addticks(1, 4);
	}
}

void z80::op_ld_r_n(int arg) {
	reg_s_t n = getbytearg();
	if (arg == 6) {
		reg_l_t addr = (r.h << 8) | r.l;
		mmu.writebyte(addr, n);
		addticks(3, 10);
	} else {
		reg_s_t *ra = getregister(arg);
		*ra = n;
		addticks(2, 7);
	}
}

void z80::op_ld_ahl_nn(int arg) {
	reg_l_t addr;
	if (arg == 4 || arg == 5) {
		reg_s_t n1 = getbytearg();
		reg_s_t n2 = getbytearg();
		addr = (n1 << 8) | n2;
		if (arg == 5) {
			r.l = mmu.readbyte(addr);
			r.h = mmu.readbyte(addr + 1);
		} else {
			mmu.writebyte(addr + 1, r.h);
			mmu.writebyte(addr, r.l);
		}
		addticks(5, 16);
	} else {
		switch (arg >> 1) {
		case 0:
			addr = (r.b << 8) | r.c;
			addticks(2, 7);
			break;
		case 1:
			addr = (r.d << 8) | r.e;
			addticks(2, 7);
			break;
		case 3:
			reg_s_t n1 = getbytearg();
			reg_s_t n2 = getbytearg();
			addr = (n1 << 8) | n2;
			addticks(4, 13);
			break;
		}
		if (arg && 1) {
			r.a = mmu.readbyte(addr);
		} else {
			mmu.writebyte(addr, r.a);
		}
	}
}

void z80::op_ld_dd_nn(int arg) {
	reg_s_t *ra = 0, *rb = 0;
	switch (arg) {
	case 0: ra = &r.b; rb = &r.c; break;
	case 1: ra = &r.d; rb = &r.e; break;
	case 2: ra = &r.h; rb = &r.l; break;
	case 3: ra = &r.s; rb = &r.p; break;
	}

	*ra = getbytearg();
	*rb = getbytearg();
	addticks(3, 10);
}

void z80::op_ld_sp_hl() {
	r.s = r.h;
	r.p = r.l;
	addticks(1, 6);
}

void z80::op_push_qq(int arg) {
	reg_s_t *ra = 0, *rb = 0;
	switch (arg) {
	case 0: ra = &r.b; rb = &r.c; break;
	case 1: ra = &r.d; rb = &r.e; break;
	case 2: ra = &r.h; rb = &r.l; break;
	case 3: ra = &r.a; rb = &r.f; break;
	}

	reg_l_t spaddr = (r.s << 8) | r.p;
	mmu.writebyte(spaddr - 2, *rb);
	mmu.writebyte(spaddr - 1, *ra);

	spaddr -= 2;
	r.s = spaddr >> 8;
	r.p = spaddr & 255;

	addticks(3, 11);
}
