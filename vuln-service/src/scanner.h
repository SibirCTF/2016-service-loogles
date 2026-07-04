#ifndef SCANNER_H
#define SCANNER_H
#include <QString>
#include <QVector>
#include <QRegExp>

class Scanner{
	public:
		Scanner();
		void traceroute(int nFirst = 3);
		void scanNetwork(QString ip);
		QString parseIPFromString(QString text);
		bool hasIP(QString text);
		void testsIP();
		void copyAdresses(QVector<QString> &);
		void rescan();
	private:
		QVector<QString> m_vAddresses;
		QString TAG;
		QString m_sPattern;
		QRegExp m_regex;
};

#endif // SCANNER_H
