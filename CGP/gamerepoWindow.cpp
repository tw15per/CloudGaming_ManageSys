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

	ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//���ֻ��
	ui.tableView->setSortingEnabled(true);//����������
	ui.tableView->setAlternatingRowColors(true);//�����ɫ����
	ui.tableView->verticalHeader()->hide();//������ͷ
	ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//�����п��ڿ��ӽ�������Ӧ
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
    model->setHeaderData(0, Qt::Horizontal, tr("��ϷID"));
    model->setHeaderData(1, Qt::Horizontal, tr("��Ϸ��"));
	model->setHeaderData(2, Qt::Horizontal, tr("����"));
	model->setHeaderData(3, Qt::Horizontal, tr("������ID"));
	model->setHeaderData(4, Qt::Horizontal, tr("��������"));
	model->setHeaderData(5, Qt::Horizontal, tr("��������"));

	this->theselection = new QItemSelectionModel(model);
	ui.tableView->setModel(model);
	ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//����ѡ��ʱΪ����ѡ��
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
		QMessageBox::information(this, "δѡ����Ϸ", "���б���ѡ����Ϸ");
	else 
	{
		QMessageBox* msgbox = new QMessageBox(this);
		msgbox->setIcon(QMessageBox::Information);
		msgbox->setWindowTitle("����");
		msgbox->setText("����������Ϸ...");
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
		QMessageBox::information(this, "�޺�ģʽ����", "������Ϸ���ᱻ��¼");

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