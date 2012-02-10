#ifndef __SCRAPPER_H__
#define __SCRAPPER_H__

#include <QNetworkAccessManager>
#include <QWidget>
#include <QTimer>
#include "webcam.h"

class Scrapper : public QWidget
{
	Q_OBJECT
	public:
		Scrapper(QWidget* parent = NULL);
		void loadUrlFromFile(const QString& filename);
		void populateWebcams(int number);
	protected:
		void appendRandomWebcam();
		void paintEvent(QPaintEvent* event);
		void mousePressEvent(QMouseEvent* event);
		int mosaik_size;
		float factor;

		typedef QVector<QUrl> Urls;
		Urls urls;

		typedef QVector<Webcam*> Webcams;
		Webcams webcams;
		int selected_webcam;

		QNetworkAccessManager* manager;
};

#endif

