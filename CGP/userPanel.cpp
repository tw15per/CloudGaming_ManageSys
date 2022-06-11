#include "userPanel.h"
#include "globle.h"
#include "help.h"
#include "userloginWindow.h"
#include "userinfoWindow.h"
#include "gamerepoWindow.h"
#pragma execution_character_set("utf-8")

userPanel::userPanel(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.welcomelabel->setStyleSheet("color:#ff6600;");
	ui.welcomelabel->setText("»¶Ó­£¬" + curruser);
}

userPanel::~userPanel()
{
}

void userPanel::on_infoButton_clicked()
{
	userinfoWindow* ui = new userinfoWindow();
	ui->show();
	currwindow = 15;
	this->hide();
	connect(ui, &userinfoWindow::ExitWin, this, &QWidget::show);
}

void userPanel::on_repoButton_clicked()
{
	gamerepoWindow* gr = new gamerepoWindow();
	gr->show();
	currwindow = 4;
	this->hide();
	connect(gr, &gamerepoWindow::ExitWin, this, &QWidget::show);
}

void userPanel::on_exitButton_clicked()
{
	userloginWindow* ul = new userloginWindow();
	ul->show();
	currwindow = 16;
	this->close();
}

void userPanel::on_helpButton_clicked()
{
	help* hp = new help();
	hp->show();
}