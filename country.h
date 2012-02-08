#ifndef __COUNTRY_H__
#define __COUNTRY_H__

#include <QObject>
#include <QUrl>

class Country : public QObject
{
	Q_OBJECT
	public:
		Country(QObject* parent = NULL);
		QString getCountry(const QUrl& url);
};

#endif
