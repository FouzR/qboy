#ifndef GBGPU_H
#define GBGPU_H

#include "libqboy_global.h"
#include "z80mmu.h"
#include <vector>

const int _GBGPU_W = 160;
const int _GBGPU_H = 144;
const int _GBGPU_VRAMBASE = 0x8000;
const int _GBGPU_VREGBASE = 0xFF40;
const int _GBGPU_VOAMBASE = 0xFE00;

class gbgpu {
public:
	gbgpu(z80mmu *mmu);
	void reset();
	void step(int z80m);
	bool is_updated();
	quint8 *getLCD();
private:
	z80mmu *mmu;
	quint8 screen_buffer[_GBGPU_H][_GBGPU_W][4];

	int mode;
	int modeclock;
	int line;

	bool updated;

	quint8 pallete_bg[4], pallete_obj0[4], pallete_obj1[4];
	quint8 lastdma;

	bool lcd_on();
	bool bg_on();
	bool win_on();
	quint16 bg_mapbase();
	quint16 win_mapbase();
	bool tileset1();
	bool sprite_on();
	bool sprite_large();
	quint8 yscroll();
	quint8 xscroll();
	quint8 linecmp();
	quint8 winxpos();
	quint8 winypos();


	void renderscan();
	void updatebuffer();
	void preprocessram();
	void postprocessram();
};

#endif // GBGPU_H
