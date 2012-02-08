#include <QApplication>
#include "scrapper.h"

int main(int argc,char * argv[])
{
	QApplication app(argc,argv);

	Scrapper scrapper;
	scrapper.loadUrlFromFile("trend.urls");

	scrapper.show();

	return app.exec();
}

