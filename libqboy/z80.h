#ifndef Z80_H
#define Z80_H

#include "libqboy_global.h"
#include "mmu.h"

enum Direction {
	LEFT,
	RIGHT
};

class z80 {
public:
	z80();
	void reset();
	void cycle();
private:
	struct clock_t {
		quint16 m, t;
	} clock;

	struct register_t {
		quint8 a, b, c, d, e, h, l, f;
		quint16 pc, sp;
		quint16 m, t;
	} r;

	mmu MMU;

	void call(quint8 opcode);
	void addticks(quint16 ticks);
	void calcflag(quint8 result, bool substraction = false, bool carry = false);

	void opp_invalid();

	void opp_nop();
	void opp_ld_rr_nn(quint8 &ra, quint8 &rb);
	void opp_ld_r_n(quint8 &ra);
	void opp_ld_mm_a(quint8 &ra, quint8 &rb);
	void opp_inc_rr(quint8 &ra, quint8 &rb);
	void opp_inc_r(quint8 &ra);
	void opp_dec_r(quint8 &ra);
	void opp_rdca(Direction dir);
};

#endif // Z80_H
