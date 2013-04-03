#ifndef Z80MBC_H
#define Z80MBC_H

#include "libqboy_global.h"

#include <string>
#include <vector>

class z80mbc {
public:
	virtual quint8 readROM(quint16 address) = 0;
	virtual quint8 readRAM(quint16 address) = 0;
	virtual void writeROM(quint16 address, quint8 value) = 0;
	virtual void writeRAM(quint16 address, quint8 value) = 0;
	virtual void save(std::string) {}
	virtual void load(std::string) {}
};

class z80mbc0 : public z80mbc {
public:
	z80mbc0(const std::vector<quint8> &rom);
	quint8 readROM(quint16 address);
	quint8 readRAM(quint16 address);
	void writeROM(quint16 address, quint8 value);
	void writeRAM(quint16 address, quint8 value);
protected:
	std::vector<quint8> rom, ram;
};

class z80mbc1 : public z80mbc {
public:
	z80mbc1(const std::vector<quint8> &rom);
	quint8 readROM(quint16 address);
	quint8 readRAM(quint16 address);
	void writeROM(quint16 address, quint8 value);
	void writeRAM(quint16 address, quint8 value);
protected:
	std::vector<quint8> rom, ram;
	int rombank;
	int rambank;
	bool extram_on;
	bool ram_mode;
};

class z80mbc3 : public z80mbc {
public:
	z80mbc3(const std::vector<quint8> &rom);
	quint8 readROM(quint16 address);
	quint8 readRAM(quint16 address);
	void writeROM(quint16 address, quint8 value);
	void writeRAM(quint16 address, quint8 value);
	void save(std::string filename);
	void load(std::string filename);
protected:
	std::vector<quint8> rom, ram;
	int rombank;
	int rambank;
	bool extram_on;
};

class z80mbc5 : public z80mbc {
public:
	z80mbc5(const std::vector<quint8> &rom);
	quint8 readROM(quint16 address);
	quint8 readRAM(quint16 address);
	void writeROM(quint16 address, quint8 value);
	void writeRAM(quint16 address, quint8 value);
	void save(std::string filename);
	void load(std::string filename);
protected:
	std::vector<quint8> rom, ram;
	int rombank;
	int rambank;
	bool extram_on;
};

#endif // Z80MBC_H
