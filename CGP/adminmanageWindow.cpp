#include "adminmanageWindow.h"
#include "globle.h"
#include "help.h"
#include "saPanel.h"
#include <QtSql/qsqldatabase.h>
#include <QtSql/QSqlQuery>
#include <QtDebug>
#include <QtWidgets/qmessagebox.h>
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <qitemselectionmodel.h>
#include <QInputDialog>
#include <qsqlerror.h>
#pragma execution_character_set("utf-8")

adminmanageWindow::adminmanageWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格只读
	ui.tableView->setSortingEnabled(true);//启用排序功能
	ui.tableView->setAlternatingRowColors(true);//表格颜色交错
	ui.tableView->verticalHeader()->hide();//隐藏行头
	ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//列宽随内容自动调整
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
	model->setEditStrategy(QSqlTableModel::OnManualSubmit);
	model->setTable("adm");
	model->select();
	model->setHeaderData(0, Qt::Horizontal, tr("管理员ID"));
	model->setHeaderData(1, Qt::Horizontal, tr("密码"));

	this->theselection = new QItemSelectionModel(model);
	ui.tableView->setModel(model);
	ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中时为整行选中
	ui.tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.tableView->setSelectionModel(theselection);

	connect(theselection, &QItemSelectionModel::currentRowChanged, this, &adminmanageWindow::on_tableView_currentRowChanged);

	this->mapper = new QDataWidgetMapper(this);
	mapper->setModel(model);
	mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
	mapper->addMapping(ui.adminIDlineEdit, 0);
	mapper->addMapping(ui.passwordlineEdit, 1);
	mapper->setCurrentIndex(0);
}

adminmanageWindow::~adminmanageWindow()
{
}

void adminmanageWindow::closeEvent(QCloseEvent*)
{
	emit ExitWin();
	currwindow = 12;
}

void adminmanageWindow::on_searchButton_clicked()
{
	QString search = ui.searchlineEdit->text();
	model->setFilter("aid like '%" + search + "%'");
	model->select();

	ui.tableView->setFocus();
	theselection->clearSelection();
	theselection->setCurrentIndex(model->index(0, 0), QItemSelectionModel::Select);

	mapper->setCurrentIndex(0);
}

void adminmanageWindow::on_addButton_clicked()
{
	bool ok;
	QString adminid = QInputDialog::getText(this, "新增管理员", "输入管理员ID：", QLineEdit::Normal, "", &ok);
	if (ok && !adminid.isEmpty())
	{
		qDebug() << "aid:" << adminid;
		int addrow = model->rowCount();
		bool T = true;
		for (int i = 0; i < addrow; i++)
		{
			QString pid = model->data(model->index(i, 0)).toString();
			if (adminid.compare(pid) == 0)
			{
				QMessageBox::critical(this, "错误", "管理员ID已被占用");
				T = false;
			}
		}
		if (T == true)
		{
			model->insertRow(addrow);
			model->setData(model->index(addrow, 0), adminid);
			model->setData(model->index(addrow, 1), adminid);

			ui.tableView->setFocus();
			theselection->clearSelection();
			theselection->setCurrentIndex(model->index(model->rowCount() - 1, 0), QItemSelectionModel::Select);

			mapper->setCurrentIndex(addrow);
		}
	}
}

void adminmanageWindow::on_passwordlineEdit_textChanged()
{
	QString password = ui.passwordlineEdit->text();
	if (password.isEmpty())
	{
		qDebug() << "password empty";
		ui.submitButton->setDisabled(true);
		ui.addButton->setDisabled(true);
		ui.deleteButton->setDisabled(true);
		ui.searchButton->setDisabled(true);
	}
	else
	{
		ui.submitButton->setEnabled(true);
		ui.addButton->setEnabled(true);
		ui.deleteButton->setEnabled(true);
		ui.searchButton->setEnabled(true);
	}
}

void adminmanageWindow::on_tableView_currentRowChanged(const QModelIndex& current, const QModelIndex& previous)
{
	QString password = ui.passwordlineEdit->text();
	if (password.isEmpty())
	{
		qDebug() << "password empty";
		QMessageBox::critical(this, "错误", "密码不能为空");
		int prevrow = previous.row();

		ui.tableView->setFocus();
		theselection->blockSignals(true);
		theselection->clearSelection();
		theselection->setCurrentIndex(model->index(prevrow, 0), QItemSelectionModel::Select);
		theselection->blockSignals(false);

		mapper->setCurrentIndex(prevrow);
	}
	else
	{
		int currow = current.row();
		mapper->setCurrentIndex(currow);
		qDebug() << "rowChanged";
	}
}

void adminmanageWindow::on_deleteButton_clicked()
{
	int result = QMessageBox::warning(this, "警告", "这将删除此管理员账户，确定继续吗？", QMessageBox::Yes | QMessageBox::No);
	switch (result)
	{
	case QMessageBox::Yes:
	{
		qDebug() << "Yes";

		QModelIndex deleindex = theselection->currentIndex();
		QString currid = model->data(model->index(deleindex.row(), 0)).toString();
		if (currid.compare("sa") == 0)
			QMessageBox::critical(this, "错误", "超级管理员账户不能被删除");
		else
		{
			model->removeRow(deleindex.row());
			model->submitAll();
			if (model->lastError().isValid())
			{
				qDebug() << model->lastError();
				QMessageBox::critical(this, "错误", "数据库连接错误，重启当前窗口可解决此问题");
			}
			else
				QMessageBox::information(this, "成功", "此管理员账户已成功删除");

			model->select();

			ui.tableView->setFocus();
			theselection->clearSelection();
			theselection->setCurrentIndex(model->index(0, 0), QItemSelectionModel::Select);

			mapper->setCurrentIndex(0);
		}

		break;
	}
	case QMessageBox::No:
	{
		qDebug() << "NO";

		int delrow = ui.tableView->currentIndex().row();

		ui.tableView->setFocus();
		theselection->clearSelection();
		theselection->setCurrentIndex(model->index(delrow, 0), QItemSelectionModel::Select);

		mapper->setCurrentIndex(delrow);

		break; 
	}
	}
}

void adminmanageWindow::on_submitButton_clicked()
{
	model->submitAll();
	if (model->lastError().isValid())
	{
		qDebug() << model->lastError();
		QMessageBox::critical(this, "错误", "数据库连接错误，重启当前窗口可解决此问题");
	}
	else
		QMessageBox::information(this, "成功", "已成功保存");

	model->select();

	ui.tableView->setFocus();
	theselection->clearSelection();
	theselection->setCurrentIndex(model->index(0, 0), QItemSelectionModel::Select);

	mapper->setCurrentIndex(0);
}

void adminmanageWindow::on_revertButton_clicked()
{
	model->revertAll();
	model->select();

	ui.tableView->setFocus();
	theselection->clearSelection();
	theselection->setCurrentIndex(model->index(0, 0), QItemSelectionModel::Select);

	mapper->setCurrentIndex(0);
}

void adminmanageWindow::on_helpButton_clicked()
{
	help* hp = new help();
	hp->show();
}