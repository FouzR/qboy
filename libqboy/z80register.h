#ifndef Z80REGISTER_H
#define Z80REGISTER_H

#include "libqboy_global.h"

class z80register {
public:
	z80register();
	void reset();
	void setfull(quint16 val);
	void setfull(quint8 hival, quint8 loval);
	void setlo(quint8 val);
	void sethi(quint8 val);
	quint16 getfull();
	quint8 getlo();
	quint8 gethi();
	bool getflag(char type);
	void setflag(char type, bool val);

	void operator+=(quint16 val);
	void operator-=(quint16 val);
private:
	quint8 hi, lo;
	int getflagmask(char type);
};

#endif // Z80REGISTER_H
