#pragma once

#include <QWidget>
#include "ui_userPanel.h"

class userPanel : public QWidget
{
	Q_OBJECT

public:
	userPanel(QWidget *parent = Q_NULLPTR);
	~userPanel();

private:
	Ui::userPanel ui;

private slots:
	void on_infoButton_clicked();
	void on_repoButton_clicked();
	void on_exitButton_clicked();
	void on_helpButton_clicked();
};
