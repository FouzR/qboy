#include "qboyclassic.h"
#include "ui_qboyclassic.h"

#include <fstream>
#include <cassert>

QBoyClassic::QBoyClassic(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QBoyClassic)
{
	ui->setupUi(this);
	qboy = new libqboy;



	std::ifstream fin("cpu_instrs.gb");
	if (!fin.is_open()) {
		assert(false && "Could not open file");
	}
	qboy->loadgame(fin);
	fin.close();

}

QBoyClassic::~QBoyClassic()
{
	delete ui;
}

void QBoyClassic::on_pushButton_clicked() {

	int c = 10000000;
	ui->pushButton->setText("Busy");

	int x = 0;
	while (c--) {
		qboy->cycle();
		if (qboy->doupdate()) {
			QImage qboylcd(qboy->getLCD(), 160, 144, QImage::Format_RGB32);
			ui->label->setPixmap(QPixmap::fromImage(qboylcd));
			QApplication::processEvents();
		}
	}

	ui->pushButton->setText("Done");
}
