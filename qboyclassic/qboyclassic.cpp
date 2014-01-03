#include "qboyclassic.h"
#include "ui_qboyclassic.h"

#include <QMessageBox>
#include <QFileDialog>

QBoyClassic::QBoyClassic(QWidget *parent) :
    QMainWindow(parent),
	ui(new Ui::QBoyClassic) {

	ui->setupUi(this);

	filename = "";
	qboyt = 0;
}

QBoyClassic::~QBoyClassic() {
	delete qboyt;
	delete ui;
}

void QBoyClassic::startGameBoy() {
	stopGameBoy();

	QFile gbfile(filename);
	if (!gbfile.exists()) {
		QMessageBox::critical(this, "File not found", "The gameboy file could not be found.");
	} else {
		qboyt = new qboythread(filename);
		qboyt->start();
		ui->gblabel->image = new QImage(qboyt->getLCD(), 160, 144, QImage::Format_RGB32);
		ui->btnpause->setText("Pause");
		connect(qboyt, SIGNAL(screen_refresh()), ui->gblabel, SLOT(repaint()));
	}
}

void QBoyClassic::stopGameBoy() {
	QImage *labelimage = ui->gblabel->image;
	ui->gblabel->image = 0;
	if (labelimage != 0) delete labelimage;
	if (qboyt != 0) delete qboyt;
	qboyt = 0;
}

void QBoyClassic::keyPressEvent(QKeyEvent* event) {
	if (qboyt == 0) return;
	GBKeypadKey key = qtkeytogb(event->key());

	if (key != GBKeypadKey_NONE) {
		qboyt->keydown(key);
		event->accept();
	}
}

void QBoyClassic::keyReleaseEvent(QKeyEvent *event) {
	if (qboyt == 0) return;
	GBKeypadKey key = qtkeytogb(event->key());

	if (key != GBKeypadKey_NONE) {
		qboyt->keyup(key);
		event->accept();
	}
}

GBKeypadKey QBoyClassic::qtkeytogb(int qtkey) {
	GBKeypadKey key = GBKeypadKey_NONE;
	switch (qtkey) {
	case Qt::Key_Return:
		key = GBKeypadKey_START; break;
	case Qt::Key_Space:
		key = GBKeypadKey_SELECT; break;
	case Qt::Key_Left:
		key = GBKeypadKey_LEFT; break;
	case Qt::Key_Right:
		key = GBKeypadKey_RIGHT; break;
	case Qt::Key_Down:
		key = GBKeypadKey_DOWN; break;
	case Qt::Key_Up:
		key = GBKeypadKey_UP; break;
	case Qt::Key_X:
		key = GBKeypadKey_B; break;
	case Qt::Key_Z:
		key = GBKeypadKey_A; break;
	}
	return key;
}

void QBoyClassic::on_btnload_clicked() {
    filename = QFileDialog::getOpenFileName(this, "Open Gameboy file", QString(), "Gameboy (*.gb)");
	if (filename != "") {
		ui->btnrestart->setEnabled(true);
		startGameBoy();
	}
}

void QBoyClassic::on_btnpause_clicked() {
	if (qboyt == 0) return;
	if (qboyt->isRunning()) {
		qboyt->stop();
		ui->btnpause->setText("Continue");
	} else {
		qboyt->start();
		ui->btnpause->setText("Pause");
	}
}

void QBoyClassic::on_btnrestart_clicked() {
	startGameBoy();
}

void QBoyClassic::on_btnspeed_clicked() {
	if (qboyt != 0) qboyt->togglespeed();
}
