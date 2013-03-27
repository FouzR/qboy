#ifndef GBKEYPAD_H
#define GBKEYPAD_H

#include "libqboy_global.h"

enum GBKeypadKey {
	GBKeypadKey_RIGHT,
	GBKeypadKey_LEFT,
	GBKeypadKey_UP,
	GBKeypadKey_DOWN,
	GBKeypadKey_A,
	GBKeypadKey_B,
	GBKeypadKey_SELECT,
	GBKeypadKey_START,
	GBKeypadKey_NONE
};

class gbkeypad {
public:
	gbkeypad();
	void reset();
	void keyup(GBKeypadKey key);
	void keydown(GBKeypadKey key);
	void writebyte(quint16 address, quint8 value);
	quint8 readbyte(quint16 address);
	bool readandclearinterrupt();
private:
	quint8 row0, row1;
	quint8 column;
	bool interrupt;
};

#endif // GBKEYPAD_H
