#pragma once

#include <QWidget>
#include "ui_signupWindow.h"

class signupWindow : public QWidget
{
	Q_OBJECT

public:
	signupWindow(QWidget *parent = Q_NULLPTR);
	~signupWindow();
	void closeEvent(QCloseEvent*);

signals:
	void ExitWin();

private:
	Ui::signupWindow ui;

private slots:
	void on_signupButton_clicked();
	void on_helpButton_clicked();
};
