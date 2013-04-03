#ifndef QBOYCLASSIC_H
#define QBOYCLASSIC_H

#include "qboythread.h"

#include <QMainWindow>
#include <QKeyEvent>

#include <QLabel>
#include <QPainter>

namespace Ui {
class QBoyClassic;
}

class QBoyClassic : public QMainWindow {
	Q_OBJECT
	
public:
	explicit QBoyClassic(QWidget *parent = 0);
	~QBoyClassic();

private slots:
	void on_btnload_clicked();
	void on_btnpause_clicked();
	void on_btnrestart_clicked();
	void on_btnspeed_clicked();

private:
	Ui::QBoyClassic *ui;
	qboythread *qboyt;
	QString filename;

	void startGameBoy();
	void stopGameBoy();
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
	GBKeypadKey qtkeytogb(int qtkey);
};


/*********/

class ImagePlacer : public QLabel {
	Q_OBJECT
public:
	ImagePlacer(QWidget *parent) : QLabel(parent), image(0) {}
	QImage *image;

	void paintEvent (QPaintEvent *){
		if (image != 0) {
			QPainter painter(this);
			QRect r(0, 0, 160, 144);
			painter.drawImage(this->rect(), *image, r);
		}
	}
};

#endif // QBOYCLASSIC_H
