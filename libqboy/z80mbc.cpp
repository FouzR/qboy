#include "z80mbc.h"

z80mbc0::z80mbc0(const std::vector<quint8> &rom) {
	this->rom = rom;
	ram.resize(0x2000);
}

quint8 z80mbc0::readROM(quint16 address) {
	return rom[address];
}

quint8 z80mbc0::readRAM(quint16 address) {
	return ram[address];
}

void z80mbc0::writeROM(quint16 address, quint8 value) {
	rom[address] = value;
}

void z80mbc0::writeRAM(quint16 address, quint8 value) {
	ram[address] = value;
}

/********************************/

z80mbc1::z80mbc1(const std::vector<quint8> &rom) {
	this->rom = rom;
	ram.resize(0x10000);
	rombank = 1;
	rambank = 0;
	extram_on = false;
	ram_mode = false;
}

quint8 z80mbc1::readROM(quint16 address) {
	if (address < 0x4000) return rom[address];
	address &= 0x3FFF;
	int the_rombank = (ram_mode) ? rombank & 0x1F : rombank;
	return rom[the_rombank * 0x4000 | address];
}

quint8 z80mbc1::readRAM(quint16 address) {
	if (!extram_on || !ram_mode) return 0;
	address &= 0x1FFF;
	return ram[rambank * 0x2000 | address];
}

void z80mbc1::writeROM(quint16 address, quint8 value) {
	switch (address) {
	case 0x0000:
	case 0x1000:
		extram_on = (value == 0x0A);
		break;
	case 0x2000:
	case 0x3000:
		value &= 0x1F;
		if (value == 0) value = 1;
		rombank = (rombank & 0x60) | value;
		break;
	case 0x4000:
	case 0x5000:
		value &= 0x03;
		if (ram_mode) {
			rambank = value;
		} else {
			rombank = (value << 5) | (rombank & 0x1F);
		}
		break;
	case 0x6000:
	case 0x7000:
		ram_mode = value & 1;
		break;
	}
}

void z80mbc1::writeRAM(quint16 address, quint8 value) {
	if (!extram_on || !ram_mode) return;
	if (!ram_mode) rambank = 0;
	address &= 0x1FFF;
	ram[rambank * 0x2000 | address] = value;
}

/********************************/

z80mbc3::z80mbc3(const std::vector<quint8> &rom) {
	this->rom = rom;
	ram.resize(0x10000);
	rombank = 1;
	rambank = 0;
	extram_on = false;
}

quint8 z80mbc3::readROM(quint16 address) {
	if (address < 0x4000) return rom[address];
	address &= 0x3FFF;
	return rom[rombank * 0x4000 | address];
}

quint8 z80mbc3::readRAM(quint16 address) {
	if (!extram_on) return 0;
	if (rambank <= 3) {
		address &= 0x1FFF;
		return ram[rambank * 0x2000 | address];
	} else {
		// TODO
		return 0;
	}
}

void z80mbc3::writeROM(quint16 address, quint8 value) {
	switch (address) {
	case 0x0000:
	case 0x1000:
		extram_on = (value == 0x0A);
		break;
	case 0x2000:
	case 0x3000:
		value &= 0x7F;
		rombank = (value == 0) ? 1 : value;
		break;
	case 0x4000:
	case 0x5000:
		rambank = value;
		break;
	case 0x6000:
	case 0x7000:
		// TODO: Lock RTC...
		break;
	}
}

void z80mbc3::writeRAM(quint16 address, quint8 value) {
	if (!extram_on) return;

	if (rambank <= 3) {
		address &= 0x1FFF;
		ram[rambank * 0x2000 | address] = value;
	} else {
		// TODO
	}
}
