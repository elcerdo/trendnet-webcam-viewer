#include "scrapper.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QPainter>
#include <fstream>
#include <cmath>

Scrapper::Scrapper(QWidget* parent)
	: QWidget(parent)
{
	manager = new QNetworkAccessManager(this);
}

void Scrapper::loadUrlFromFile(const QString& filename)
{
	std::ifstream handle(qPrintable(filename));
	while (handle && !handle.eof())
	{
		std::string line;
		handle >> line;
		QUrl url(line.c_str());

		if (!url.isValid())
		{
			qDebug() << "invalid url" << url.toString();
			continue;
		}

		urls.push_back(url);
	}

	qDebug() << "found" << urls.size() << "urls";
}

void Scrapper::sendRequest()
{
	if (urls.empty()) {
		qDebug() << "empty urls";
		return;
	}

	int selection = qrand() % urls.size();
	Webcam* webcam = new Webcam(this,manager,urls[selection]);
	connect(webcam,SIGNAL(gotImage(const QPixmap&)),SLOT(displayImage(const QPixmap&)));
	urls.remove(selection);

	webcams.push_back(webcam);
}

void Scrapper::displayImage(const QPixmap& image)
{
	Webcam* webcam = dynamic_cast<Webcam*>(sender());
	Q_ASSERT(webcam);
	qDebug() << "got image" << image.size() << webcam->getImageCount();
	update();
}

void Scrapper::paintEvent(QPaintEvent* event)
{
	static const QRect base_rect(0,0,640,480);
	Q_UNUSED(event);
	qDebug() << "paint event";

	int mosaik_size = ceil(sqrt(webcams.size()));
	QPainter painter(this);
	painter.scale(1./mosaik_size,1./mosaik_size);
	Webcams::const_iterator iter = webcams.begin();
	for (int ii=0; ii<mosaik_size; ii++)
	{
		for (int jj=0; jj<mosaik_size; jj++)
		{
			if (iter == webcams.end()) return;
			Webcam* current = *iter;
			iter++;

			painter.save();
			painter.translate(jj*base_rect.width(),ii*base_rect.height());
			painter.drawPixmap(base_rect,current->getLastImage());
			painter.drawText(0,0,current->getStatus());
			painter.restore();
		}
	}
}
