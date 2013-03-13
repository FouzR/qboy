#ifndef Z80_H
#define Z80_H

#include "libqboy_global.h"
#include "z80mmu.h"
#include "z80alu.h"

enum Direction {
	LEFT,
	RIGHT
};

typedef qint8 reg_s_t;
typedef qint16 reg_l_t;

class z80 {
public:
	z80();
	void reset();
	void cycle();
private:
	struct clock_t {
		reg_l_t m, t;
	} clock;

	struct register_t {
		reg_s_t a, b, c, d, e, h, l;
		reg_l_t pc, sp;
		reg_l_t m, t;
	} r;

	z80mmu mmu;
	z80alu alu;

	void call(reg_s_t opcode);
	void addticks(reg_l_t m, reg_l_t t);

	reg_s_t alu_add(reg_s_t &a, reg_s_t &b);

	void opp_invalid();

	void opp_nop();
	void opp_ld_rr_nn(reg_s_t &ra, reg_s_t &rb);
	void opp_ld_r_n(reg_s_t &ra);
	void opp_ld_ss_a(reg_s_t &ra, reg_s_t &rb);
	void opp_inc_rr(reg_s_t &ra, reg_s_t &rb);
	void opp_inc_r(reg_s_t &ra);
	void opp_dec_r(reg_s_t &ra);
	void opp_rdca(Direction dir);
	void opp_ld_mm_sp();
	void opp_add_hl_rr(reg_s_t &ra, reg_s_t &rb);
};

#endif // Z80_H
