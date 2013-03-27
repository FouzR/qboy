#include "gbgpu.h"

#include <cassert>
#include <iostream>

gbgpu::gbgpu() {
	reset();
}

void gbgpu::reset() {
	mode = 2;
	modeclock = 0;
	line = 0;
	vram.resize(0x2000, 0);
	vreg.resize(256, 0);
	oam.resize(160, 0);
	sprites.resize(_GBGPU_SPRITENUM);

	for (int i = 0; i < 4; ++i) {
		pallete_bg[i] = pallete_obj0[i] = pallete_obj1[i] = 255;
	}

	lcd_on = false;
	bg_on = false, win_on = false;
	sprite_on = sprite_large = false;
	xscroll = yscroll = 0;
	winxpos = winypos = 0;
	linecmp = 0;
	tileset1 = false;
	bg_mapbase = 0x1800;
	win_mapbase = 0x1800;
	updated = false;

	for (int y = 0; y < _GBGPU_H; ++y) {
		for (int x = 0; x < _GBGPU_W; ++x) {
			for (int c = 0; c < 4; ++c) {
				screen_buffer[y][x][0] = 255;
				screen_buffer[y][x][1] = 255;
				screen_buffer[y][x][2] = 255;
				screen_buffer[y][x][3] = 255;
			}
		}
	}
}

void gbgpu::step(int z80m) {
	modeclock += z80m;

	switch (mode) {
	case 2:
		if (modeclock >= 20) {
			modeclock = 0;
			mode = 3;
		}
		break;
	case 3:
		if (modeclock >= 43) {
			modeclock = 0;
			mode = 0;
			renderscan();
		}
		break;
	case 0:
		if (modeclock >= 51) {
			modeclock = 0;
			++line;
			if (line == 144) {
				mode = 1;
				updated = true;
			} else {
				mode = 2;
			}
		}
		break;
	case 1:
		if (modeclock >= 114) {
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

quint8 *gbgpu::getLCD() {
	return &screen_buffer[0][0][0];
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
		win_mapbase = (val & 0x40) ? 0x1C00 : 0x1800;
		win_on = (val & 0x20);
		tileset1 = (val & 0x10);
		bg_mapbase = (val & 0x08) ? 0x1C00 : 0x1800;
		sprite_large = (val & 0x04);
		sprite_on = (val & 0x02);
		bg_on = (val & 0x01);
		break;
	case 2:
		yscroll = val;
		break;
	case 3:
		xscroll = val;
		break;
	case 5:
		linecmp = val;
		break;

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
	case 10:
		winypos = val;
		break;
	case 11:
		winxpos = val;
		break;
	}
}

quint8 gbgpu::getvreg(quint16 address) {
	address -= 0xFF40;

	switch(address) {
	case 0:
		return (lcd_on ? 0x80 : 0) |
				(win_mapbase == 0x1C00 ? 0x40 : 0) |
				(win_on ? 0x20 : 0) |
				(tileset1 ? 0x10 : 0) |
				(bg_mapbase == 0x1C00 ? 0x08 : 0) |
				(sprite_large ? 0x04 : 0) |
				(sprite_on ? 0x02 : 0) |
				(bg_on ? 0x01 : 0);
	case 1:
		return (line == linecmp ? 4 : 0) | mode;
	case 2:
		return yscroll;
	case 3:
		return xscroll;
	case 4:
		return line;
	case 5:
		return linecmp;
	case 10:
		return winypos;
	case 11:
		return winxpos;
	}

	return vreg[address];
}

void gbgpu::setoam(quint16 address, quint8 val) {
	oam[address & 0xFF] = val;
	buildsprite((address & 0xFF) / 4);
}

quint8 gbgpu::getoam(quint16 address) {
	return address & 0xFF;
}

void gbgpu::renderscan() {
	if (!lcd_on) return;

	int winx, winy = line - winypos;
	int bgx, bgy = yscroll + line;
	quint16 mapbase;
	int posx, posy;

	for (int x = 0; x < 160; ++x) {
		quint8 colour = 0;

		quint16 tileaddress;
		if (win_on && winypos <= line && x >= winxpos - 7) {
			// select tile from window
			winx = x - (winxpos - 7);
			posx = winx;
			posy = winy;
			mapbase = win_mapbase;
		} else if (bg_on) {
			// select regular bg tile
			bgx = xscroll + x;
			posx = bgx;
			posy = bgy;
			mapbase = bg_mapbase;
		} else {
			screen_buffer[line][x][0] = screen_buffer[line][x][1] = screen_buffer[line][x][2] = 255;
			continue;
		}

		int tiley = ((posy >> 3)) << 5;
		int tilex = ((posx >> 3));

		if (tileset1) {
			quint8 tilenr;
			tilenr = vram[mapbase + tiley + tilex];
			tileaddress = tilenr * 16;
		} else {
			qint8 tilenr; // signed!
			tileaddress = 0x0800;
			tilenr = vram[mapbase + tiley + tilex];
			tileaddress += ((tilenr + 128)*16);
		}

		quint8 byte1 = vram[tileaddress + ((posy % 8) * 2)];
		quint8 byte2 = vram[tileaddress + ((posy % 8) * 2) + 1];

		quint8 xbit = 7 - (posx % 8);
		quint8 colnr = (byte1 & (1u << xbit)) ? 1 : 0;
		colnr |= (byte2 & (1u << xbit)) ? 2 : 0;
		colour = pallete_bg[colnr];

		screen_buffer[line][x][0] = screen_buffer[line][x][1] = screen_buffer[line][x][2] = colour;
	}

	if (sprite_on) {
		for (int i = 0; i < _GBGPU_SPRITENUM; ++i) {
			gbgpu_sprite sprite = sprites[i];

			if (line >= sprite.y && line < sprite.y + 8) {
				int tiley = sprite.yflip
						? 7 - (line - sprite.y)
						: line - sprite.y;

				quint16 tileaddress = sprite.tile * 16 + tiley * 2;
				quint8 byte1 = vram[tileaddress];
				quint8 byte2 = vram[tileaddress + 1];

				for (int x = 0; x < 8; ++x) {
					int tilex = sprite.xflip ? 7 - x : x;
					if (sprite.x + x < 0 || sprite.x + x >= 160) continue;

					int colnr = (byte1 & (1 << tilex)) ? 1 : 0;
					colnr |= (byte2 & (1 << tilex)) ? 2 : 0;
					int colour = (sprite.pallete1) ? pallete_obj1[colnr] : pallete_obj0[colnr];

					if (colour == 255) continue;
					if (sprite.belowbg && screen_buffer[line][sprite.x][0] != 255) continue;

					screen_buffer[line][sprite.x + x][0] = screen_buffer[line][sprite.x + x][1] = screen_buffer[line][sprite.x + x][2] = colour;
				}
			}
		}
	}
}

void gbgpu::buildsprite(int num) {
	quint16 oambase = num * 4;

	sprites[num].y = oam[oambase + 0] - 16;
	sprites[num].x = oam[oambase + 1] - 8;
	sprites[num].tile = oam[oambase + 2];

	quint8 flags = oam[oambase + 3];
	sprites[num].pallete1 = flags & 0x10;
	sprites[num].xflip = flags & 0x20;
	sprites[num].yflip = flags & 0x40;
	sprites[num].belowbg = flags & 0x80;
}

int gbgpu::readandclearinterrupt() {
	int response = 0;
	response |= updated ? 1 : 0;
	response |= line == linecmp ? 2 : 0;

	updated = false;

	return response;
}
