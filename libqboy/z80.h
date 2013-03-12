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
		quint8 a, b, c, d, e, h, l;

		struct flags {
			quint8 s : 1;
			quint8 z : 1;
			quint8 x1 : 1;
			quint8 n : 1;
			quint8 x2: 1;
			quint8 pv : 1;
			quint8 h : 1;
			quint8 c : 1;
		} f;

		quint16 pc, sp;
		quint16 m, t;
	} r;

	mmu MMU;

	void call(quint8 opcode);
	void addticks(quint16 m, quiint16 t);

	void opp_invalid();

	void opp_nop();
	void opp_ld_rr_nn(quint8 &ra, quint8 &rb);
	void opp_ld_r_n(quint8 &ra);
	void opp_ld_ss_a(quint8 &ra, quint8 &rb);
	void opp_inc_rr(quint8 &ra, quint8 &rb);
	void opp_inc_r(quint8 &ra);
	void opp_dec_r(quint8 &ra);
	void opp_rdca(Direction dir);
	void opp_ld_mm_sp();
	void opp_add_hl_rr(quint8 &ra, quint8 &rb);
};

#endif // Z80_H
