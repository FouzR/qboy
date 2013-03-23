#ifndef QBOYTHREAD_H
#define QBOYTHREAD_H

#include "../libqboy/libqboy.h"
#include <QThread>
#include <QString>

class qboythread : public QThread {
	Q_OBJECT
public:
	explicit qboythread(QString filename, QObject *parent = 0);
	~qboythread();
	quint8 *getLCD();
	void run();
	void stop();
signals:
	
public slots:

private:
	bool dorun;
	libqboy *qboy;
};

#endif // QBOYTHREAD_H
