#include "gbgpu.h"

gbgpu::gbgpu(z80mmu *mmu) {
	this->mmu = mmu;
	reset();
}

void gbgpu::reset() {
	mode = 2;
	modeclock = 0;
	line = 0;

	for (int i = 0; i < 4; ++i) {
		pallete_bg[i] = pallete_obj0[i] = pallete_obj1[i] = 255;
	}

	for (int y = 0; y < _GBGPU_H; ++y) {
		for (int x = 0; x < _GBGPU_W; ++x) {
			for (int c = 0; c < 4; ++c) {
                screen_buffer[y][x][c] = 255;
			}
		}
	}
}

void gbgpu::step(int z80m) {
	preprocessram();

    updated = false;
    if (!lcd_on()) return;
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
	postprocessram();
}

quint8 *gbgpu::getLCD() {
	return &screen_buffer[0][0][0];
}

bool gbgpu::is_updated() {
	return updated;
}

bool gbgpu::lcd_on() {
	return mmu->readbyte(_GBGPU_VREGBASE) & 0x80;
}

bool gbgpu::bg_on() {
	return mmu->readbyte(_GBGPU_VREGBASE) & 0x01;
}

bool gbgpu::win_on() {
	return mmu->readbyte(_GBGPU_VREGBASE) & 0x20;
}

bool gbgpu::sprite_large() {
	return mmu->readbyte(_GBGPU_VREGBASE) & 0x04;
}

quint16 gbgpu::bg_mapbase() {
	if (mmu->readbyte(_GBGPU_VREGBASE) & 0x08) {
		return _GBGPU_VRAMBASE + 0x1C00;
	} else {
		return _GBGPU_VRAMBASE + 0x1800;
	}
}

quint16 gbgpu::win_mapbase() {
	if (mmu->readbyte(_GBGPU_VREGBASE) & 0x40) {
		return _GBGPU_VRAMBASE + 0x1C00;
	} else {
		return _GBGPU_VRAMBASE + 0x1800;
	}
}

bool gbgpu::tileset1() {
	return mmu->readbyte(_GBGPU_VREGBASE) & 0x10;
}

bool gbgpu::sprite_on() {
	return mmu->readbyte(_GBGPU_VREGBASE) & 0x02;
}

quint8 gbgpu::yscroll() {
	return mmu->readbyte(_GBGPU_VREGBASE + 2);
}

quint8 gbgpu::xscroll() {
	return mmu->readbyte(_GBGPU_VREGBASE + 3);
}

quint8 gbgpu::winypos() {
	return mmu->readbyte(_GBGPU_VREGBASE + 10);
}

quint8 gbgpu::winxpos() {
	return mmu->readbyte(_GBGPU_VREGBASE + 11);
}

quint8 gbgpu::linecmp() {
	return mmu->readbyte(_GBGPU_VREGBASE + 5);
}

void gbgpu::preprocessram() {
	quint8 oamdma = mmu->readbyte(_GBGPU_VREGBASE + 6);
	if (oamdma != 0) {
		quint16 baseaddr = oamdma;
		baseaddr <<= 8;
		for(quint8 i = 0; i < 0xA0; ++i) {
            mmu->writebyte(_GBGPU_VOAMBASE + i, mmu->readbyte(baseaddr + i));
		}
		mmu->writebyte(_GBGPU_VREGBASE + 6, 0);
	}

	quint8 val = mmu->readbyte(_GBGPU_VREGBASE + 7);
	for(int i = 0; i < 4; ++i) {
        switch((val >> (2*i)) & 3) {
			case 0: pallete_bg[i] = 255; break;
			case 1: pallete_bg[i] = 192; break;
			case 2: pallete_bg[i] = 96; break;
            case 3: pallete_bg[i] = 0; break;
		}
	}
	val = mmu->readbyte(_GBGPU_VREGBASE + 8);
	for(int i = 0; i < 4; ++i) {
		switch((val >> (2*i)) & 3) {
		case 0: pallete_obj0[i] = 255; break;
		case 1: pallete_obj0[i] = 192; break;
		case 2: pallete_obj0[i] = 96; break;
		case 3: pallete_obj0[i] = 0; break;
		}
	}
	val = mmu->readbyte(_GBGPU_VREGBASE + 9);
	for (int i = 0; i < 4; ++i) {
		switch((val >> (2*i)) & 3) {
		case 0: pallete_obj1[i] = 255; break;
		case 1: pallete_obj1[i] = 192; break;
		case 2: pallete_obj1[i] = 96; break;
		case 3: pallete_obj1[i] = 0; break;
		}
	}
}

void gbgpu::postprocessram() {
	mmu->writebyte(_GBGPU_VREGBASE + 4, line);

	quint8 vreg1 = mmu->readbyte(_GBGPU_VREGBASE + 1);
	vreg1 &= 0xF8;
	vreg1 |= (line == linecmp() ? 4 : 0) | (mode & 0x3);

	mmu->writebyte(_GBGPU_VREGBASE + 1, vreg1);

	bool lcdstat = false;
	if ((vreg1 & 0x40) && line == linecmp()) lcdstat = true;
	if ((vreg1 & 0x20) && mode == 2) lcdstat = true;
	if ((vreg1 & 0x10) && mode == 1) lcdstat = true;
	if ((vreg1 & 0x08) && mode == 0) lcdstat = true;

	if (lcdstat || updated) {
		quint8 int_flags = mmu->readbyte(0xFF0F);
        int_flags |= (lcdstat ? 0x2 : 0) | (updated ? 0x1 : 0);
		mmu->writebyte(0xFF0F, int_flags);
	}
}

void gbgpu::renderscan() {
	if (!lcd_on()) return;

	int winx, winy = line - winypos();
	int bgx, bgy = yscroll() + line;
	quint16 mapbase;
	int posx, posy;

	for (int x = 0; x < 160; ++x) {
		quint8 colour = 0;

		quint16 tileaddress;
		if (win_on() && winypos() <= line && x >= winxpos() - 7) {
			// select tile from window
			winx = x - (winxpos() - 7);
			posx = winx;
			posy = winy;
			mapbase = win_mapbase();
		} else if (bg_on()) {
			// select regular bg tile
			bgx = xscroll() + x;
			posx = bgx;
			posy = bgy;
			mapbase = bg_mapbase();
		} else {
			screen_buffer[line][x][0] = screen_buffer[line][x][1] = screen_buffer[line][x][2] = 255;
			continue;
		}

		int tiley = (posy >> 3) & 31;
		int tilex = (posx >> 3) & 31;

		if (tileset1()) {
			quint8 tilenr;
			tilenr = mmu->readbyte(mapbase + tiley * 32 + tilex);
			tileaddress = 0x8000 + tilenr * 16;
		} else {
			qint8 tilenr; // signed!
			tilenr = mmu->readbyte(mapbase + tiley * 32 + tilex);
			tileaddress = 0x9000 + tilenr * 16;
		}

		quint8 byte1 = mmu->readbyte(tileaddress + ((posy & 0x7) * 2));
		quint8 byte2 = mmu->readbyte(tileaddress + ((posy & 0x7) * 2) + 1);

		quint8 xbit = posx % 8;
		quint8 colnr = (byte1 & (0x80 >> xbit)) ? 1 : 0;
		colnr |= (byte2 & (0x80 >> xbit)) ? 2 : 0;
		colour = pallete_bg[colnr];

		screen_buffer[line][x][0] = screen_buffer[line][x][1] = screen_buffer[line][x][2] = colour;
	}

	if (sprite_on()) {
		int spritenum = 0;
		int spriteheight = sprite_large() ? 16 : 8;
		for (int i = 0; i < 40; ++i) {
			quint16 spriteaddr = _GBGPU_VOAMBASE + i*4;
			int spritey = mmu->readbyte(spriteaddr + 0) - 16;
			int spritex = mmu->readbyte(spriteaddr + 1) - 8;
			quint8 spritetile = mmu->readbyte(spriteaddr + 2);
			quint8 spriteflags = mmu->readbyte(spriteaddr + 3);

			if (line < spritey || line >= spritey + spriteheight)
				continue;

			spritenum++;
			if (spritenum > 10) break;

			int tiley = line - spritey;
			if (spriteflags & 0x40) tiley = spriteheight - 1 - tiley;
			if (spriteheight == 16) {
				spritetile &= 0xFE;
			}

			quint16 tileaddress = 0x8000 + spritetile * 16 + tiley * 2;
			quint8 byte1 = mmu->readbyte(tileaddress);
			quint8 byte2 = mmu->readbyte(tileaddress + 1);

			for (int x = 0; x < 8; ++x) {
				int tilex = x;
				if (spriteflags & 0x20) tilex = 7 - tilex;
				if (spritex + x < 0 || spritex + x >= 160) continue;

				int colnr = ((byte1 & (0x80 >> tilex)) ? 1 : 0)
							| ((byte2 & (0x80 >> tilex)) ? 2 : 0);
				int colour = (spriteflags & 0x10) ? pallete_obj1[colnr] : pallete_obj0[colnr];

				// colnr 0 is always transparant, and only draw on white if belowbg
				if (colnr == 0 || ((spriteflags & 0x80) && screen_buffer[line][spritex + x][0] != 255)) continue;

				screen_buffer[line][spritex + x][0] = screen_buffer[line][spritex + x][1] = screen_buffer[line][spritex + x][2] = colour;
			}
		}
	}
}
