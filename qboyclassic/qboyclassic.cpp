#include "qboyclassic.h"
#include "ui_qboyclassic.h"

#include <fstream>

QBoyClassic::QBoyClassic(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QBoyClassic)
{
	ui->setupUi(this);
	qboy = new libqboy;



	std::ifstream fin("cpu_instrs.gb");
	if (!fin.bad()) {
		qboy->loadgame(fin);
		fin.close();
	}
}

QBoyClassic::~QBoyClassic()
{
	delete ui;
}

void QBoyClassic::on_pushButton_clicked() {

	int c = 100;
	while (c--) {
		qboy->cycle();
	}
	QImage qboylcd(qboy->getLCD(), 160, 144, QImage::Format_RGB32);
	ui->label->setPixmap(QPixmap::fromImage(qboylcd));
}
