#ifndef QBOYCLASSIC_H
#define QBOYCLASSIC_H

#include "qboythread.h"

#include <QMainWindow>
#include <QKeyEvent>

#include <QLabel>
#include <QPainter>
#include <QTimer>

namespace Ui {
class QBoyClassic;
}

class QBoyClassic : public QMainWindow {
	Q_OBJECT
	
public:
	explicit QBoyClassic(QWidget *parent = 0);
	~QBoyClassic();

private slots:

private:
	Ui::QBoyClassic *ui;
	qboythread *qboyt;
	QTimer *timer;

	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
	GBKeypadKey qtkeytogb(int qtkey);
};


/*********/

class ImagePlacer : public QLabel {
	Q_OBJECT
public:
	ImagePlacer(QWidget *parent) : QLabel(parent) {}
	QImage *image;


	void paintEvent (QPaintEvent *){
		if (image != 0)
		{
		  QPainter painter(this);
		  painter.drawImage(this->rect(), *image, this->rect());
		}
	  }
};

#endif // QBOYCLASSIC_H
