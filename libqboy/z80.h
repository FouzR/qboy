#ifndef Z80_H
#define Z80_H

#include "libqboy_global.h"
#include "z80mmu.h"
#include "z80alu.h"
#include "z80register.h"

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
	int clock_m, clock_t;
	bool iff; // interupt flipflop

	z80mmu mmu;
	z80alu alu;
	z80register af, bc, de, hl, sp, pc;
	bool setticks;

	quint8 getbytearg();
	quint16 getwordarg();
	quint8 getbyteregisterval(int code);
	void setbyteregisterval(int code, quint8 val);
	quint16 getwordregisterval(int code, bool lastsp);
	void setwordregisterval(int code, bool lastsp, quint16 val);
	void addticks(int m, int t);

	void call(quint8 opcode);

	void op_nop();
	void op_ld_r_r(int arg1, int arg2);
	void op_ld_r_n(int arg);
	void op_ld_ahl_nn(int arg);
	void op_ld_dd_nn(int arg);
	void op_ld_sp_hl();
	void op_push_qq(int arg);
	void op_pop_qq(int arg);
	void op_add_r(int arg, bool withcarry);
	void op_add_n(bool withcarry);
	void op_sub_r(int arg, bool withcarry);
	void op_sub_n(bool withcarry);
	void op_and_r(int arg);
	void op_and_n();
	void op_or_r(int arg);
	void op_or_n();
	void op_xor_r(int arg);
	void op_xor_n();
	void op_cp_r(int arg);
	void op_cp_n();
	void op_inc_r(int arg);
	void op_dec_r(int arg);
	void op_cpl();
	void op_ccf();
	void op_scf();
	void op_halt();
	void op_di();
	void op_ei();
};

#endif // Z80_H
