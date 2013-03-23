#include "libqboy.h"

libqboy::libqboy() : cpu(&mmu) {
	reset();
}

void libqboy::reset() {
	cpu.reset();
	gpu.reset();
	mmu.reset();
	mmu.setgpu(&gpu);
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

bool libqboy::doupdate() {
	if (gpu.updated) {
		gpu.updated = false;
		return true;
	}
	return false;
}

int libqboy::get_elapsed_time() {
	return cpu.get_t();
}
