#include "z80.h"

#include <cassert>

z80::z80() {
	reset();
}

void z80::cycle() {
	call(getbytearg());

	if (!setticks) assert(false && "No ticks added! (incorrect or no instruction)");
	setticks = false;
}

void z80::reset() {
	clock_m = 0;
	clock_t = 0;
	setticks = false;
	iff = false;

	af.reset();
	bc.reset();
	de.reset();
	hl.reset();
	pc.reset();
	sp.reset();

	alu.setregisters(&af, &hl);
}

quint8 z80::getbyteregisterval(int code) {
	switch (code) {
	case 7: return af.gethi();
	case 0: return bc.gethi();
	case 1: return bc.getlo();
	case 2: return de.gethi();
	case 3: return de.getlo();
	case 4: return hl.gethi();
	case 5: return hl.getlo();
	}
	return 0;
}

void z80::setbyteregisterval(int code, quint8 val) {
	switch (code) {
	case 7: af.sethi(val); break;
	case 0: bc.sethi(val); break;
	case 1: bc.setlo(val); break;
	case 2: de.sethi(val); break;
	case 3: de.setlo(val); break;
	case 4: hl.sethi(val); break;
	case 5: hl.setlo(val); break;
	}
}

quint16 z80::getwordregisterval(int code, bool lastsp) {
	switch (code) {
	case 0: return bc.getfull();
	case 1: return de.getfull();
	case 2: return hl.getfull();
	case 3: return (lastsp) ? sp.getfull() : af.getfull();
	}
	return 0;
}

void z80::setwordregisterval(int code, bool lastsp, quint16 val) {
	switch (code) {
	case 0: bc.setfull(val); break;
	case 1: de.setfull(val); break;
	case 2: hl.setfull(val); break;
	case 3: if (lastsp) sp.setfull(val); else af.setfull(val); break;
	}
}

quint8 z80::getbytearg() {
	quint8 retval = mmu.readbyte(pc.getfull());
	pc += 1;
	return retval;
}

quint16 z80::getwordarg() {
	quint16 retval;
	retval = mmu.readbyte(pc.getfull());
	retval <<= 8;
	pc += 1;
	retval |= mmu.readbyte(pc.getfull());
	pc += 1;
	return retval;
}

void z80::addticks(int m, int t) {
	clock_m += m;
	clock_t += t;
	setticks = true;
}

void z80::call(quint8 opcode) {
	int hi2 = opcode >> 6;
	int mid3 = (opcode >> 3) & 7;
	int low3 = opcode & 7;
	int mid2 = (opcode >> 4) & 3;
	int low4 = opcode & 15;

	switch (hi2) {
	case 0:
		switch (low3) {
		case 0:
			if (mid3 == 0) op_nop();
			break;
		case 1:
			if ((mid3 & 1) == 0) op_ld_dd_nn(mid2);
			else op_add16_rr(mid2);
			break;
		case 3:
			if ((mid3 & 1) == 0) op_inc16_rr(mid2);
			else op_dec16_rr(mid2);
			break;
		case 2:
			op_ld_ahl_nn(mid3); break;
		case 4:
			op_inc_r(mid3); break;
		case 5:
			op_dec_r(mid3); break;
		case 6:
			op_ld_r_n(mid3); break;
		case 7:
			switch (mid3) {
			case 0:
				op_rdca(Direction::LEFT); break;
			case 1:
				op_rdca(Direction::RIGHT); break;
			case 2:
				op_rda(Direction::LEFT); break;
			case 3:
				op_rda(Direction::RIGHT); break;
			case 5:
				op_cpl(); break;
			case 6:
				op_scf(); break;
			case 7:
				op_ccf(); break;
			}
			break;
		}
		break;
	case 1:
		if (mid3 == 6 && low3 == 6) op_halt();
		else op_ld_r_r(mid3, low3);
		break;
	case 2:
		switch (mid3) {
		case 0:
			op_add_r(low3, false); break;
		case 1:
			op_add_r(low3, true); break;
		case 2:
			op_sub_r(low3, false); break;
		case 3:
			op_sub_r(low3, true); break;
		case 4:
			op_and_r(low3); break;
		case 5:
			op_xor_r(low3); break;
		case 6:
			op_or_r(low3); break;
		case 7:
			op_cp_r(low3); break;
		}
		break;
	case 3:
		switch (low3) {
		case 1:
			if ((mid3 & 1) == 0) op_pop_qq(mid2);
			else if (mid3 == 7) op_ld_sp_hl();
			break;
		case 3:
			switch (mid3) {
			case 0:
				op_jump(); break;
			case 1:
				call_extended(); break;
			case 6:
				op_di(); break;
			case 7:
				op_ei(); break;
			}
			break;
		case 5:
			if ((mid3 & 1) == 0) op_push_qq(mid2);
			break;
		case 6:
			switch (mid3) {
			case 0:
				op_add_n(false); break;
			case 1:
				op_add_n(true); break;
			case 2:
				op_sub_n(false); break;
			case 3:
				op_sub_n(true); break;
			case 4:
				op_and_n(); break;
			case 5:
				op_xor_n(); break;
			case 6:
				op_or_n(); break;
			case 7:
				op_cp_n(); break;
			}
			break;
		}
		break;
	}
}

void z80::call_extended() {
	int opcode = getbytearg();
	int arg = opcode & 7;
	opcode >>= 3;

	int hi2 = opcode >> 3;
	int mid3 = opcode & 7;

	switch (hi2) {
	case 0:
		switch (mid3) {
		case 0:
			op_rdc_r(arg, Direction::LEFT); break;
		case 1:
			op_rdc_r(arg, Direction::RIGHT); break;
		case 2:
			op_rd_r(arg, Direction::LEFT); break;
		case 3:
			op_rd_r(arg, Direction::RIGHT); break;
		case 4:
			op_sda(arg, Direction::LEFT); break;
		case 5:
			op_sda(arg, Direction::RIGHT); break;
		case 6:
			// TODO: swap
			break;
		case 7:
			op_srl(arg); break;
		}
		break;
	case 1:
		op_bit(mid3, arg); break;
	case 2:
		op_res(mid3, arg); break;
	case 3:
		op_set(mid3, arg); break;
	}
}

void z80::op_nop() {
	addticks(1, 4);
}

void z80::op_ld_r_r(int arg1, int arg2) {
	if (arg1 == 6) {
		// (HL) <- r
		mmu.writebyte(hl.getfull(), getbyteregisterval(arg2));
		addticks(2, 7);
	} else if (arg2 == 6) {
		// r <- (HL)
		setbyteregisterval(arg1, mmu.readbyte(hl.getfull()));
		addticks(2, 7);
	} else {
		// r <- r'
		setbyteregisterval(arg1, getbyteregisterval(arg2));
		addticks(1, 4);
	}
}

void z80::op_ld_r_n(int arg) {
	quint8 n = getbytearg();
	if (arg == 6) {
		// (HL) <- n;
		mmu.writebyte(hl.getfull(), n);
		addticks(3, 10);
	} else {
		// r <- n
		setbyteregisterval(arg, n);
		addticks(2, 7);
	}
}

void z80::op_ld_ahl_nn(int arg) {
	quint16 addr;
	if (arg == 4 || arg == 5) {
		addr = getwordarg();
		if (arg == 5) {
			hl.setfull(mmu.readword(addr));
			// TODO: make sure readword works like this:
			//r.l = mmu.readbyte(addr);
			//r.h = mmu.readbyte(addr + 1);
		} else {
			mmu.writeword(addr, hl.getfull());
			// TODO: make sure writeword works like this:
			//mmu.writebyte(addr + 1, r.h);
			//mmu.writebyte(addr, r.l);
		}
		addticks(5, 16);
	} else {
		switch (arg >> 1) {
		case 0:
			addr = bc.getfull();
			addticks(2, 7);
			break;
		case 1:
			addr = de.getfull();
			addticks(2, 7);
			break;
		case 3:
			addr = getwordarg();
			addticks(4, 13);
			break;
		}
		if (arg & 1) {
			af.sethi(mmu.readbyte(addr));
		} else {
			mmu.writebyte(addr, af.gethi());
		}
	}
}

void z80::op_ld_dd_nn(int arg) {
	quint16 nn = getwordarg();
	setwordregisterval(arg, true, nn);
	addticks(3, 10);
}

void z80::op_ld_sp_hl() {
	sp.setfull(hl.getfull());
	addticks(1, 6);
}

void z80::op_push_qq(int arg) {
	sp -= 2;
	mmu.writeword(sp.getfull(), getwordregisterval(arg, false));
	addticks(3, 11);
}

void z80::op_pop_qq(int arg) {
	setwordregisterval(arg, false, mmu.readword(sp.getfull()));
	sp += 2;
	addticks(3, 10);
}

void z80::op_add_r(int arg, bool withcarry) {
	if (arg == 6) {
		alu.add(mmu.readbyte(hl.getfull()), withcarry);
		addticks(2, 7);
	} else {
		alu.add(getbyteregisterval(arg), withcarry);
		addticks(1, 4);
	}
}

void z80::op_add_n(bool withcarry) {
	alu.add(getbytearg(), withcarry);
	addticks(2, 7);
}

void z80::op_sub_r(int arg, bool withcarry) {
	if (arg == 6) {
		alu.sub(mmu.readbyte(hl.getfull()), withcarry);
		addticks(2, 7);
	} else {
		alu.sub(getbyteregisterval(arg), withcarry);
		addticks(1, 4);
	}
}

void z80::op_sub_n(bool withcarry) {
	alu.sub(getbytearg(), withcarry);
	addticks(2, 7);
}

void z80::op_and_r(int arg) {
	if (arg == 6) {
		alu.land(mmu.readbyte(hl.getfull()));
		addticks(2, 7);
	} else {
		alu.land(getbyteregisterval(arg));
		addticks(1, 4);
	}
}

void z80::op_and_n() {
	alu.land(getbytearg());
	addticks(2, 7);
}

void z80::op_or_r(int arg) {
	if (arg == 6) {
		alu.lor(mmu.readbyte(hl.getfull()));
		addticks(2, 7);
	} else {
		alu.lor(getbyteregisterval(arg));
		addticks(1, 4);
	}
}

void z80::op_or_n() {
	alu.lor(getbytearg());
	addticks(2, 7);
}

void z80::op_xor_r(int arg) {
	if (arg == 6) {
		alu.lxor(mmu.readbyte(hl.getfull()));
		addticks(2, 7);
	} else {
		alu.lxor(getbyteregisterval(arg));
		addticks(1, 4);
	}
}

void z80::op_xor_n() {
	alu.lxor(getbytearg());
	addticks(2, 7);
}

void z80::op_cp_r(int arg) {
	if (arg == 6) {
		alu.cp(mmu.readbyte(hl.getfull()));
		addticks(2, 7);
	} else {
		alu.cp(getbyteregisterval(arg));
		addticks(1, 4);
	}
}

void z80::op_cp_n() {
	alu.cp(getbytearg());
	addticks(2, 7);
}

void z80::op_inc_r(int arg) {
	if (arg == 6) {
		mmu.writebyte(hl.getfull(), alu.inc(mmu.readbyte(hl.getfull())));
		addticks(3, 11);
	} else {
		setbyteregisterval(arg, alu.inc(getbyteregisterval(arg)));
		addticks(1, 4);
	}
}

void z80::op_dec_r(int arg) {
	if (arg == 6) {
		mmu.writebyte(hl.getfull(), alu.dec(mmu.readbyte(hl.getfull())));
		addticks(3, 11);
	} else {
		setbyteregisterval(arg, alu.dec(getbyteregisterval(arg)));
		addticks(1, 4);
	}
}

void z80::op_cpl() {
	alu.cpl();
	addticks(1, 4);
}

void z80::op_ccf() {
	alu.ccf();
	addticks(1, 4);
}

void z80::op_scf() {
	alu.scf();
	addticks(1, 4);
}

void z80::op_halt() {
	// if???
	assert(false && "Halt called, not implemented");
}

void z80::op_di() {
	iff = false;
	addticks(1, 4);
}

void z80::op_ei() {
	iff = true;
	addticks(1, 4);
}

void z80::op_add16_rr(int arg) {
	alu.add16(getwordregisterval(arg, true));
	addticks(3, 11);
}

void z80::op_inc16_rr(int arg) {
	setwordregisterval(arg, true, getwordregisterval(arg, true) + 1);
	addticks(1, 6);
}

void z80::op_dec16_rr(int arg) {
	setwordregisterval(arg, true, getwordregisterval(arg, true) - 1);
	addticks(1, 6);
}

void z80::op_rdca(Direction dir) {
	quint8 ans = af.gethi();
	if (dir == Direction::LEFT) {
		ans = alu.rlc(ans, false);
	} else {
		ans = alu.rrc(ans, false);
	}
	af.sethi(ans);

	addticks(1, 4);
}

void z80::op_rda(Direction dir) {
	quint8 ans = af.gethi();
	if (dir == Direction::LEFT) {
		ans = alu.rl(ans, false);
	} else {
		ans = alu.rr(ans, false);
	}
	af.sethi(ans);

	addticks(1, 4);
}

void z80::op_rdc_r(int arg, Direction dir) {
	quint8 ans;
	if (arg == 6) {
		quint16 addr = hl.getfull();
		ans = mmu.readbyte(addr);
		if (dir == Direction::LEFT) {
			ans = alu.rlc(ans);
		} else {
			ans = alu.rrc(ans);
		}
		mmu.writebyte(addr, ans);

		addticks(4, 15);
	} else {
		ans = getbyteregisterval(arg);
		if (dir == Direction::LEFT) {
			ans = alu.rlc(ans);
		} else {
			ans = alu.rrc(ans);
		}
		setbyteregisterval(arg, ans);

		addticks(2, 8);
	}
}

void z80::op_rd_r(int arg, Direction dir) {
	quint8 ans;
	if (arg == 6) {
		quint16 addr = hl.getfull();
		ans = mmu.readbyte(addr);
		if (dir == Direction::LEFT) {
			ans = alu.rl(ans);
		} else {
			ans = alu.rr(ans);
		}
		mmu.writebyte(addr, ans);

		addticks(4, 15);
	} else {
		ans = getbyteregisterval(arg);
		if (dir == Direction::LEFT) {
			ans = alu.rl(ans);
		} else {
			ans = alu.rr(ans);
		}
		setbyteregisterval(arg, ans);

		addticks(2, 8);
	}
}

void z80::op_sda(int arg, Direction dir) {
	quint8 ans;
	if (arg == 6) {
		quint16 addr = hl.getfull();
		ans = mmu.readbyte(addr);
		if (dir == Direction::LEFT) {
			ans = alu.sla(ans);
		} else {
			ans = alu.sra(ans);
		}
		mmu.writebyte(addr, ans);

		addticks(4, 15);
	} else {
		ans = getbyteregisterval(arg);
		if (dir == Direction::LEFT) {
			ans = alu.sla(ans);
		} else {
			ans = alu.sra(ans);
		}
		setbyteregisterval(arg, ans);

		addticks(2, 8);
	}
}

void z80::op_srl(int arg) {
	quint8 ans;
	if (arg == 6) {
		quint16 addr = hl.getfull();
		ans = mmu.readbyte(addr);
		ans = alu.srl(ans);
		mmu.writebyte(addr, ans);

		addticks(4, 15);
	} else {
		ans = getbyteregisterval(arg);
		ans = alu.srl(ans);
		setbyteregisterval(arg, ans);

		addticks(2, 8);
	}
}

void z80::op_bit(int bit, int arg) {
	quint8 ans;
	quint8 test = (1 << bit);
	if (arg == 6) {
		quint16 addr = hl.getfull();
		ans = mmu.readbyte(addr);
		af.setflag('z', ans & test);
		addticks(3, 12);
	} else {
		ans = getbyteregisterval(arg);
		af.setflag('z', ans & test);
		addticks(2, 8);
	}
}

void z80::op_set(int bit, int arg) {
	quint8 ans;
	quint8 test = (1 << bit);

	if (arg == 6) {
		quint16 addr = hl.getfull();
		ans = mmu.readbyte(addr);
		mmu.writebyte(addr, ans | test);
		addticks(4, 15);
	} else {
		ans = getbyteregisterval(arg);
		setbyteregisterval(arg, ans | test);
		addticks(2, 8);
	}
}

void z80::op_res(int bit, int arg) {
	quint8 ans;
	quint8 test = (1 << bit);
	test = ~test;

	if (arg == 6) {
		quint16 addr = hl.getfull();
		ans = mmu.readbyte(addr);
		mmu.writebyte(addr, ans & test);
		addticks(4, 15);
	} else {
		ans = getbyteregisterval(arg);
		setbyteregisterval(arg, ans & test);
		addticks(2, 8);
	}
}

void z80::op_jump() {
	pc.setfull(getwordarg());
	addticks(3, 10);
}
