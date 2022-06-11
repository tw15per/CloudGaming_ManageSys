#include "gamerepoWindow.h"
#include "globle.h"
#include "help.h"
#include "userPanel.h"
#include "recordWindow.h"
#include <QtSql/qsqldatabase.h>
#include <QtSql/QSqlQuery>
#include <QtDebug>
#include <QtWidgets/qmessagebox.h>
#include <QSqlTableModel>
#include <qitemselectionmodel.h>
#include <qsqlerror.h>
#include <qdatetime.h>
#include <qtimer.h>
#pragma execution_character_set("utf-8")

gamerepoWindow::gamerepoWindow(QWidget *parent)
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
	if (!db.open())
		qDebug() << "Failed to connect to root mysql admin";
	else
		qDebug() << "open";

	this->model = new QSqlTableModel(this, db);
	model->setTable("game");
	model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("游戏ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("游戏名"));
	model->setHeaderData(2, Qt::Horizontal, tr("类型"));
	model->setHeaderData(3, Qt::Horizontal, tr("发行商ID"));
	model->setHeaderData(4, Qt::Horizontal, tr("发行商名"));
	model->setHeaderData(5, Qt::Horizontal, tr("发行日期"));

	this->theselection = new QItemSelectionModel(model);
	ui.tableView->setModel(model);
	ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中时为整行选中
	ui.tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.tableView->setSelectionModel(theselection);

	ui.tableView->hideColumn(0);
	ui.tableView->hideColumn(3);
}

gamerepoWindow::~gamerepoWindow()
{
}

void gamerepoWindow::closeEvent(QCloseEvent*)
{
	emit ExitWin();
	currwindow = 18;
}

void gamerepoWindow::on_searchButton_clicked()
{
	QString search = ui.searchlineEdit->text();
	model->setFilter("gname like '%" + search + "%' or pname like '%" + search + "%'");
	model->select();

	ui.tableView->setFocus();
	theselection->clearSelection();
	theselection->setCurrentIndex(model->index(0, 0), QItemSelectionModel::Select);
}

void gamerepoWindow::on_playButton_clicked()
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

	QModelIndex curindex= theselection->currentIndex();
	QModelIndex gidindex = model->index(curindex.row(), 0);
	QString gameid = model->data(gidindex).toString();
	QModelIndex gnameindex = model->index(curindex.row(), 1);
	QString gamename = model->data(gnameindex).toString();
	qDebug() << gameid << gamename;

	if (gameid .isEmpty())
		QMessageBox::information(this, "未选择游戏", "在列表中选择游戏");
	else 
	{
		QMessageBox* msgbox = new QMessageBox(this);
		msgbox->setIcon(QMessageBox::Information);
		msgbox->setWindowTitle("启动");
		msgbox->setText("正在启动游戏...");
		msgbox->setStandardButtons(QMessageBox::NoButton);
		QTimer::singleShot(1000, msgbox, &QMessageBox::accept);
		msgbox->exec();
	}

	ui.tableView->setFocus();

	if (!ui.checkBox->isChecked())
	{
		QDateTime current_date_time = QDateTime::currentDateTime();
		QString current_date = current_date_time.toString("yyyyMMddhhmmss");
		QString current_date_mysql = current_date_time.toString("yyyy-MM-dd-hh-mm-ss");
		qDebug() << current_date << current_date_mysql;

		QSqlQuery query(db);
		db.exec("SET NAMES 'utf8'");
		query.exec("insert into serv(sid, stime, uid, gid, gname) values(?, ?, ?, ?, ?);");
		QVariantList sidList;
		sidList << current_date + curruser;
		QVariantList stimeList;
		stimeList << current_date_mysql;
		QVariantList uidList;
		uidList << curruser;
		QVariantList gidList;
		gidList << gameid;
		QVariantList gnameList;
		gnameList << gamename;

		query.addBindValue(sidList);
		query.addBindValue(stimeList);
		query.addBindValue(uidList);
		query.addBindValue(gidList);
		query.addBindValue(gnameList);
		query.execBatch();
		if (query.lastError().isValid())
			qDebug() << query.lastError();
	}
	else
		QMessageBox::information(this, "无痕模式开启", "本次游戏不会被记录");

	db.close();
}

void gamerepoWindow::on_recordButton_clicked()
{
	recordWindow* rw = new recordWindow();
	rw->show();
	currwindow = 10;
}

void gamerepoWindow::on_helpButton_clicked()
{
	help* hp = new help();
	hp->show();
}