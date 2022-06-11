#pragma once

#include <QWidget>
#include "ui_gamerepoWindow.h"
#include <QSqlTableModel>
#include <qitemselectionmodel.h>

class gamerepoWindow : public QWidget
{
	Q_OBJECT

public:
	gamerepoWindow(QWidget *parent = Q_NULLPTR);
	~gamerepoWindow();
	void closeEvent(QCloseEvent*);

signals:
	void ExitWin();

private:
	Ui::gamerepoWindow ui;
	QSqlTableModel* model;
	QItemSelectionModel* theselection;

private slots:
	void on_searchButton_clicked();
	void on_playButton_clicked();
	void on_recordButton_clicked();
	void on_helpButton_clicked();
};
