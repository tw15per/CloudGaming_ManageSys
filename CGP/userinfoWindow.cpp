#include "userinfoWindow.h"
#include "globle.h"
#include "help.h"
#include "userPanel.h"
#include "userloginWindow.h"
#include <QtSql/qsqldatabase.h>
#include <QtSql/QSqlQuery>
#include <QtDebug>
#include <QtWidgets/qmessagebox.h>
#pragma execution_character_set("utf-8")

userinfoWindow::userinfoWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.cancelButton, &QPushButton::clicked, this, &QWidget::close);

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

	QString user, pass, age, gen,email;
	QSqlQuery query(db);
	db.exec("SET NAMES 'utf8'");
	query.exec("select * from sub where uid='" + curruser + "';");
	if (query.first())
	{
		user = query.value("uid").toString();
		pass = query.value("pass").toString();
		age = query.value("age").toString();
		gen = query.value("gender").toString();
		email = query.value("uemail").toString();
		qDebug() << user << pass << age << email;
	}
	ui.userIDlineEdit->setText(user);
	ui.passwordlineEdit->setText(pass);
	ui.confirmpasswordlineEdit->setText(pass);
	ui.gendercomboBox->setCurrentText(gen);
	ui.agelineEdit->setText(age);
	ui.useremaillineEdit->setText(email);
	db.close();
}

userinfoWindow::~userinfoWindow()
{
}

void userinfoWindow::closeEvent(QCloseEvent*)
{
	emit ExitWin();
	currwindow = 18;
}

void userinfoWindow::on_passwordlineEdit_textEdited()
{
	ui.confirmpasswordlineEdit->setEnabled(true);
	ui.confirmpasswordlineEdit->clear();
}

void userinfoWindow::on_applyButton_clicked()
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
	QString pass;
	QString confirmpassword = ui.confirmpasswordlineEdit->text();
	QString agenum = ui.agelineEdit->text();
	QString gender = ui.gendercomboBox->currentText();
	QString uemail = ui.useremaillineEdit->text();	
	bool T = true;
	QSqlQuery query(db);
	db.exec("SET NAMES 'utf8'");
	query.exec("select pass from sub where uid='" + username + "';");
	qDebug() << "select pass from sub where uid='" + username + "';";
	if (query.first())
		pass = query.value("pass").toString();
	qDebug() << "账户ID：" << username << "密码:" << password;
	if (!password.isEmpty() && password.compare(pass) != 0)
	{
		if (password.compare(confirmpassword) != 0)
		{			
			QMessageBox::critical(this, "错误", "密码不一致");
			T = false;			
		}
		else 
		{
			QSqlQuery query(db);
			db.exec("SET NAMES 'utf8'");
			query.exec("update sub set pass = '"+password+"' where uid = '"+username+"';");
			qDebug() << "update sub set pass = '" + password + "' where uid = '" + username + "';";
			QMessageBox::information(this, "成功", "账户密码更改成功");
			ui.confirmpasswordlineEdit->setDisabled(true);
		}
	}
	else if (password.isEmpty())
	{
		QMessageBox::critical(this, "错误", "密码不能为空");
		T = false;
	}

	if (T == true)
	{
		QSqlQuery query(db);
		db.exec("SET NAMES 'utf8'");
		query.exec("update sub set age = " + agenum + ", gender = '" + gender + "', uemail='" + uemail + "' where uid = '" + username + "';");
		qDebug() << "update sub set age = " + agenum + ", gender = '" + gender + "', uemail='" + uemail + "' where uid = '" + username + "';";
		QMessageBox::information(this, "成功", "账户信息更改成功，单击“返回”返回主界面");
		db.close();
	}
}

void userinfoWindow::on_helpButton_clicked()
{
	help* hp = new help();
	hp->show();
}