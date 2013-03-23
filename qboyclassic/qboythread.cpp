#include "qboythread.h"

#include <fstream>
#include <cassert>
#include <QElapsedTimer>
#include <QDebug>

qboythread::qboythread(QString filename, QObject *parent) : QThread(parent) {
	dorun = true;
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

void qboythread::run() {
	QElapsedTimer timer;
	timer.start();

	int old_gbtime = 0, gbtime = 0;
	while (dorun) {
		qboy->cycle();
		if (qboy->doupdate()) {
			gbtime = qboy->get_elapsed_time();
			if (gbtime - old_gbtime > 10000) {
				int s = (gbtime - old_gbtime) / 4000 - timer.elapsed();
				s = qMax(0, s);
				msleep(s);
				timer.restart();
				old_gbtime = gbtime;
			}
		}
	}
}

void qboythread::stop() {
	dorun = false;
}
