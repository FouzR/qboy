#include "libqboy.h"

libqboy::libqboy() : cpu(&mmu) {
	mmu.attach(&gpu, &keypad, &timer);
	reset();
}

void libqboy::reset() {
	timer.reset();
	mmu.reset();
	cpu.reset();
}

void libqboy::loadgame(std::istream &in) {
	mmu.load(in);
}

quint8 *libqboy::getLCD() {
	return gpu.getLCD();
}

void libqboy::cycle() {
	gpu.step(cpu.get_m());
	cpu.cycle();
	timer.inc(cpu.get_m());
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
