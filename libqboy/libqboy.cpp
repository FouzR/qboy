#include "libqboy.h"

libqboy::libqboy() : cpu(&mmu), gpu(&mmu), keypad(&mmu), timer(&mmu) {
	reset();
}

void libqboy::reset() {
	mmu.reset();
	gpu.reset();
	timer.reset();
	keypad.reset();
	cpu.reset();
	passed_mtime = 0;
}

void libqboy::loadgame(std::string filename) {
	mmu.load(filename);
}

quint8 *libqboy::getLCD() {
	return gpu.getLCD();
}

void libqboy::cycle() {
	cpu.cycle();
	gpu.step(cpu.get_m());
	keypad.step();
	timer.step(cpu.get_m());

	passed_mtime += cpu.get_m();
	if (passed_mtime > 2000*1024) {
		passed_mtime = 0;
		mmu.save();
	}
}

int libqboy::get_elapsed_time() {
	return cpu.get_m();
}

void libqboy::keyup(GBKeypadKey key) {
	keypad.keyup(key);
}


void libqboy::keydown(GBKeypadKey key) {
	keypad.keydown(key);
}
