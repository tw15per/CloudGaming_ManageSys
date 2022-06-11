#pragma once

#include <QWidget>
#include "ui_initWindow.h"
#include "userloginWindow.h"
#include "adminloginWindow.h"

class initWindow : public QWidget
{
	Q_OBJECT

public:
	initWindow(QWidget *parent = Q_NULLPTR);
	~initWindow();

private:
	Ui::initWindow ui;

private slots:
	void on_userButton_clicked();
	void on_adminButton_clicked();
	void on_helpButton_clicked();
};
