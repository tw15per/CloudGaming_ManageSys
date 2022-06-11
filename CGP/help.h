#pragma once

#include <QWidget>
#include "ui_help.h"

class help : public QWidget
{
	Q_OBJECT

public:
	help(QWidget *parent = Q_NULLPTR);
	~help();

private:
	Ui::help ui;
};
