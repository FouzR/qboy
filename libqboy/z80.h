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
		reg_s_t a, b, c, d, e, h, l, s, p;
		reg_l_t pc;
		reg_l_t m, t;
	} r;

	z80mmu mmu;
	z80alu alu;

	reg_s_t* getregister(int code);
	reg_s_t* getregisterpair(int code, bool first);
	reg_l_t getSPval();

	reg_s_t getbytearg();
	void call(reg_s_t opcode);
	void addticks(reg_l_t m, reg_l_t t);

	reg_s_t alu_add(reg_s_t &a, reg_s_t &b);

	void op_invalid();

	void op_nop();
	void op_ld_r_r(int arg1, int arg2);
	void op_ld_r_n(int arg);
	void op_ld_ahl_nn(int arg);
	void op_ld_dd_nn(int arg);
	void op_ld_sp_hl();
	void op_push_qq(int arg);
};

#endif // Z80_H
