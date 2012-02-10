#include "scrapper.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QFile>
#include <cmath>
#include "country.h"

static const QRect base_rect(0,0,640,480);
static const QRect status_rect(0,435,640,45);

Scrapper::Scrapper(QWidget* parent)
	: QWidget(parent)
{
	selected_webcam = -1;
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

void Scrapper::loadUrlsList(const QString& filename)
{
	QFile handle(filename);
	if (!handle.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	int kk = 0;
	while (!handle.atEnd())
	{
		QUrl url(handle.readLine().replace("\n",""));

		if (!url.isValid())
		{
			qWarning() << "invalid url" << url.toString();
			continue;
		}

		urls.push_back(url);
		kk++;
	}

	qDebug() << "found" << kk << "urls (total" << urls.size() << "urls)";
	emit urlsCountChanged(urls.size());
}

void Scrapper::populateWebcams(int number)
{
	for (int kk=0; kk<number; kk++)
	{
		if (webcams.size()>=number)
			return;
		appendRandomWebcam();
	}
	emit urlsCountChanged(urls.size());
}

void Scrapper::appendRandomWebcam()
{
	Country country;

	if (urls.empty()) {
		qWarning() << "empty urls";
		return;
	}

	int selection = qrand() % urls.size();
	Webcam* webcam = new Webcam(this,manager,urls[selection],country.getCountry(urls[selection]));
	urls.remove(selection);

	selected_webcam = -1;
	mosaik_size = -1;
	factor = 1;
	webcams.push_back(webcam);
}

void Scrapper::mousePressEvent(QMouseEvent* event)
{
	if (mosaik_size<0) return;
	
	const int ii = floor(event->y()*mosaik_size/(factor*base_rect.height()));
	if (ii<0) return;
	if (ii>=mosaik_size) return;

	const int jj = floor(event->x()*mosaik_size/(factor*base_rect.width()));
	if (jj<0) return;
	if (jj>=mosaik_size) return;

	const int index = ii*mosaik_size+jj;
	if (index>=webcams.size()) return;

	if (event->button() == Qt::LeftButton && selected_webcam<0) { // left click rotate camera
		{ // remove webcam
			Webcam* webcam = webcams[index];
			webcams.remove(index);
			delete webcam;
		}

		qDebug() << "adding another cam" << urls.size() << "left";
		
		{ // add a new one
			Country country;
			if (urls.empty()) return;
			int selection = qrand() % urls.size();
			Webcam* webcam = new Webcam(this,manager,urls[selection],country.getCountry(urls[selection]));
			urls.remove(selection);
			webcams.insert(index,webcam);
		}

		emit urlsCountChanged(urls.size());
		update();
		return;
	}

	if (event->button() == Qt::RightButton) { // right click switch to full screen
		if (selected_webcam<0)
		{
			selected_webcam = index;
			qDebug() << "switching to fullscreen mode for" << webcams[selected_webcam]->getUrl().host();
		} else {
			qDebug() << "switching to mozaic";
			selected_webcam = -1;
		}
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

	if (selected_webcam<0) // mozaic mode
	{
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
	} else { //zoom mode
		Q_ASSERT(selected_webcam<webcams.size());

		Webcam* current = webcams[selected_webcam];

		painter.scale(factor,factor);
		painter.drawPixmap(base_rect,current->getLastImage());
		painter.setPen(Qt::red);
		if (current->isActive()) painter.setPen(Qt::green);
		painter.drawText(status_rect,current->getStatus());
	}

}
