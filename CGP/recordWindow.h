#pragma once

#include <QWidget>
#include "ui_recordWindow.h"
#include <QSqlTableModel>
#include <qitemselectionmodel.h>

class recordWindow : public QWidget
{
	Q_OBJECT

public:
	recordWindow(QWidget *parent = Q_NULLPTR);
	~recordWindow();
	void closeEvent(QCloseEvent*);

private:
	Ui::recordWindow ui;
	QSqlTableModel* model;
	QItemSelectionModel* theselection;

private slots:
	void on_searchButton_clicked();
	void on_deleteButton_clicked();
	void on_helpButton_clicked();
};
