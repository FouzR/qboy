#ifndef Z80ALU_H
#define Z80ALU_H

#include "libqboy_global.h"
#include "z80register.h"

class z80alu {
public:
	z80alu();
	void setflagregister(z80register *afregister);
	void add(quint8 val, bool withcarry);
	void sub(quint8 val, bool withcarry);
	void land(quint8 val);
	void lor(quint8 val);
	void lxor(quint8 val);
	void cp(quint8 val);
	quint8 inc(quint8 val);
	quint8 dec(quint8 val);
	void cpl();
	void ccf();
	void scf();
private:
	z80register *af;
};

#endif // Z80ALU_H
