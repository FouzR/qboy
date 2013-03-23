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
	void setvram(quint16 address, quint8 val);
	quint8 getvram(quint16 address);
	void setvreg(quint16 address, quint8 val);
	quint8 getvreg(quint16 address);
private:
	quint8 screen_buffer[_GBGPU_W][_GBGPU_H][4];
	int mode;
	int modeclock;
	int line;
	quint8 pallete_bg[4], pallete_obj0[4], pallete_obj1[4];
	bool lcd_on;
	quint16 bgtilebase;
	quint16 bgmapbase;
	bool objsize;
	bool obj_on;
	bool bg_on;
	quint8 yscroll, xscroll, raster;


	std::vector<quint8> vram, vreg;

	void renderscan();
	void updatebuffer();
};

#endif // GBGPU_H
