#ifndef __STATE_H__
#define __STATE_H__

#include <QObject>
#include <QPixmap>

class State : public QObject
{
	Q_OBJECT
	public:
		State(QObject* parent);
		void append(QByteArray chunk);
	signals:
		void gotImage(const QPixmap& image);
	protected:
		enum StateType {INIT,GETTYPE,GETLENGTH,SKIPLINE,GETIMAGE,ERROR};
		QByteArray buffer;

		int imageCount;
		typedef QList<QPixmap> Images;
		Images images;

		bool updateState();
		bool extractLine(QByteArray& line);
		StateType state;
		int imageLength;
};


#endif

