#include "state.h"

#include <QDebug>

State::State(QObject* parent)
	: QObject(parent)
{
	state = INIT;
}

void State::append(QByteArray chunk)
{
	buffer.append(chunk);
	qDebug() << "buffer size" << buffer.size();
	
	StateType old_state = state;
	while (updateState())
	{
		qDebug() << old_state << "->" << state;
		old_state = state;
	}
}

bool State::updateState()
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
		if (buffer.size()<imageLength) return false;

		QByteArray image = buffer.left(imageLength);
		buffer.remove(0,imageLength);

		Q_ASSERT(image.size() == imageLength);

		QPixmap pixmap;
		pixmap.loadFromData(image,"JPEG");
		emit gotPixmap(pixmap);

		state = INIT;
		return true;
	}

	Q_ASSERT(false);

	return false;
}

bool State::extractLine(QByteArray& line)
{
	int pos = buffer.indexOf("\r\n");
	if (pos<0) return false;

	line = buffer.left(pos);
	buffer.remove(0,pos+2);
	return true;
}

