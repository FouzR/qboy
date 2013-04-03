#include "qboythread.h"

#include <fstream>
#include <cassert>
#include <QElapsedTimer>
#include <QFile>

qboythread::qboythread(QString filename, QObject *parent) : QThread(parent) {
	dorun = true;
	sloweddown = true;
	qboy = new libqboy();

	QFile file(filename);
	if (!file.exists()) {
		assert(false && "Could not find file");
	}
	qboy->loadgame(filename.toStdString());
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

	int thirds = 0;
	while (dorun) {
		qboy->cycle();
		if (qboy->refresh_screen()) {
			emit screen_refresh();
			int s = timer.elapsed();
			if (thirds++ == 3) {
				thirds = 0;
				s--;
			}
			if (16 - s > 0) msleep(16 - s);
			timer.restart();
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
