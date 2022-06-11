#include "saPanel.h"
#include "globle.h"
#include "help.h"
#include "adminloginWindow.h"
#include "usermanageWindow.h"
#include "adminmanageWindow.h"
#pragma execution_character_set("utf-8")

saPanel::saPanel(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.welcomelabel->setStyleSheet("color:#ff6600;");
	ui.welcomelabel->setText("»¶Ó­£¬" + curruser);
}

saPanel::~saPanel()
{
}

void saPanel::on_usermanageButton_clicked()
{
	usermanageWindow* um = new usermanageWindow();
	um->show();
	currwindow = 17;
	this->hide();
	connect(um, &usermanageWindow::ExitWin, this, &QWidget::show);
}

void saPanel::on_adminmanageButton_clicked()
{
	adminmanageWindow* am = new adminmanageWindow();
	am->show();
	currwindow = 2;
	this->hide();
	connect(am, &adminmanageWindow::ExitWin, this, &QWidget::show);
}

void saPanel::on_exitButton_clicked()
{
	adminloginWindow* al = new adminloginWindow();
	al->show();
	currwindow = 1;
	this->close();
}

void saPanel::on_helpButton_clicked()
{
	help* hp = new help();
	hp->show();
}