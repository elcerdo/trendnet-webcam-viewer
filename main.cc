#include <QApplication>
#include "scrapper.h"

int main(int argc,char * argv[])
{
	qsrand(time(NULL));

	QApplication app(argc,argv);

	Scrapper scrapper;
	scrapper.loadUrlFromFile("trendnet_security_cams.txt");
	scrapper.populateWebcams(16);

	scrapper.show();

	return app.exec();
}

