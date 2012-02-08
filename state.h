#ifndef __STATE_H__
#define __STATE_H__

#include <QObject>

class State : public QObject
{
	Q_OBJECT
	public:
		State(QObject* parent);
		void append(QByteArray chunk);
	protected:
		enum StateType {INIT,GETTYPE,GETLENGTH,SKIPLINE,GETIMAGE,ERROR};
		QByteArray buffer;

		bool updateState();
		bool extractLine(QByteArray& line);
		StateType state;
		int imageLength;
};


#endif

