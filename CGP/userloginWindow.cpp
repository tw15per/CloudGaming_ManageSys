#include "userloginWindow.h"
#include "globle.h"
#include "help.h"
#include "userPanel.h"
#include "signupWindow.h"
#include <QtSql/qsqldatabase.h>
#include <QtSql/QSqlQuery>
#include <QtDebug>
#include <QtWidgets/qmessagebox.h>
#pragma execution_character_set("utf-8")

userloginWindow::userloginWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

userloginWindow::~userloginWindow()
{
}

void userloginWindow::on_loginButton_clicked()
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

	QString username = ui.userIDlineEdit->text();
	QString password = ui.passwordlineEdit->text();	
	bool T = false;
	if (!username.isEmpty())
	{
		QSqlQuery query(db);
		db.exec("SET NAMES 'utf8'");
		query.exec("select uid,pass from sub;");
		while (query.next())
		{
			QString user = query.value("uid").toString();
			QString pass = query.value("pass").toString();
			qDebug() << user << pass;
			if (username.compare(user) == 0 && password.compare(pass) == 0)
			{
				curruser = username;
				T = true;
				userPanel* up = new userPanel();
				up->show();
				currwindow = 18;
				db.close();
				this->close();
			}
		}
		if (T == false)
			QMessageBox::critical(this, "´íÎó", "ÕË»§ID»òÃÜÂë´íÎó");
	}
	else
		QMessageBox::critical(this, "´íÎó", "ÕË»§ID²»ÄÜÎª¿Õ");
}

void userloginWindow::on_registerButton_clicked()
{
	signupWindow* su = new signupWindow();
	su->show();
	currwindow = 14;
	this->hide();
	connect(su, &signupWindow::ExitWin, this, &QWidget::show);
}

void userloginWindow::on_helpButton_clicked()
{
	help* hp = new help();
	hp->show();
}