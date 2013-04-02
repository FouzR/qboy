#include "z80mmu.h"

#include <fstream>

z80mmu::z80mmu() {
	reset();
}

void z80mmu::reset() {
	const quint8 bios_arr[] = {0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E,
							0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3, 0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0,
							0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B,
							0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9,
							0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99, 0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20,
							0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04,
							0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2,
							0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62, 0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06,
							0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xF2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20,
							0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04, 0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17,
							0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9, 0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
							0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
							0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
							0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 0x3c, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x4C,
							0x21, 0x04, 0x01, 0x11, 0xA8, 0x00, 0x1A, 0x13, 0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20,
							0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50};
	bios = std::vector<quint8>(bios_arr, bios_arr + sizeof(bios_arr));
	inbios = true;

	rom.clear();
	eram.resize(0x2000, 0);
	wram.resize(0x2000, 0);
	vram.resize(0x2000, 0);
	voam.resize(0xA0, 0);
	zram.resize(0x100, 0);
	rombank = 1;
	rambank = 0;
	mbctype = 0;
	extram_on = false;
	ram_mode = false;
}

void z80mmu::load(std::string filename) {
	char byte;
	std::ifstream fin(filename.c_str(), std::ios_base::in | std::ios_base::binary);

	if (!fin.is_open()) return;

	rom.clear();
	while (fin.read(&byte, 1)) {
		rom.push_back(byte);
	}
	fin.close();

	mbctype = readbyte(0x0147);
}

void z80mmu::outofbios() {
	inbios = false;
}

quint8 z80mmu::readbyte(quint16 address) {
	int the_rombank;
	switch(address & 0xF000) {
	// ROM bank 0
	case 0x0000:
		if(inbios && address < 0x0100)
			return bios[address];
	case 0x1000:
	case 0x2000:
	case 0x3000:
		return rom[address];

	// ROM bank 1
	case 0x4000: case 0x5000: case 0x6000: case 0x7000:
		the_rombank = (ram_mode) ? rombank & 0x1F : rombank;
		return rom[the_rombank * 0x4000 + (address & 0x3FFF)];

	// VRAM
	case 0x8000: case 0x9000:
		return vram[address & 0x1FFF];

	// External RAM
	case 0xA000: case 0xB000:
		return extram_on && ram_mode ? eram[rambank * 0x2000 + (address & 0x1FFF)] : 0;

	// Work RAM and echo
	case 0xC000: case 0xD000: case 0xE000:
		return wram[address & 0x1FFF];

	// Everything else
	case 0xF000:
		switch(address & 0x0F00) {
		// Echo RAM
		case 0x000: case 0x100: case 0x200: case 0x300:
		case 0x400: case 0x500: case 0x600: case 0x700:
		case 0x800: case 0x900: case 0xA00: case 0xB00:
		case 0xC00: case 0xD00:
			return wram[address & 0x1FFF];

		// OAM
		case 0xE00:
			return address < 0xFEA0 ? voam[address & 0xFF] : 0;

		// Zeropage RAM, I/O
		case 0xF00:
			return zram[address & 0xFF];

		}
	}

	return 0;
}

quint16 z80mmu::readword(quint16 address) {
	quint16 ret;
	ret = readbyte(address + 1);
	ret <<= 8;
	ret |= readbyte(address);
	return ret;
}

void z80mmu::writebyte(quint16 address, quint8 value) {
	switch(address & 0xF000) {
	// bios and ROM bank 0
	case 0x0000:
	case 0x1000:
		if (mbctype == 0) return;
		extram_on = (value == 0x0A);
		break;
	case 0x2000:
	case 0x3000:
		if (mbctype == 0) return;
		value &= 0x1F;
		if (value == 0) value = 1;
		rombank = (rombank & 0x60) | value;
		break;

	// ROM bank 1
	case 0x4000:
	case 0x5000:
		if (mbctype == 0) return;
		value &= 0x03;
		if (ram_mode) {
			rambank = value;
			if (eram.size() < value * 0x2000) eram.resize(0x2000 * value, 0);
		} else {
			rombank = (value << 5) | (rombank & 0x1F);
		}
		break;
	case 0x6000:
	case 0x7000:
		if (mbctype == 0) return;
		ram_mode = value & 1;

	// VRAM
	case 0x8000: case 0x9000:
		vram[address & 0x1FFF] = value;
		break;

	// External RAM
	case 0xA000: case 0xB000:
		if (extram_on && ram_mode) eram[address & 0x1FFF] = value;
		break;

	// Work RAM and echo
	case 0xC000: case 0xD000: case 0xE000:
		wram[address & 0x1FFF] = value;
		break;

	// Everything else
	case 0xF000:
		switch(address & 0x0F00) {
		// Echo RAM
		case 0x000: case 0x100: case 0x200: case 0x300:
		case 0x400: case 0x500: case 0x600: case 0x700:
		case 0x800: case 0x900: case 0xA00: case 0xB00:
		case 0xC00: case 0xD00:
			wram[address & 0x1FFF] = value;
			break;

		// OAM
		case 0xE00:
			if(address < 0xFEA0) voam[address & 0xFF] = value;
			break;

		// Zeropage RAM, I/O
		case 0xF00:
			zram[address & 0xFF] = value;
			break;
		}

		break;
	}
}

void z80mmu::writeword(quint16 address, quint16 value) {
	writebyte(address, value & 0xFF);
	writebyte(address + 1, value >> 8);
}
