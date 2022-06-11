#pragma once

#include <QWidget>
#include "ui_saPanel.h"

class saPanel : public QWidget
{
	Q_OBJECT

public:
	saPanel(QWidget *parent = Q_NULLPTR);
	~saPanel();

private:
	Ui::saPanel ui;

private slots:
	void on_usermanageButton_clicked();
	void on_adminmanageButton_clicked();
	void on_exitButton_clicked();
	void on_helpButton_clicked();
};
