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
	model->setHeaderData(0, Qt::Horizontal, tr("����ID"));
	model->setHeaderData(1, Qt::Horizontal, tr("��Ϸʱ��"));
	model->setHeaderData(2, Qt::Horizontal, tr("�û�ID"));
	model->setHeaderData(3, Qt::Horizontal, tr("��ϷID"));
	model->setHeaderData(4, Qt::Horizontal, tr("��Ϸ��"));

	this->theselection = new QItemSelectionModel(model);
	ui.tableView->setModel(model);
	ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//����ѡ��ʱΪ����ѡ��
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
	int result = QMessageBox::warning(this, "����", "�⽫ɾ������Ϸ��¼��ȷ��������", QMessageBox::Yes | QMessageBox::No);
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
			QMessageBox::critical(this, "����", "ɾ��ʧ�ܣ���������ǰ���ں�����");
		}
		else
			QMessageBox::information(this, "�ɹ�", "����Ϸ��¼�ѳɹ�ɾ��");

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