#include "gbgpu.h"

gbgpu::gbgpu() {
	reset();
}

void gbgpu::reset() {
	mode = 0;
	modeclock = 0;
	line = 0;
	vram.resize(0x2000, 0);
	vreg.resize(10, 0);

	for (int i = 0; i < 4; ++i) {
		pallete_bg[i] = pallete_obj0[i] = pallete_obj1[i] = 255;
	}

	lcd_on = false;
	bg_on = false;
	obj_on = false;
	objsize = false;
	xscroll = yscroll = 0;
	raster = 0;
	bgtilebase = 0;
	bgmapbase = 0x1800;

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

void gbgpu::setvreg(quint16 address, quint8 val) {
	address -= 0xFF40;
	vreg[address] = val;
	switch(address) {
	case 0:
		lcd_on = (val & 0x80);
		bgtilebase = (val & 0x10) ? 0x0000 : 0x0800;
		bgmapbase = (val&0x08) ? 0x1C00 : 0x1800;
		objsize = (val&0x04);
		obj_on = (val&0x02);
		bg_on = (val&0x01);
		break;
	case 2:
		yscroll = val;
		break;
	case 3:
		xscroll = val;
		break;
	case 5:
		raster = val;

	/*
	// OAM DMA
	case 6:
		var v;
		for(var i=0; i<160; i++)
		{
			v = MMU.rb((val<<8)+i);
			GPU._oam[i] = v;
			GPU.updateoam(0xFE00+i, v);
		}
		break;
	*/

	// BG palette mapping
	case 7:
		for(int i = 0; i < 4; ++i)
		{
			switch((val >> (2*i)) & 3)
			{
			case 0: pallete_bg[i] = 255; break;
			case 1: pallete_bg[i] = 192; break;
			case 2: pallete_bg[i] = 96; break;
			case 3: pallete_bg[i] = 0; break;
			}
		}
		break;

	// OBJ0 palette mapping
	case 8:
		for(int i = 0; i < 4; ++i)
		{
			switch((val >> (2*i)) & 3)
			{
			case 0: pallete_obj0[i] = 255; break;
			case 1: pallete_obj0[i] = 192; break;
			case 2: pallete_obj0[i] = 96; break;
			case 3: pallete_obj0[i] = 0; break;
			}
		}
		break;

	// OBJ1 palette mapping
	case 9:
		for (int i = 0; i < 4; ++i)
		{
			switch((val >> (2*i)) & 3)
			{
			case 0: pallete_obj1[i] = 255; break;
			case 1: pallete_obj1[i] = 192; break;
			case 2: pallete_obj1[i] = 96; break;
			case 3: pallete_obj1[i] = 0; break;
			}
		}
		break;
	}
}

quint8 gbgpu::getvreg(quint16 address) {
	address -= 0xFF40;

	switch(address) {
	case 0:
		return (lcd_on ? 0x80 : 0) |
			   ((bgtilebase == 0x0000) ? 0x10 : 0) |
			   ((bgmapbase == 0x1C00) ? 0x08 : 0) |
			   (objsize ? 0x04 : 0) |
			   (obj_on ? 0x02 : 0) |
			   (bg_on ? 0x01 : 0);
	case 1:
		return (line == raster ? 4 : 0) | mode;
	case 2:
		return yscroll;
	case 3:
		return xscroll;
	case 4:
		return line;
	case 5:
		return raster;
	}

	return vreg[address];
}

void gbgpu::renderscan() {
	if (!lcd_on) return;
	if (bg_on) {
		// TODO
	}
	if (obj_on) {
		// TODO
	}
}

void gbgpu::updatebuffer() {

}
