#include "servicerecordWindow.h"
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
#pragma execution_character_set("utf-8")

servicerecordWindow::servicerecordWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//���ֻ��
	ui.tableView->setSortingEnabled(true);//����������
	ui.tableView->setAlternatingRowColors(true);//�����ɫ����
	ui.tableView->verticalHeader()->hide();//������ͷ
	ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//�п��������Զ�����
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
	model->setEditStrategy(QSqlTableModel::OnManualSubmit);
	model->setTable("serv");
	model->setSort(1, Qt::DescendingOrder);
	model->select();
	model->setHeaderData(0, Qt::Horizontal, tr("����ID"));
	model->setHeaderData(1, Qt::Horizontal, tr("����ʱ��"));
	model->setHeaderData(2, Qt::Horizontal, tr("�û�ID"));
	model->setHeaderData(3, Qt::Horizontal, tr("��ϷID"));
	model->setHeaderData(4, Qt::Horizontal, tr("��Ϸ��"));

	this->theselection = new QItemSelectionModel(model);
	ui.tableView->setModel(model);
	ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//����ѡ��ʱΪ����ѡ��
	ui.tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.tableView->setSelectionModel(theselection);
	ui.tableView->hideColumn(2);
	ui.tableView->hideColumn(3);
	ui.tableView->hideColumn(4);

	connect(theselection, &QItemSelectionModel::currentRowChanged, this, &servicerecordWindow::on_tableView_currentRowChanged);
}

servicerecordWindow::~servicerecordWindow()
{
}

void servicerecordWindow::closeEvent(QCloseEvent*)
{
	emit ExitWin();
	currwindow = 3;
}

void servicerecordWindow::on_searchButton_clicked()
{
	QString search = ui.searchlineEdit->text();
	model->setFilter("sid like '%" + search + "%'");
	model->select();

	ui.tableView->setFocus();
	theselection->clearSelection();
	theselection->setCurrentIndex(model->index(0, 0), QItemSelectionModel::Select);
}

void servicerecordWindow::on_tableView_currentRowChanged(const QModelIndex& current, const QModelIndex& previous)
{
	qDebug() << "rowChanged";		
	int currow = current.row();
	int colucount = model->columnCount();
	QString data[5];
	for (int i = 0; i < colucount; i++)
		data[i] = model->data(model->index(currow, i)).toString();

	ui.plainTextEdit->setPlainText("����ID��" + data[0]
			+ "\n����ʱ�䣺" + data[1]
			+ "\n�û�ID��" + data[2]
			+ "\n��ϷID��" + data[3]
			+ "\n��Ϸ����" + data[4]);
}

void servicerecordWindow::on_helpButton_clicked()
{
	help* hp = new help();
	hp->show();
}