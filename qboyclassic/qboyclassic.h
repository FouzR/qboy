#ifndef QBOYCLASSIC_H
#define QBOYCLASSIC_H

#include "../libqboy/libqboy.h"

#include <QMainWindow>

namespace Ui {
class QBoyClassic;
}

class QBoyClassic : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit QBoyClassic(QWidget *parent = 0);
	~QBoyClassic();
	
private slots:
	void on_pushButton_clicked();

private:
	Ui::QBoyClassic *ui;
	libqboy *qboy;
};

#endif // QBOYCLASSIC_H
