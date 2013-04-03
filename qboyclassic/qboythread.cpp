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

	int gbtime = 0;
	while (dorun) {
		qboy->cycle();
		if (qboy->refresh_screen()) emit screen_refresh();
		gbtime += qboy->get_elapsed_time();
		if (gbtime > 40960) {
			int s = gbtime / 1024 - timer.elapsed();
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
