#pragma once

#include <QWidget>
#include "ui_adminloginWindow.h"

class adminloginWindow : public QWidget
{
	Q_OBJECT

public:
	adminloginWindow(QWidget *parent = Q_NULLPTR);
	~adminloginWindow();

private:
	Ui::adminloginWindow ui;

private slots:
	void on_loginButton_clicked();
	void on_helpButton_clicked();
};
