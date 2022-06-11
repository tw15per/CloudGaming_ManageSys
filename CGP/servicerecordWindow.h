#pragma once

#include <QWidget>
#include "ui_servicerecordWindow.h"
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <qitemselectionmodel.h>

class servicerecordWindow : public QWidget
{
	Q_OBJECT

public:
	servicerecordWindow(QWidget *parent = Q_NULLPTR);
	~servicerecordWindow();
	void closeEvent(QCloseEvent*);

signals:
	void ExitWin();

private:
	Ui::servicerecordWindow ui;
	QSqlTableModel* model;
	QDataWidgetMapper* mapper;
	QItemSelectionModel* theselection;

private slots:
	void on_searchButton_clicked();
	void on_tableView_currentRowChanged(const QModelIndex& current, const QModelIndex& previous);
	void on_helpButton_clicked();
};
