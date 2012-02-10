#include <QApplication>
#include "mainwindow.h"
#include "scrapper.h"

int main(int argc,char * argv[])
{
	qsrand(time(NULL));

	QApplication app(argc,argv);

	MainWindow widget;
	widget.show();

	return app.exec();
}

