#include "mainwindow.h"

#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	labelUrlsCount = new QLabel(this);
	labelUrlsCount->setObjectName("labelUrlsCount");

	setupUi(this);

	statusbar->addPermanentWidget(labelUrlsCount);
}

void MainWindow::on_actionOpenUrls_activated()
{
	QString filename = QFileDialog::getOpenFileName(this, "Open urls list", "", "Urls list (*.txt *.list)");
	if (!QFile(filename).exists())
		return;

	qDebug() << "opening" << filename;
	scrapper->loadUrlsList(filename);
	scrapper->populateWebcams(16);
}

void MainWindow::on_scrapper_urlsCountChanged(int count)
{
	labelUrlsCount->setText(QString("%1 urls").arg(count));
}
