#ifndef Z80REGISTER_H
#define Z80REGISTER_H

#include "libqboy_global.h"

class z80register {
public:
	z80register();
	void reset();
	void setfull(qint16 val);
	void setfull(qint8 hival, qint8 loval);
	void setlo(qint8 val);
	void sethi(qint8 val);
	qint16 getfull();
	qint8 getlo();
	qint8 gethi();
	bool getflag(char type);
	void setflag(char type, bool val);

	void operator+=(qint16 val);
	void operator-=(qint16 val);
private:
	qint8 hi, lo;
	int getflagmask(char type);
};

#endif // Z80REGISTER_H
