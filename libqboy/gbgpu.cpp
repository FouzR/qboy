#include "gbgpu.h"

gbgpu::gbgpu() {
	reset();
}

void gbgpu::reset() {
	mode = 0;
	modeclock = 0;
	line = 0;
	vram.resize(0x2000, 0);

	for (int x = 0; x < _GBGPU_W; ++x) {
		for (int y = 0; y < _GBGPU_H; ++y) {
			for (int c = 0; c < 4; ++c) {
				screen_buffer[x][y][4] = 255;
			}
		}
	}
}

void gbgpu::step(int z80t) {
	modeclock += z80t;

	switch (mode) {
	case 2:
		if (modeclock >= 80) {
			modeclock = 0;
			mode = 3;
		}
		break;
	case 3:
		if (modeclock >= 172) {
			modeclock = 0;
			mode = 0;
			renderscan();
		}
		break;
	case 0:
		if (modeclock >= 204) {
			modeclock = 0;
			++line;
			if (line == 143) {
				mode = 1;
				updatebuffer();
			} else {
				mode = 2;
			}
		}
		break;
	case 1:
		if (modeclock >= 456) {
			modeclock = 0;
			++line;
			if (line > 153) {
				mode = 2;
				line = 0;
			}
		}
		break;
	}
}

void gbgpu::setvram(quint16 address, quint8 val) {
	vram[address] = val;
}

quint8 gbgpu::getvram(quint16 address) {
	return vram[address];
}

void gbgpu::renderscan() {

}

void gbgpu::updatebuffer() {

}
