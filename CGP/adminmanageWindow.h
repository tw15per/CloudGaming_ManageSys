#pragma once

#include <QWidget>
#include "ui_adminmanageWindow.h"
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <qitemselectionmodel.h>

class adminmanageWindow : public QWidget
{
	Q_OBJECT

public:
	adminmanageWindow(QWidget *parent = Q_NULLPTR);
	~adminmanageWindow();
	void closeEvent(QCloseEvent*);

signals:
	void ExitWin();

private:
	Ui::adminmanageWindow ui;
	QSqlTableModel* model;
	QDataWidgetMapper* mapper;
	QItemSelectionModel* theselection;

private slots:
	void on_searchButton_clicked();
	void on_addButton_clicked();
	void on_deleteButton_clicked();
	void on_submitButton_clicked();
	void on_revertButton_clicked();
	void on_passwordlineEdit_textChanged();
	void on_tableView_currentRowChanged(const QModelIndex& current, const QModelIndex& previous);
	void on_helpButton_clicked();
};
