#ifndef GBGPU_H
#define GBGPU_H

#include "libqboy_global.h"
#include <vector>

const int _GBGPU_W = 160;
const int _GBGPU_H = 144;

class gbgpu {
public:
	gbgpu();
	void reset();
	void step(int z80t);
	quint8 *getLCD();
	void setvram(quint16 address, quint8 val);
	quint8 getvram(quint16 address);
	void setvreg(quint16 address, quint8 val);
	quint8 getvreg(quint16 address);
private:
	quint8 screen_buffer[_GBGPU_H][_GBGPU_W][4];
	int mode;
	int modeclock;
	int line;

	quint8 pallete_bg[4], pallete_obj0[4], pallete_obj1[4];

	bool lcd_on;
	bool bg_on, win_on;
	quint16 bg_mapbase, win_mapbase;
	bool tilemap1;
	bool sprite_on, sprite_large;
	quint8 yscroll, xscroll, linecmp;
	quint8 winxpos, winypos;


	std::vector<quint8> vram, vreg;

	void renderscan();
	void updatebuffer();
};

#endif // GBGPU_H
