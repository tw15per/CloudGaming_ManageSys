#include "adminPanel.h"
#include "globle.h"
#include "help.h"
#include "adminloginWindow.h"
#include "repomanageWindow.h"
#include "pubmanageWindow.h"
#include "servicerecordWindow.h"
#pragma execution_character_set("utf-8")

adminPanel::adminPanel(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.welcomelabel->setStyleSheet("color:#ff6600;");
	ui.welcomelabel->setText("»¶Ó­£¬" + curruser);
}

adminPanel::~adminPanel()
{
}

void adminPanel::on_pubButton_clicked()
{
	pubmanageWindow* pm = new pubmanageWindow();
	pm->show();
	currwindow = 9;
	this->hide();
	connect(pm, &pubmanageWindow::ExitWin, this, &QWidget::show);
}
void adminPanel::on_repoButton_clicked()
{
	repomanageWindow* rm = new repomanageWindow();
	rm->show();
	currwindow = 11;
	this->hide();
	connect(rm, &repomanageWindow::ExitWin, this, &QWidget::show);
}

void adminPanel::on_recordButton_clicked()
{
	servicerecordWindow* sr = new servicerecordWindow();
	sr->show();
	currwindow = 13;
	this->hide();
	connect(sr, &servicerecordWindow::ExitWin, this, &QWidget::show);
}

void adminPanel::on_exitButton_clicked()
{
	adminloginWindow* al = new adminloginWindow();
	al->show();
	currwindow = 1;
	this->close();
}

void adminPanel::on_helpButton_clicked()
{
	help* hp = new help();
	hp->show();
}
