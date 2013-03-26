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
	qboyt = new qboythread("tetris.gb");
	qboyt->start();

	ui->label->image = new QImage(qboyt->getLCD(), 160, 144, QImage::Format_RGB32);
}

QBoyClassic::~QBoyClassic() {
	delete timer;
	delete qboyt;
	delete ui;
}

void QBoyClassic::keyPressEvent(QKeyEvent* event) {
	GBKeypadKey key = qtkeytogb(event->key());

	if (key != GBKeypadKey_NONE) {
		qboyt->keydown(key);
		event->accept();
	}
}

void QBoyClassic::keyReleaseEvent(QKeyEvent *event) {
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
