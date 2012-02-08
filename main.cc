#include <QApplication>
#include "scrapper.h"

int main(int argc,char * argv[])
{
	qsrand(time(NULL));

	QApplication app(argc,argv);

	Scrapper scrapper;
	scrapper.loadUrlFromFile("trendnet_security_cams.txt");
	scrapper.sendRequest();
	scrapper.sendRequest();
	scrapper.sendRequest();
	scrapper.sendRequest();

	scrapper.show();

	return app.exec();
}

