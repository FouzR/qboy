#include "qboyclassic.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QBoyClassic w;
	w.show();
	
	return a.exec();
}
