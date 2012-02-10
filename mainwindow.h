#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QLabel>

class MainWindow : public QMainWindow, protected Ui::MainWindow
{
	Q_OBJECT
	public:
		MainWindow(QWidget* parent = NULL);
	public slots:
		void on_actionOpenUrls_activated();
		void on_scrapper_urlsCountChanged(int);
	protected:
		QLabel* labelUrlsCount;
};

#endif

