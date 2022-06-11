#pragma once

#include <QWidget>
#include "ui_usermanageWindow.h"
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <qitemselectionmodel.h>

class usermanageWindow : public QWidget
{
	Q_OBJECT

public:
	usermanageWindow(QWidget *parent = Q_NULLPTR);
	~usermanageWindow();
	void closeEvent(QCloseEvent*);

signals:
	void ExitWin();
	void clicked();

protected:
	bool eventFilter(QObject* obj, QEvent* e);

private:
	Ui::usermanageWindow ui;
	QSqlTableModel* model;
	QDataWidgetMapper* mapper;
	QItemSelectionModel* theselection;

private slots:
	void on_searchButton_clicked();
	void on_addButton_clicked();
	void on_deleteButton_clicked();
	void on_submitButton_clicked();
	void on_revertButton_clicked();
	void on_saveButton_clicked();
	void on_userIDlineEdit_textChanged();
	void on_passwordlineEdit_textChanged();
	void on_userIDlineEdit_editingFinished();
	void on_tableView_currentRowChanged(const QModelIndex& current, const QModelIndex& previous);
	void on_helpButton_clicked();
};
