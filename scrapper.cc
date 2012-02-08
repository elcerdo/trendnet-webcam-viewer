#include "scrapper.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <fstream>

Scrapper::Scrapper(QWidget* parent)
	: QWidget(parent)
{
	timer = new QTimer(this);
	timer->setInterval(1000);
	timer->setSingleShot(false);
	connect(timer,SIGNAL(timeout()),SLOT(sendRequest()));

	manager = new QNetworkAccessManager(this);

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
	buffer.append(reply->readAll());
	//qDebug() << "buffer" << buffer.size();

	while (true)
	{
		int pos = buffer.indexOf("\r\n");
		if (pos<0) break;
		qDebug() << "found chunk" << pos;

		QByteArray chunk = buffer.left(pos);
		buffer.remove(0,pos+2);

		if (chunk.size() < 50)
			qDebug() << chunk;
		
		//qDebug() << chunk;
	}
}

//void Scrapper::gotReply(QNetworkReply* reply)
//{
//	qDebug() << "got reply" << reply->url().toString();
//}
