#include "libqboy.h"

libqboy::libqboy() : cpu(&mmu) {
	mmu.attach(&gpu, &keypad);
	reset();
}

void libqboy::reset() {
	cpu.reset();
	mmu.reset();
}

void libqboy::loadgame(std::istream &in) {
	mmu.load(in);
}

quint8 *libqboy::getLCD() {
	return gpu.getLCD();
}

void libqboy::cycle() {
	cpu.cycle();
	gpu.step(cpu.get_t());
}

int libqboy::get_elapsed_time() {
	return cpu.get_t();
}

void libqboy::keyup(GBKeypadKey key) {
	keypad.keyup(key);
}


void libqboy::keydown(GBKeypadKey key) {
	keypad.keydown(key);
}
