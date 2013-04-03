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
	void keydown(GBKeypadKey key);
	void keyup(GBKeypadKey key);
	void togglespeed();

signals:
	void screen_refresh();

public slots:

private:
	bool dorun;
	bool sloweddown;
	libqboy *qboy;
};

#endif // QBOYTHREAD_H
