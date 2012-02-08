#include "webcam.h"

#include <QDebug>
#include <QNetworkReply>
#include <QNetworkRequest>

Webcam::Webcam(QObject* parent, QNetworkAccessManager* manager, const QUrl& url)
	: QObject(parent), url(url)
{
	newImageSinceLastCall = false;
	indent = "["+url.host()+"]";
	buffer.clear();

	imageCount = 0;
	images.clear();

	state = INIT;
	imageLength = -1;

	QNetworkRequest request(url);
	reply = manager->get(request);
	reply->setParent(this);
	connect(reply,SIGNAL(readyRead()),SLOT(appendChunk()));

	qDebug() << qPrintable(indent) << "sending request";
}

void Webcam::appendChunk()
{
	QNetworkReply* reply_casted = dynamic_cast<QNetworkReply*>(sender());
	if (!reply || !reply_casted || reply_casted!=reply)
	{
		qWarning() << qPrintable(indent) << "got wrong reply";
		return;
	}

	buffer.append(reply->readAll());
	
	WebcamType old_state = state;
	while (updateWebcam())
	{
		old_state = state;
	}
}

bool Webcam::updateWebcam()
{
	if (state == INIT) 
	{
		QByteArray line;
		if (!extractLine(line)) return false;

		Q_ASSERT(line.contains("--boundary"));
		state = GETTYPE;
		return true;
	}

	if (state == GETTYPE) 
	{
		QByteArray line;
		if (!extractLine(line)) return false;

		Q_ASSERT(line.contains("Content-Type: image/jpeg"));
		state = GETLENGTH;
		return true;
	}

	if (state == GETLENGTH) 
	{
		QByteArray line;
		if (!extractLine(line)) return false;

		Q_ASSERT(line.contains("Content-Length: "));
		line.remove(0,16);
		bool ok = false;
		imageLength = line.toInt(&ok);
		Q_ASSERT(ok);
		state = SKIPLINE;
		return true;
	}

	if (state == SKIPLINE) 
	{
		QByteArray line;
		if (!extractLine(line)) return false;

		Q_ASSERT(line.empty());
		state = GETIMAGE;
		return true;
	}

	if (state == GETIMAGE) 
	{
		if (buffer.size() < imageLength) return false;

		QByteArray image_data = buffer.left(imageLength);
		buffer.remove(0,imageLength);

		Q_ASSERT(image_data.size() == imageLength);

		QPixmap image;
		image.loadFromData(image_data,"JPEG");

		images.push_back(image);
		while (images.size() > 20)
			images.pop_front();

		imageCount++;

		newImageSinceLastCall = true;
		//qDebug() << qPrintable(indent) << "got new image image_count =" << imageCount;
		//emit gotImage(image);

		state = INIT;
		return true;
	}

	Q_ASSERT(false);

	return false;
}

int Webcam::getImageCount() const
{
	return imageCount;
}

QString Webcam::getStatus() const
{
	return QString("%1 %2").arg(indent).arg(imageCount);
}

QPixmap Webcam::getLastImage() const
{
	if (images.empty()) return QPixmap();
	return images.back();
}

bool Webcam::extractLine(QByteArray& line)
{
	int pos = buffer.indexOf("\r\n");
	if (pos<0) return false;

	line = buffer.left(pos);
	buffer.remove(0,pos+2);
	return true;
}

QUrl Webcam::getUrl() const
{
	return url;
}

bool Webcam::isActive()
{
	bool local = newImageSinceLastCall;
	newImageSinceLastCall = false;
	return local;
}
