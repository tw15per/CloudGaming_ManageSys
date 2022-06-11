#include "signupWindow.h"
#include "globle.h"
#include "help.h"
#include "userloginWindow.h"
#include <QtSql/qsqldatabase.h>
#include <QtSql/QSqlQuery>
#include <QtDebug>
#include <QtWidgets/qmessagebox.h>
#pragma execution_character_set("utf-8")

signupWindow::signupWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.cancelButton, &QPushButton::clicked, this, &QWidget::close);
}

signupWindow::~signupWindow()
{
}

void signupWindow::closeEvent(QCloseEvent*)
{
	emit ExitWin();
	currwindow = 16;
}

void signupWindow::on_signupButton_clicked()
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
	QString confirmpassword = ui.confirmpasswordlineEdit->text();	
	bool T = true;
	qDebug() << "账户ID：" << username << "密码:" << password;
	if (!username.isEmpty())
	{
		QSqlQuery query(db);
		db.exec("SET NAMES 'utf8'");
		query.exec("select uid from sub;");
		while (query.next())
		{
			QString user = query.value("uid").toString();
			qDebug() << user;
			if (username.compare(user) == 0)
			{
				QMessageBox::critical(this, "错误", "账户ID已被占用");
				T = false;
			}
		}
	}
	else
	{
		QMessageBox::critical(this, "错误", "账户ID不能为空");
		T = false;
	}
	if (T == true && !password.isEmpty())
	{
		if (password.compare(confirmpassword) == 0)
		{
			QSqlQuery query(db);
			db.exec("SET NAMES 'utf8'");
			query.exec("insert into sub(uid, pass) values(?, ?);");
			QVariantList idList;
			idList << username;
			QVariantList passList;
			passList << password;
			query.addBindValue(idList);
			query.addBindValue(passList);
			query.execBatch();
			db.close();
			QMessageBox::information(this, "成功", "账户注册成功，单击“返回”返回登录界面");
		}
		else
			QMessageBox::critical(this, "错误", "密码不一致");
	}
	else if (password.isEmpty())
		QMessageBox::critical(this, "错误", "密码不能为空");
}

void signupWindow::on_helpButton_clicked()
{
	help* hp = new help();
	hp->show();
}