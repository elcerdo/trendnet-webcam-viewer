#ifndef __SCRAPPER_H__
#define __SCRAPPER_H__

#include <QNetworkAccessManager>
#include <QWidget>
#include <QTimer>
#include "state.h"

class Scrapper : public QWidget
{
	Q_OBJECT
	public:
		Scrapper(QWidget* parent = NULL);
		void loadUrlFromFile(const QString& filename);
	public slots:
		void sendRequest();
		void readImage();
		void displayPixmap(QPixmap);
	protected:
		void paintEvent(QPaintEvent* event);
		typedef QList<QString> Urls;
		Urls urls;
		
		State* state;
		QTimer* timer;
		QNetworkAccessManager* manager;
		QPixmap current;
};

#endif

