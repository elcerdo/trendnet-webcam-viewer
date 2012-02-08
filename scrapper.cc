#include "scrapper.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QPainter>
#include <fstream>
#include <cmath>
#include <QMouseEvent>
#include "country.h"

static const QRect base_rect(0,0,640,480);
static const QRect status_rect(0,440,640,40);

Scrapper::Scrapper(QWidget* parent)
	: QWidget(parent)
{
	mosaik_size = -1;
	factor = 1;

	manager = new QNetworkAccessManager(this);

	setMinimumSize(640,480);
	resize(minimumSize());

	QTimer* timer = new QTimer(this);
	timer->setInterval(200);
	timer->setSingleShot(false);
	connect(timer,SIGNAL(timeout()),SLOT(update()));
	timer->start();
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

void Scrapper::populateWebcams(int number)
{
	for (int kk=0; kk<number; kk++)
	{
		appendRandomWebcam();
		if (webcams.size()>=number)
			return;
	}
}

void Scrapper::appendRandomWebcam()
{
	Country country;

	if (urls.empty()) {
		qDebug() << "empty urls";
		return;
	}

	int selection = qrand() % urls.size();
	Webcam* webcam = new Webcam(this,manager,urls[selection],country.getCountry(urls[selection]));
	urls.remove(selection);

	webcams.push_back(webcam);
}

void Scrapper::mousePressEvent(QMouseEvent* event)
{
	if (event->button() != Qt::LeftButton) return;
	if (mosaik_size<0) return;
	
	int ii = floor(event->y()*mosaik_size/(factor*base_rect.height()));
	if (ii<0) return;
	if (ii>=mosaik_size) return;

	int jj = floor(event->x()*mosaik_size/(factor*base_rect.width()));
	if (jj<0) return;
	if (jj>=mosaik_size) return;

	int index = ii*mosaik_size+jj;
	if (index>=webcams.size()) return;

	{ // remove webcam
		Webcam* webcam = webcams[index];
		qDebug() << webcams[index]->getUrl().toString() << urls.size();
		webcams.removeAll(webcam);
		delete webcam;
	}
	
	{ // add a new one
		Country country;
		if (urls.empty()) return;
		int selection = qrand() % urls.size();
		Webcam* webcam = new Webcam(this,manager,urls[selection],country.getCountry(urls[selection]));
		urls.remove(selection);
		webcams.insert(index,webcam);
	}

}

void Scrapper::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);

	mosaik_size = ceil(sqrt(webcams.size()));
	factor = qMin(static_cast<float>(width())/base_rect.width(),static_cast<float>(height())/base_rect.height());

	QFont font;
	font.setBold(true);
	font.setPointSize(30);

	QPainter painter(this);
	painter.setFont(font);
	painter.scale(factor/mosaik_size,factor/mosaik_size);
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
			painter.setPen(Qt::red);
			if (current->isActive()) painter.setPen(Qt::green);
			painter.drawText(status_rect,current->getStatus());
			painter.restore();
		}
	}
}
