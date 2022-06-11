#pragma once

#include <QWidget>
#include "ui_adminPanel.h"

class adminPanel : public QWidget
{
	Q_OBJECT

public:
	adminPanel(QWidget *parent = Q_NULLPTR);
	~adminPanel();

private:
	Ui::adminPanel ui;

private slots:
	void on_pubButton_clicked();
	void on_repoButton_clicked();
	void on_recordButton_clicked();
	void on_exitButton_clicked();
	void on_helpButton_clicked();
};
