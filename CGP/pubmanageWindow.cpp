#include "pubmanageWindow.h"
#include "globle.h"
#include "help.h"
#include "adminPanel.h"
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

pubmanageWindow::pubmanageWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格只读
	ui.tableView->setSortingEnabled(true);//启用排序功能
	ui.tableView->setAlternatingRowColors(true);//表格颜色交错
	ui.tableView->verticalHeader()->hide();//隐藏行头
	ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//列宽随内容自动调整
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
	model->setTable("pub");
	model->select();
	model->setHeaderData(0, Qt::Horizontal, tr("发行商ID"));
	model->setHeaderData(1, Qt::Horizontal, tr("发行商名"));
	model->setHeaderData(2, Qt::Horizontal, tr("发行商电话"));
	model->setHeaderData(3, Qt::Horizontal, tr("发行商电子邮箱"));
	model->setHeaderData(4, Qt::Horizontal, tr("发行商备注"));	

	this->theselection= new QItemSelectionModel(model);
	ui.tableView->setModel(model);
	ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中时为整行选中
	ui.tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.tableView->setSelectionModel(theselection);

	connect(theselection, &QItemSelectionModel::currentRowChanged, this, &pubmanageWindow::on_tableView_currentRowChanged);

	this->mapper = new QDataWidgetMapper(this);
	mapper->setModel(model);
	mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
	mapper->addMapping(ui.pubIDlineEdit, 0);
	mapper->addMapping(ui.pubnamelineEdit, 1);
	mapper->addMapping(ui.pubtelelineEdit, 2);
	mapper->addMapping(ui.pubemaillineEdit, 3);
	mapper->addMapping(ui.notetextEdit, 4);
	mapper->setCurrentIndex(0);
}

pubmanageWindow::~pubmanageWindow()
{
}

void pubmanageWindow::closeEvent(QCloseEvent*)
{
	emit ExitWin();
	currwindow = 3;
}

void pubmanageWindow::on_searchButton_clicked()
{
	QString search = ui.searchlineEdit->text();
	model->setFilter("pid like '%" + search + "%'");
	model->select();

	ui.tableView->setFocus();
	theselection->clearSelection();
	theselection->setCurrentIndex(model->index(0, 0), QItemSelectionModel::Select);

	mapper->setCurrentIndex(0);
}

void pubmanageWindow::on_addButton_clicked()
{
	bool ok;
	QString pubid = QInputDialog::getText(this, "新增发行商", "输入发行商ID：", QLineEdit::Normal, "", &ok);
	if (ok && !pubid.isEmpty())
	{
		qDebug() << "pid:" << pubid;
		int addrow = model->rowCount();
		bool T = true;
		for (int i = 0; i < addrow; i++)
		{
			QString pid = model->data(model->index(i, 0)).toString();
			if (pubid.compare(pid) == 0)
			{
				QMessageBox::critical(this, "错误", "发行商ID已被占用");
				T = false;
			}
		}
		if (T == true)
		{
			model->insertRow(addrow);
			model->setData(model->index(addrow, 0), pubid);
			model->setData(model->index(addrow, 1), pubid);

			ui.tableView->setFocus();
			theselection->clearSelection();
			theselection->setCurrentIndex(model->index(model->rowCount() - 1, 0), QItemSelectionModel::Select);

			mapper->setCurrentIndex(addrow);
		}
	}
}

void pubmanageWindow::on_pubnamelineEdit_textChanged()
{
	QString pubname = ui.pubnamelineEdit->text();
	if (pubname.isEmpty())
	{
		qDebug() << "pubname empty";
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

void pubmanageWindow::on_tableView_currentRowChanged(const QModelIndex& current, const QModelIndex& previous)
{
	QString pubname = ui.pubnamelineEdit->text();
	if (pubname.isEmpty())
	{
		qDebug() << "pubname empty";
		QMessageBox::critical(this, "错误", "关键数据不能为空");
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

void pubmanageWindow::on_deleteButton_clicked()
{
	int result = QMessageBox::warning(this, "警告", "这将删除此发行商、相关游戏和相关服务记录，确定继续吗？", QMessageBox::Yes | QMessageBox::No);
	switch (result)
	{
	case QMessageBox::Yes:
	{
		QSqlDatabase db;
		if (QSqlDatabase::contains("qt_sql_default_connection"))
			db = QSqlDatabase::database("qt_sql_default_connection");
		else
			db = QSqlDatabase::addDatabase("QMYSQL");
		db.setHostName("localhost");
		db.setDatabaseName("cgp");
		db.setUserName(sqluser);
		db.setPassword(sqlpass);
		if (!db.open())
			qDebug() << "Failed to connect to root mysql admin";
		else
			qDebug() << "open";

		QString pubid = ui.pubIDlineEdit->text();
		QSqlQuery query1(db);
		db.exec("SET NAMES 'utf8'");
		query1.exec("select gid from game where pid ='" + pubid + "';");
		qDebug() << "select gid from game where pid ='" + pubid + "';";
		while (query1.next())
		{
			QString gid = query1.value("gid").toString();
			QSqlQuery query2(db);
			db.exec("SET NAMES 'utf8'");
			query2.exec("delete from serv where gid='" + gid + "';");
			qDebug() << "delete from serv where gid='" + gid + "';";
			QSqlQuery query3(db);
			db.exec("SET NAMES 'utf8'");
			query3.exec("delete from game where gid='" + gid + "';");
			qDebug() << "delete from game where gid='" + gid + "';";
		}

		QModelIndex deleindex = theselection->currentIndex();
		model->removeRow(deleindex.row());
		model->submitAll();
		if (model->lastError().isValid())
		{
			qDebug() << model->lastError();
			QMessageBox::critical(this, "错误", "数据库连接错误，重启当前窗口可解决此问题");
		}
		else
			QMessageBox::information(this, "成功", "此发行商、相关游戏和相关服务记录已成功删除");

		model->select();

		ui.tableView->setFocus();
		theselection->clearSelection();
		theselection->setCurrentIndex(model->index(0, 0), QItemSelectionModel::Select);

		mapper->setCurrentIndex(0);

		break;
	}
	case QMessageBox::No:
	{
		int delrow = ui.tableView->currentIndex().row();

		ui.tableView->setFocus();
		theselection->clearSelection();
		theselection->setCurrentIndex(model->index(delrow, 0), QItemSelectionModel::Select);

		mapper->setCurrentIndex(delrow);

		break;
	}
	}
}

void pubmanageWindow::on_submitButton_clicked()
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

void pubmanageWindow::on_revertButton_clicked()
{
	model->revertAll();
	model->select();

	ui.tableView->setFocus();
	theselection->clearSelection();
	theselection->setCurrentIndex(model->index(0, 0), QItemSelectionModel::Select);

	mapper->setCurrentIndex(0);
}

void pubmanageWindow::on_helpButton_clicked()
{
	help* hp = new help();
	hp->show();
}