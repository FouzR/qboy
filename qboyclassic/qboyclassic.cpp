#include "qboyclassic.h"
#include "ui_qboyclassic.h"

QBoyClassic::QBoyClassic(QWidget *parent) :
    QMainWindow(parent),
	ui(new Ui::QBoyClassic) {

	ui->setupUi(this);

	// Repaint the label at set intervals
	timer = new QTimer();
	timer->setInterval(25);
	connect(timer, SIGNAL(timeout()), ui->label, SLOT(repaint()));
	timer->start();

	// Create gameboy
	qboyt = new qboythread("opus5.gb");
	qboyt->start();

	ui->label->image = new QImage(qboyt->getLCD(), 160, 144, QImage::Format_RGB32);
}

QBoyClassic::~QBoyClassic() {
	delete timer;
	delete qboyt;
	delete ui;
}


void QBoyClassic::on_pushButton_clicked() {
	qboyt->stop();
}
