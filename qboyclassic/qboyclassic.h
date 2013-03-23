#ifndef QBOYCLASSIC_H
#define QBOYCLASSIC_H

#include "qboythread.h"

#include <QMainWindow>

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
	void on_pushButton_clicked();

private:
	Ui::QBoyClassic *ui;
	qboythread *qboyt;
	QTimer *timer;
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
