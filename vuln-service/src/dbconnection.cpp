#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <iostream>
#include <QMap>
#include <QtSql/QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include "dbconnection.h"

DBConnection::DBConnection() {
	m_pDB = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
	m_pDB->setDatabaseName("loogles.sqlite");
	m_bOpened = m_pDB->open();
	
	if(m_bOpened){
		if (!m_pDB->tables().contains("records")) {
			std::cout << "Table 'records' did not exists\n";
			QSqlQuery query(*m_pDB);
			query.exec(""
				" CREATE TABLE records ( "
				"	id INTEGER PRIMARY KEY, "
				"	sshort TEXT, "
				"	record TEXT, "
				"	recordtype TEXT, "
				"	recordtime DATETIME"
				" )"
			);
		}
	}
	
	addRecord("hello", "hello", "hello");
};

// --------------------------------------------------------------------

bool DBConnection::isOpened(){
	return m_bOpened;
}

// --------------------------------------------------------------------

QJsonObject DBConnection::search(QString text){
		QJsonObject result;
		QJsonArray objects;
		QJsonObject meta;
		meta["count"] = 0;
		meta["offset"] = 0;
		meta["limit"] = 20;
		if(!m_bOpened){
			result["meta"] = meta;
			result["objects"] = objects;
			return result;
		}

		// calculate count
		{
			QSqlQuery query(*m_pDB);
			query.prepare("SELECT COUNT(*) FROM records WHERE record LIKE :text");
			query.bindValue(":text", "%" + text + "%");
			query.exec();
			query.first();
			meta["count"] = query.value(0).toInt();
		}
		QSqlQuery query(*m_pDB);
		query.prepare("SELECT * FROM records WHERE record LIKE :text ORDER BY id DESC LIMIT 20");
		query.bindValue(":text", "%" + text + "%");
		query.exec();
		QSqlRecord rec = query.record();
		int index_sshort = rec.indexOf("sshort");
		int index_record = rec.indexOf("record");
		int index_recordtype = rec.indexOf("recordtype");
		int index_recordtime = rec.indexOf("recordtime");
		while (query.next()) {
			QJsonObject record;
			record["sshort"] = query.value(index_sshort).toString();
			record["record"] = query.value(index_record).toString();
			record["recordtype"] = query.value(index_recordtype).toString();
			record["recordtime"] = query.value(index_recordtime).toString();
			objects.push_back(record);
		}
		result["meta"] = meta;
		result["objects"] = objects;
		return result;
}

void DBConnection::addRecord(QString sshort, QString record, QString recordtype){
	if(!m_bOpened){ return; }
	QSqlQuery query(*m_pDB);
	query.prepare("INSERT INTO records (sshort,record,recordtype,recordtime) VALUES (?,?,?, datetime('now'))");
	query.addBindValue(sshort);
	query.addBindValue(record);
	query.addBindValue(recordtype);
	query.exec();
}
