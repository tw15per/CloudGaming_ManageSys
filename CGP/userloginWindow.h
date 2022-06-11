#pragma once

#include <QWidget>
#include "ui_userloginWindow.h"

class userloginWindow : public QWidget
{
	Q_OBJECT

public:
	userloginWindow(QWidget *parent = Q_NULLPTR);
	~userloginWindow();

private:
	Ui::userloginWindow ui;

private slots:
	void on_loginButton_clicked();
	void on_registerButton_clicked();
	void on_helpButton_clicked();
};
