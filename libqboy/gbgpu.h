#ifndef GBGPU_H
#define GBGPU_H

#include "libqboy_global.h"
#include <vector>

const int _GBGPU_W = 160;
const int _GBGPU_H = 144;
const int _GBGPU_SPRITENUM = 40;

struct gbgpu_sprite {
	int y, x, tile;
	bool pallete1, xflip, yflip, belowbg;
	gbgpu_sprite() {
		y = 0; x = 0; tile = 0;
		pallete1 = false;
		xflip = false;
		yflip = false;
		belowbg = false;
	}
};

class gbgpu {
public:
	gbgpu();
	void reset();
	void step(int z80m);
	quint8 *getLCD();
	void setvram(quint16 address, quint8 val);
	quint8 getvram(quint16 address);
	void setvreg(quint16 address, quint8 val);
	quint8 getvreg(quint16 address);
	void setoam(quint16 address, quint8 val);
	quint8 getoam(quint16 address);
	int readandclearinterrupt();
private:
	quint8 screen_buffer[_GBGPU_H][_GBGPU_W][4];

	int mode;
	int modeclock;
	int line;

	bool updated;

	quint8 pallete_bg[4], pallete_obj0[4], pallete_obj1[4];

	bool lcd_on;
	bool bg_on, win_on;
	quint16 bg_mapbase, win_mapbase;
	bool tileset1;
	bool sprite_on, sprite_large;
	quint8 yscroll, xscroll, linecmp;
	quint8 winxpos, winypos;


	std::vector<quint8> vram, vreg, oam;
	std::vector<gbgpu_sprite> sprites;

	void renderscan();
	void updatebuffer();
	void buildsprite(int num);
};

#endif // GBGPU_H
