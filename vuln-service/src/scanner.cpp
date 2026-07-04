#include "scanner.h"
#include "log.h"
#include <iostream>
#include <QProcess>
#include <QStringList>

Scanner::Scanner(){
	TAG = "[SCANNER]";
	m_sPattern = ".*(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}).*";
}

bool Scanner::hasIP(QString text){
	QRegExp regex(m_sPattern, Qt::CaseInsensitive, QRegExp::RegExp);
    bool matches = regex.exactMatch(text);
    return matches;
}

QString Scanner::parseIPFromString(QString text){
	QString result = "";
	QRegExp regex(m_sPattern, Qt::CaseInsensitive, QRegExp::RegExp);
	if(regex.indexIn(text) >= 0){
		result = regex.cap(1);
	}
	return result;
}

void Scanner::traceroute(int nFirst){
	Log::i(TAG, "traceroute start");
	QProcess process;
	process.start("traceroute -m 4 8.8.8.8");
	while(!process.waitForFinished()){
		Log::i(TAG, "traceroute wait...");
	}
	m_vAddresses.clear();
	QString output(process.readAllStandardOutput());
	QStringList list = output.split("\n");
	int nCount = 0;
	for(int i = 1; i < list.size(); i++){
		QString s = list[i];
		if(this->hasIP(s)){
			QString sIP = this->parseIPFromString(s);
			if(!m_vAddresses.contains(sIP)){
				m_vAddresses.push_back(sIP);
			}
			if(nCount >= nFirst){
				break;
			}
			nCount++;
		}
	}
}

void Scanner::scanNetwork(QString ip){
	Log::i(TAG, "nmap start by " + ip);
	QProcess process;
	process.start("nmap -sP " + ip + "/24");
	while(!process.waitForFinished()){
		Log::i(TAG, "nmap wait... by " + ip);
	}
	
	QString output(process.readAllStandardOutput());
	QStringList list = output.split("\n");
	for(int i = 0; i < list.size(); i++){
		QString s = list[i];
		if(this->hasIP(s)){
			QString sIP = this->parseIPFromString(s);
			if(!m_vAddresses.contains(sIP)){
				m_vAddresses.push_back(sIP);
			}
		}
	}
}

void Scanner::copyAdresses(QVector<QString> &adresses){
	for(int i = 1; i < m_vAddresses.size(); i++){
		adresses.push_back(m_vAddresses[i]);
	}
}

void Scanner::testsIP(){
	QVector<QString> v;
	v.push_back("Nmap scan report for 213.228.113.131");
	v.push_back("Host is up (0.0021s latency).");
	v.push_back("Nmap scan report for webstream3-nso-huawei5105.71.sinor.ru (213.228.113.71)");
	
	for(int i = 0; i < v.size(); i++){
		QString s = v[i];
		std::cout << s.toStdString() << "";
		if(this->hasIP(s)){
			std::cout << " true [" << this->parseIPFromString(s).toStdString() + "]";
		}else{
			std::cout << " false";
		}
		std::cout << "\n";
	}
}

void Scanner::rescan(){
	m_vAddresses.clear();
	this->traceroute();
	QVector<QString> trase;
	this->copyAdresses(trase);
	for(int i = 0; i < trase.size(); i++){
		this->scanNetwork(trase[i]);
	}
}
