#ifndef __SCRAPPER_H__
#define __SCRAPPER_H__

#include <QNetworkAccessManager>
#include <QWidget>
#include <QTimer>

class Scrapper : public QWidget
{
	Q_OBJECT
	public:
		Scrapper(QWidget* parent = NULL);
		void loadUrlFromFile(const QString& filename);
	public slots:
		void sendRequest();
		void readImage();
	protected:
		typedef QList<QString> Urls;
		Urls urls;
		
		QByteArray buffer;

		QTimer* timer;
		QNetworkAccessManager* manager;
};

#endif

