#ifndef __SCRAPPER_H__
#define __SCRAPPER_H__

#include <QWidget>


class Scrapper : public QWidget
{
	Q_OBJECT
	public:
		Scrapper(QWidget* parent = NULL);
		void loadUrlFromFile(const QString& filename);
};

#endif

