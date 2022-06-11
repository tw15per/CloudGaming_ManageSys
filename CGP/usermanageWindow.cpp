#include "usermanageWindow.h"
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
#include <QMouseEvent>
#pragma execution_character_set("utf-8")

usermanageWindow::usermanageWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.saveButton->setVisible(0);
	ui.userIDlineEdit->installEventFilter(this);

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
	db.setConnectOptions("MYSQL_OPT_RECONNECT=1");
	if (!db.open())
		qDebug() << "Failed to connect to root mysql admin";
	else
		qDebug() << "open";

	this->model = new QSqlTableModel(this, db);
	model->setEditStrategy(QSqlTableModel::OnManualSubmit);
	model->setTable("sub");
	model->select();
	model->setHeaderData(0, Qt::Horizontal, tr("�û�ID"));
	model->setHeaderData(1, Qt::Horizontal, tr("����"));
	model->setHeaderData(2, Qt::Horizontal, tr("����"));
	model->setHeaderData(3, Qt::Horizontal, tr("�Ա�"));
	model->setHeaderData(4, Qt::Horizontal, tr("��������"));

	this->theselection = new QItemSelectionModel(model);
	ui.tableView->setModel(model);
	ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//����ѡ��ʱΪ����ѡ��
	ui.tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.tableView->setSelectionModel(theselection);

	connect(theselection, &QItemSelectionModel::currentRowChanged, this, &usermanageWindow::on_tableView_currentRowChanged);

	this->mapper = new QDataWidgetMapper(this);
	mapper->setModel(model);
	mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
	mapper->addMapping(ui.userIDlineEdit, 0);
	mapper->addMapping(ui.passwordlineEdit, 1);
	mapper->addMapping(ui.agelineEdit, 2);
	mapper->addMapping(ui.gendercomboBox, 3);
	mapper->addMapping(ui.useremaillineEdit, 4);
	mapper->setCurrentIndex(0);
}

usermanageWindow::~usermanageWindow()
{
}

bool usermanageWindow::eventFilter(QObject* obj, QEvent* e)
{
	if (obj == ui.userIDlineEdit)
	{
		if (e->type() == QEvent::MouseButtonPress)
		{
			QMouseEvent* me = (QMouseEvent*)e;
			if (me->button() == Qt::LeftButton)
			{
				qDebug() << "userIDlineEdit clicked";
				temp = ui.userIDlineEdit->text();
				qDebug() << temp;
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return usermanageWindow::eventFilter(obj, e);
}

void usermanageWindow::closeEvent(QCloseEvent*)
{
	emit ExitWin();
	currwindow = 12;
}

void usermanageWindow::on_searchButton_clicked()
{
	QString search = ui.searchlineEdit->text();
	model->setFilter("uid like '%" + search + "%'");
	model->select();

	ui.tableView->setFocus();
	theselection->clearSelection();
	theselection->setCurrentIndex(model->index(0, 0), QItemSelectionModel::Select);

	mapper->setCurrentIndex(0);
}

void usermanageWindow::on_addButton_clicked()
{
	bool ok;
	QString userid = QInputDialog::getText(this, "�����û�", "�����û�ID��", QLineEdit::Normal, "", &ok);
	if (ok && !userid.isEmpty())
	{
		qDebug() << "uid:" << userid;
		int addrow = model->rowCount();
		bool T = true;
		for (int i = 0; i < addrow; i++)
		{
			QString pid = model->data(model->index(i, 0)).toString();
			if (userid.compare(pid) == 0)
			{
				QMessageBox::critical(this, "����", "�û�ID�ѱ�ռ��");
				T = false;
			}
		}
		if (T == true)
		{
			model->insertRow(addrow);
			model->setData(model->index(addrow, 0), userid);
			model->setData(model->index(addrow, 1), userid);

			ui.tableView->setFocus();
			theselection->clearSelection();
			theselection->setCurrentIndex(model->index(model->rowCount() - 1, 0), QItemSelectionModel::Select);

			mapper->setCurrentIndex(addrow);
		}
	}
}

void usermanageWindow::on_userIDlineEdit_textChanged()
{
	QString userid = ui.userIDlineEdit->text();
	QString password = ui.passwordlineEdit->text();

	if (userid.isEmpty() || password.isEmpty())
	{
		qDebug() << "key data empty";
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

void usermanageWindow::on_passwordlineEdit_textChanged()
{
	QString userid = ui.userIDlineEdit->text();
	QString password = ui.passwordlineEdit->text();
	if (userid.isEmpty() || password.isEmpty())
	{
		qDebug() << "key data empty";
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

void usermanageWindow::on_userIDlineEdit_editingFinished()
{
	ui.submitButton->setDisabled(true);
	ui.addButton->setDisabled(true);
	ui.deleteButton->setDisabled(true);
	ui.searchButton->setDisabled(true);
	ui.tableView->setDisabled(true);
	ui.userIDlineEdit->setDisabled(true);
	ui.passwordlineEdit->setDisabled(true);
	ui.agelineEdit->setDisabled(true);
	ui.gendercomboBox->setDisabled(true);
	ui.useremaillineEdit->setDisabled(true);

	ui.saveButton->setVisible(1);
}

void usermanageWindow::on_tableView_currentRowChanged(const QModelIndex& current, const QModelIndex& previous)
{
	QString userid = ui.userIDlineEdit->text();
	QString password = ui.passwordlineEdit->text();
	if (userid.isEmpty() || password.isEmpty())
	{
		qDebug() << "key data empty";
		QMessageBox::critical(this, "����", "�ؼ����ݲ���Ϊ��");
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

void usermanageWindow::on_deleteButton_clicked()
{
	int result = QMessageBox::warning(this, "����", "�⽫ɾ�����˻�����ط����¼��ȷ��������", QMessageBox::Yes | QMessageBox::No);
	switch (result)
	{
	case QMessageBox::Yes:
	{
		qDebug() << "Yes";

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

		QString userid = ui.userIDlineEdit->text();
		QSqlQuery query(db);
		db.exec("SET NAMES 'utf8'");
		query.exec("delete from serv where uid='" + userid + "';");
		qDebug() << "delete from serv where uid='" + userid + "';";

		QModelIndex deleindex = theselection->currentIndex();
		model->removeRow(deleindex.row());
		model->submitAll();
		if (model->lastError().isValid())
		{
			qDebug() << model->lastError();
			QMessageBox::critical(this, "����", "���ݿ����Ӵ���������ǰ���ڿɽ��������");
		}
		else
			QMessageBox::information(this, "�ɹ�", "���˻�����ط����¼�ѳɹ�ɾ��");

		model->select();

		ui.tableView->setFocus();
		theselection->clearSelection();
		theselection->setCurrentIndex(model->index(0, 0), QItemSelectionModel::Select);

		mapper->setCurrentIndex(0);

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

void usermanageWindow::on_submitButton_clicked()
{
	model->submitAll();
	if (model->lastError().isValid())
	{
		qDebug() << model->lastError();
		QMessageBox::critical(this, "����", "���ݿ����Ӵ���������ǰ���ڿɽ��������");
	}
	else
		QMessageBox::information(this, "�ɹ�", "�ѳɹ�����");

	model->select();

	ui.tableView->setFocus();
	theselection->clearSelection();
	theselection->setCurrentIndex(model->index(0, 0), QItemSelectionModel::Select);

	mapper->setCurrentIndex(0);
}

void usermanageWindow::on_revertButton_clicked()
{
	model->revertAll();
	model->select();

	ui.saveButton->setVisible(0);

	ui.submitButton->setEnabled(true);
	ui.addButton->setEnabled(true);
	ui.deleteButton->setEnabled(true);
	ui.searchButton->setEnabled(true);
	ui.tableView->setEnabled(true);
	ui.userIDlineEdit->setEnabled(true);
	ui.passwordlineEdit->setEnabled(true);
	ui.agelineEdit->setEnabled(true);
	ui.gendercomboBox->setEnabled(true);
	ui.useremaillineEdit->setEnabled(true);

	ui.tableView->setFocus();
	theselection->clearSelection();
	theselection->setCurrentIndex(model->index(0, 0), QItemSelectionModel::Select);

	mapper->setCurrentIndex(0);
}

void usermanageWindow::on_saveButton_clicked()
{
	int currrow = ui.tableView->currentIndex().row();

	int result = QMessageBox::warning(this, "����", "�⽫ɾ�����˻���ǰ�ķ����¼��ȷ��������", QMessageBox::Yes | QMessageBox::No);
	switch (result)
	{
	case QMessageBox::Yes:
	{
		qDebug() << "Yes";

		QString userid = ui.userIDlineEdit->text();
		bool T = true;

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

		if (!userid.isEmpty())
		{
			QSqlQuery query(db);
			db.exec("SET NAMES 'utf8'");
			query.exec("select uid from sub;");
			qDebug() << "select uid from sub;";
			while (query.next())
			{
				QString uid = query.value("uid").toString();
				qDebug() << uid;
				if (userid.compare(uid) == 0)
				{
					ui.submitButton->setDisabled(true);
					ui.addButton->setDisabled(true);
					ui.deleteButton->setDisabled(true);
					ui.searchButton->setDisabled(true);

					QMessageBox::critical(this, "����", "�˻�ID�ѱ�ռ��");
					T = false;
				}
			}
		}
		else
		{
			QMessageBox::critical(this, "����", "�˻�ID����Ϊ��");
			T = false;
		}
		if (T == true)
		{
			QSqlQuery query(db);
			db.exec("SET NAMES 'utf8'");
			query.exec("delete from serv where uid='" + temp + "';");
			qDebug() << "delete from serv where uid='" + temp + "';";

			model->submitAll();
			if (model->lastError().isValid())
			{
				qDebug() << model->lastError();
				QMessageBox::critical(this, "����", "���ݿ����Ӵ���������ǰ���ڿɽ��������");
			}
			model->select();
		}
	}
	case QMessageBox::No:
	{
		qDebug() << "NO";

		model->revertAll();
		model->select();

		ui.saveButton->setVisible(0);

		ui.submitButton->setEnabled(true);
		ui.addButton->setEnabled(true);
		ui.deleteButton->setEnabled(true);
		ui.searchButton->setEnabled(true);
		ui.tableView->setEnabled(true);
		ui.userIDlineEdit->setEnabled(true);
		ui.passwordlineEdit->setEnabled(true);
		ui.agelineEdit->setEnabled(true);
		ui.gendercomboBox->setEnabled(true);
		ui.useremaillineEdit->setEnabled(true);

		ui.tableView->setFocus();
		theselection->clearSelection();
		theselection->setCurrentIndex(model->index(currrow, 0), QItemSelectionModel::Select);

		mapper->setCurrentIndex(currrow);

		break;
	}
	}
}

void usermanageWindow::on_helpButton_clicked()
{
	help* hp = new help();
	hp->show();
}