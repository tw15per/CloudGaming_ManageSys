#include "help.h"
#include "globle.h"
#include <QtDebug>
#pragma execution_character_set("utf-8")

help::help(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QImage* img = new QImage;
	img->load("help-48.png");
	ui.picture->setPixmap(QPixmap::fromImage(*img));

	qDebug() << currwindow;

	switch (currwindow)
	{
	case 7://init
	{
		QFile file("init.txt");
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			qDebug() << "fail to open file";
			return;
		}
		qDebug() << "file open";
		QByteArray context = file.readAll();
		ui.plainTextEdit->setPlainText(context);
		file.close();

		break;
	}
	case 16://userlogin
	{
		QFile file("userlogin.txt");
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			qDebug() << "fail to open file";
			return;
		}
		qDebug() << "file open";
		QByteArray context = file.readAll();
		ui.plainTextEdit->setPlainText(context);
		file.close();

		break;
	}
	case 14://signup
	{
		QFile file("signup.txt");
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			qDebug() << "fail to open file";
			return;
		}
		qDebug() << "file open";
		QByteArray context = file.readAll();
		ui.plainTextEdit->setPlainText(context);
		file.close();

		break;
	}
	case 18://userPanel
	{
		QFile file("userPanel.txt");
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			qDebug() << "fail to open file";
			return;
		}
		qDebug() << "file open";
		QByteArray context = file.readAll();
		ui.plainTextEdit->setPlainText(context);
		file.close();

		break;
	}
	case 15://userinfo
	{
		QFile file("userinfo.txt");
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			qDebug() << "fail to open file";
			return;
		}
		qDebug() << "file open";
		QByteArray context = file.readAll();
		ui.plainTextEdit->setPlainText(context);
		file.close();

		break;
	}
	case 4://gamerepo
	{
		QFile file("gamerepo.txt");
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			qDebug() << "fail to open file";
			return;
		}
		qDebug() << "file open";
		QByteArray context = file.readAll();
		ui.plainTextEdit->setPlainText(context);
		file.close();

		break;
	}
	case 10://record
	{
		QFile file("record.txt");
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			qDebug() << "fail to open file";
			return;
		}
		qDebug() << "file open";
		QByteArray context = file.readAll();
		ui.plainTextEdit->setPlainText(context);
		file.close();

		break;
	}
	case 1://adminlogin
	{
		QFile file("adminlogin.txt");
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			qDebug() << "fail to open file";
			return;
		}
		qDebug() << "file open";
		QByteArray context = file.readAll();
		ui.plainTextEdit->setPlainText(context);
		file.close();

		break;
	}
	case 3://adminPanel
	{
		QFile file("adminPanel.txt");
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			qDebug() << "fail to open file";
			return;
		}
		qDebug() << "file open";
		QByteArray context = file.readAll();
		ui.plainTextEdit->setPlainText(context);
		file.close();

		break;
	}
	case 11://repomanage
	{
		QFile file("repomanage.txt");
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			qDebug() << "fail to open file";
			return;
		}
		qDebug() << "file open";
		QByteArray context = file.readAll();
		ui.plainTextEdit->setPlainText(context);
		file.close();

		break;
	}
	case 9://pubmanage
	{
		QFile file("pubmanage.txt");
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			qDebug() << "fail to open file";
			return;
		}
		qDebug() << "file open";
		QByteArray context = file.readAll();
		ui.plainTextEdit->setPlainText(context);
		file.close();

		break;
	}
	case 13://servicerecord
	{
		QFile file("servicerecord.txt");
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			qDebug() << "fail to open file";
			return;
		}
		qDebug() << "file open";
		QByteArray context = file.readAll();
		ui.plainTextEdit->setPlainText(context);
		file.close();

		break;
	}
	case 12://saPanel
	{
		QFile file("saPanel.txt");
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			qDebug() << "fail to open file";
			return;
		}
		qDebug() << "file open";
		QByteArray context = file.readAll();
		ui.plainTextEdit->setPlainText(context);
		file.close();

		break;
	}
	case 17://usermanage
	{
		QFile file("usermanage.txt");
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			qDebug() << "fail to open file";
			return;
		}
		qDebug() << "file open";
		QByteArray context = file.readAll();
		ui.plainTextEdit->setPlainText(context);
		file.close();

		break;
	}
	case 2://adminmanage
	{
		QFile file("adminmanage.txt");
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			qDebug() << "fail to open file";
			return;
		}
		qDebug() << "file open";
		QByteArray context = file.readAll();
		ui.plainTextEdit->setPlainText(context);
		file.close();

		break;
	}
	}
}

help::~help()
{
}
