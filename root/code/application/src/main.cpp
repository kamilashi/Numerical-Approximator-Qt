#include <QApplication>
#include "mainwindow.h"
#include "approximator.h"

int main(int argc, char* argv[])
{
	Approximator approximator;

	QApplication a(argc, argv);
	MainWindow w(&approximator);

	w.show();
	return a.exec();
}
