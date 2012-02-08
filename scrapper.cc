#include "scrapper.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QPainter>
#include <fstream>

Scrapper::Scrapper(QWidget* parent)
	: QWidget(parent)
{
	timer = new QTimer(this);
	timer->setInterval(1000);
	timer->setSingleShot(false);
	connect(timer,SIGNAL(timeout()),SLOT(sendRequest()));

	manager = new QNetworkAccessManager(this);

	state = new State(this);
	connect(state,SIGNAL(gotImage(const QPixmap&)),SLOT(displayImage(const QPixmap&)));

	//sendRequest();
	//timer->start();
}

void Scrapper::loadUrlFromFile(const QString& filename)
{
	std::ifstream handle(qPrintable(filename));
	while (handle && !handle.eof())
	{
		std::string line;
		handle >> line;
		urls.push_back(QString(line.c_str()));
	}

	qDebug() << "found" << urls.size() << "urls";
}

void Scrapper::sendRequest()
{
	if (urls.empty()) {
		qDebug() << "empty urls";
		return;
	}

	QNetworkRequest request(QUrl(urls.back()));
	urls.pop_back();

	qDebug() << "requesting" << request.url().toString();
	QNetworkReply* reply = manager->get(request);
	connect(reply,SIGNAL(readyRead()),SLOT(readImage()));
}

void Scrapper::readImage()
{
	QNetworkReply* reply = dynamic_cast<QNetworkReply*>(sender());
	if (!reply)
	{
		qDebug() << "got wrong reply";
		return;
	}

	//qDebug() << "read from" << reply->url().toString();
	state->append(reply->readAll());
}

void Scrapper::displayImage(const QPixmap& image)
{
	qDebug() << "got image" << image.size();
	current = image;
	update();
}

//void Scrapper::gotReply(QNetworkReply* reply)
//{
//	qDebug() << "got reply" << reply->url().toString();
//}

void Scrapper::paintEvent(QPaintEvent* event)
{
	qDebug() << "paint event";
	QPainter painter(this);
	painter.drawPixmap(rect(),current);
}
