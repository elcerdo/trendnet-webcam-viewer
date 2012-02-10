#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include "ui_mainwindow.h"
#include <QMainWindow>

class MainWindow : public QMainWindow, protected Ui::MainWindow
{
	Q_OBJECT
	public:
		MainWindow(QWidget* parent = NULL);
};

#endif

