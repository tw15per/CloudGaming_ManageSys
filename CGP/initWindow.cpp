#include "initWindow.h"
#include "globle.h"
#include "help.h"
#include "userloginWindow.h"
#include "adminloginWindow.h"
#pragma execution_character_set("utf-8")

initWindow::initWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

initWindow::~initWindow()
{
}

void initWindow::on_userButton_clicked()
{
	userloginWindow* ul=new userloginWindow();
	ul->show();
	currwindow = 16;
	this->close();
}

void initWindow::on_adminButton_clicked()
{
	adminloginWindow* al = new adminloginWindow();
	al->show();
	currwindow = 1;
	this->close();
}

void initWindow::on_helpButton_clicked()
{
	help* hp = new help();
	hp->show();
}