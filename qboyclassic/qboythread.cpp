#include "qboythread.h"

#include <fstream>
#include <cassert>
#include <QElapsedTimer>
#include <QDebug>

qboythread::qboythread(QString filename, QObject *parent) : QThread(parent) {
	dorun = true;
	sloweddown = true;
	qboy = new libqboy();

	std::ifstream fin(filename.toStdString().c_str(), std::ios_base::in | std::ios_base::binary);
	if (!fin.is_open()) {
		assert(false && "Could not open file");
	}
	qboy->loadgame(fin);
	fin.close();
}

qboythread::~qboythread() {
	stop();
	while (isRunning());
	delete qboy;
}

quint8* qboythread::getLCD() {
	return qboy->getLCD();
}

void qboythread::togglespeed() {
	sloweddown = !sloweddown;
}

void qboythread::run() {
	dorun = true;
	QElapsedTimer timer;
	timer.start();

	int gbtime = 0;
	while (dorun) {
		qboy->cycle();
		gbtime += qboy->get_elapsed_time();
		if (gbtime > 10000) {
			int s = gbtime / 1000 - timer.elapsed();
			s = qMax(0, s);
			timer.restart();
			if (sloweddown) msleep(s);
			gbtime = 0;
		}
	}
}

void qboythread::stop() {
	dorun = false;
}

void qboythread::keydown(GBKeypadKey key) {
	qboy->keydown(key);
}

void qboythread::keyup(GBKeypadKey key) {
	qboy->keyup(key);
}
