#pragma once

#include <QWidget>
#include "ui_pubmanageWindow.h"
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <qitemselectionmodel.h>

class pubmanageWindow : public QWidget
{
	Q_OBJECT

public:
	pubmanageWindow(QWidget *parent = Q_NULLPTR);
	~pubmanageWindow();
	void closeEvent(QCloseEvent*);

signals:
	void ExitWin();

private:
	Ui::pubmanageWindow ui;
	QSqlTableModel* model;
	QDataWidgetMapper* mapper;
	QItemSelectionModel* theselection;

private slots:
	void on_searchButton_clicked();
	void on_addButton_clicked();
	void on_deleteButton_clicked();
	void on_submitButton_clicked();
	void on_revertButton_clicked();
	void on_pubnamelineEdit_textChanged();
	void on_tableView_currentRowChanged(const QModelIndex& current, const QModelIndex& previous);
	void on_helpButton_clicked();
};
