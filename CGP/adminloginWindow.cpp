#include "adminloginWindow.h"
#include "globle.h"
#include "help.h"
#include "adminPanel.h"
#include "saPanel.h"
#include <QtSql/qsqldatabase.h>
#include <QtSql/QSqlQuery>
#include <QtDebug>
#include <QtWidgets/qmessagebox.h>
#pragma execution_character_set("utf-8")

adminloginWindow::adminloginWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

adminloginWindow::~adminloginWindow()
{
}

void adminloginWindow::on_loginButton_clicked() 
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

	QString username = ui.adminIDlineEdit->text();
	QString password = ui.passwordlineEdit->text();	
	bool T = false;
	if (!username.isEmpty())
	{
		if (username.compare("sa") == 0)
		{
			QSqlQuery query(db);
			db.exec("SET NAMES 'utf8'");
			query.exec("select pass from adm where aid = 'sa';");
			if (query.first())
			{
				QString pass = query.value("pass").toString();
				if (password.compare(pass) == 0)
				{
					qDebug() << "sa login";
					curruser = username;
					T = true;
					saPanel* sp = new saPanel();
					sp->show();
					currwindow = 12;
					db.close();
					this->close();
				}
			}
		}
		else
		{
			QSqlQuery query(db);
			db.exec("SET NAMES 'utf8'");
			query.exec("select aid, pass from adm;");
			while (query.next())
			{
				QString user = query.value("aid").toString();
				QString pass = query.value("pass").toString();
				qDebug() << user << pass;
				if (username.compare(user) == 0 && password.compare(pass) == 0)
				{
					curruser = username;
					T = true;
					adminPanel* ap = new adminPanel();
					ap->show();
					currwindow = 3;
					db.close();
					this->close();
				}
			}
		}
		if (T == false)
			QMessageBox::critical(this, "错误", "管理员ID或密码错误");
	}
	else
		QMessageBox::critical(this, "错误", "管理员ID不能为空");
}

void adminloginWindow::on_helpButton_clicked()
{
	help* hp = new help();
	hp->show();
}