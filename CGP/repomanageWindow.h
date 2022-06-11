#pragma once

#include <QWidget>
#include "ui_repomanageWindow.h"
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <qitemselectionmodel.h>

class repomanageWindow : public QWidget
{
	Q_OBJECT

public:
	repomanageWindow(QWidget *parent = Q_NULLPTR);
	~repomanageWindow();
	void closeEvent(QCloseEvent*);

signals:
	void ExitWin();

private:
	Ui::repomanageWindow ui;
	QSqlTableModel* model;
	QDataWidgetMapper* mapper;
	QItemSelectionModel* theselection;

private slots:
	void on_searchButton_clicked();
	void on_addButton_clicked();
	void on_deleteButton_clicked();
	void on_submitButton_clicked();
	void on_revertButton_clicked();
	void on_gamenamelineEdit_textChanged();
	void on_pubIDlineEdit_textChanged();
	void on_pubnamelineEdit_textChanged();
	void on_pubIDlineEdit_editingFinished();
	void on_tableView_currentRowChanged(const QModelIndex& current, const QModelIndex& previous);
	void on_helpButton_clicked();
};
