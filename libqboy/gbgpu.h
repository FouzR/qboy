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
private:
	quint8 screen_buffer[_GBGPU_W][_GBGPU_H][4];
	int mode;
	int modeclock;
	int line;
	std::vector<quint8> vram;

	void renderscan();
	void updatebuffer();
};

#endif // GBGPU_H
