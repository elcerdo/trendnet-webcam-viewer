#ifndef __WEBCAM_H__
#define __WEBCAM_H__

#include <QUrl>
#include <QNetworkAccessManager>
#include <QObject>
#include <QPixmap>

class Webcam : public QObject
{
	Q_OBJECT
	public:
		Webcam(QObject* parent, QNetworkAccessManager* manager, const QUrl& url);
		int getImageCount() const;
		QPixmap getLastImage() const;
		QString getStatus() const;
	signals:
		void gotImage(const QPixmap& image);
	protected slots:
		void appendChunk();
	protected:
		const QUrl url;
		QString indent;
		QNetworkReply* reply;

		enum WebcamType {INIT,GETTYPE,GETLENGTH,SKIPLINE,GETIMAGE,ERROR};
		QByteArray buffer;

		typedef QList<QPixmap> Images;
		int imageCount;
		Images images;

		bool updateWebcam();
		bool extractLine(QByteArray& line);
		WebcamType state;
		int imageLength;
};


#endif

