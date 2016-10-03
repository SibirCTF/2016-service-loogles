#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <QObject>
#include <QMap>
#include <QJsonObject>
#include <QSettings>
#include <QtSql/QSqlDatabase>

class DBConnection
{
	public:
		DBConnection();
		bool isOpened();
		QJsonObject search(QString text);
		void addRecord(QString sshort, QString record, QString recordtype);
   
    private:
		QSqlDatabase *m_pDB;
		bool m_bOpened;
};

static DBConnection *g_pDB;

#endif // DBCONNECTION_H
