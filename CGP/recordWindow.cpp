#include "recordWindow.h"
#include "globle.h"
#include "help.h"
#include <QtSql/qsqldatabase.h>
#include <QtSql/QSqlQuery>
#include <QtDebug>
#include <QtWidgets/qmessagebox.h>
#include <QSqlTableModel>
#include <qitemselectionmodel.h>
#include <qsqlerror.h>
#pragma execution_character_set("utf-8")

recordWindow::recordWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格只读
	ui.tableView->setSortingEnabled(true);//启用排序功能
	ui.tableView->setAlternatingRowColors(true);//表格颜色交错
	ui.tableView->verticalHeader()->hide();//隐藏行头
	ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//设置列宽在可视界面自适应
	ui.tableView->setFocus();

	connect(ui.returnButton, &QPushButton::clicked, this, &QWidget::close);

	QSqlDatabase db;
	if (QSqlDatabase::contains("qt_sql_default_connection"))
		db = QSqlDatabase::database("qt_sql_default_connection");
	else
		db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("localhost");
	db.setDatabaseName("cgp");
	db.setUserName(sqluser);
	db.setPassword(sqlpass);
	db.setConnectOptions("MYSQL_OPT_RECONNECT=1");
	if (!db.open())
		qDebug() << "Failed to connect to root mysql admin";
	else
		qDebug() << "open";

	this->model = new QSqlTableModel(this, db);
	model->setTable("serv");
	model->setEditStrategy(QSqlTableModel::OnManualSubmit);
	model->setFilter("uid = '" + curruser + "'");
	model->setSort(0, Qt::DescendingOrder);
	model->select();
	model->setHeaderData(0, Qt::Horizontal, tr("服务ID"));
	model->setHeaderData(1, Qt::Horizontal, tr("游戏时间"));
	model->setHeaderData(2, Qt::Horizontal, tr("用户ID"));
	model->setHeaderData(3, Qt::Horizontal, tr("游戏ID"));
	model->setHeaderData(4, Qt::Horizontal, tr("游戏名"));

	this->theselection = new QItemSelectionModel(model);
	ui.tableView->setModel(model);
	ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中时为整行选中
	ui.tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.tableView->setSelectionModel(theselection);

	ui.tableView->hideColumn(0);
	ui.tableView->hideColumn(2);
	ui.tableView->hideColumn(3);
}

recordWindow::~recordWindow()
{
}

void recordWindow::closeEvent(QCloseEvent*)
{
	currwindow = 4;
}

void recordWindow::on_searchButton_clicked()
{
	QString search = ui.searchlineEdit->text();
	model->setFilter("uid = '" + curruser + "'and gname like '%" + search + "%'");
	model->select();

	ui.tableView->setFocus();
	theselection->clearSelection();
	theselection->setCurrentIndex(model->index(0, 0), QItemSelectionModel::Select);
}

void recordWindow::on_deleteButton_clicked()
{
	int result = QMessageBox::warning(this, "警告", "这将删除此游戏记录，确定继续吗？", QMessageBox::Yes | QMessageBox::No);
	switch (result)
	{
	case QMessageBox::Yes:
	{
		QModelIndex deleindex = theselection->currentIndex();
		model->removeRow(deleindex.row());
		model->submitAll();
		if (model->lastError().isValid())
		{
			qDebug() << model->lastError();
			QMessageBox::critical(this, "错误", "删除失败，请重启当前窗口后重试");
		}
		else
			QMessageBox::information(this, "成功", "此游戏记录已成功删除");

		model->select();

		ui.tableView->setFocus();
		theselection->clearSelection();
		theselection->setCurrentIndex(model->index(0, 0), QItemSelectionModel::Select);

		break;
	}
	case QMessageBox::No:
	{
		int delrow = ui.tableView->currentIndex().row();

		ui.tableView->setFocus();
		theselection->clearSelection();
		theselection->setCurrentIndex(model->index(delrow, 0), QItemSelectionModel::Select);

		break;
	}
	}
}

void recordWindow::on_helpButton_clicked()
{
	help* hp = new help();
	hp->show();
}