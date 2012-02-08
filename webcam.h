#ifndef __WEBCAM_H__
#define __WEBCAM_H__

#include <QObject>
#include <QPixmap>

class Webcam : public QObject
{
	Q_OBJECT
	public:
		Webcam(QObject* parent);
		void append(QByteArray chunk);
	signals:
		void gotImage(const QPixmap& image);
	protected:
		enum WebcamType {INIT,GETTYPE,GETLENGTH,SKIPLINE,GETIMAGE,ERROR};
		QByteArray buffer;

		int imageCount;
		typedef QList<QPixmap> Images;
		Images images;

		bool updateWebcam();
		bool extractLine(QByteArray& line);
		WebcamType state;
		int imageLength;
};


#endif

