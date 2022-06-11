#pragma once

#include <QWidget>
#include "ui_userinfoWindow.h"

class userinfoWindow : public QWidget
{
	Q_OBJECT

public:
	userinfoWindow(QWidget *parent = Q_NULLPTR);
	~userinfoWindow();
	void closeEvent(QCloseEvent*);

signals:
	void ExitWin();

private:
	Ui::userinfoWindow ui;

private slots:
	void on_passwordlineEdit_textEdited();
	void on_applyButton_clicked();
	void on_helpButton_clicked();
};
