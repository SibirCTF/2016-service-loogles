#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <QtCore>
#include <QFile>
#include <QString>
#include "scanner.h"
#include "log.h"
#include "qhttpserver.hpp"
#include "qhttpserverresponse.hpp"
#include "qhttpserverrequest.hpp"
#include "dbconnection.h"

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);

	QString TAG = "[MAIN]";
	Log::i(TAG, "Started");

	/*Scanner *pScanner = new Scanner();
	
	// pScanner->testsIP();
	
	pScanner->rescan();
	QVector<QString> ips;
	pScanner->copyAdresses(ips);
	for(int i = 0; i < ips.size(); i++){
		Log::i(TAG, ips[i]);
	}*/

	/*pid_t parpid, sid;

	parpid = fork(); // create fork
	if(parpid < 0) {
		exit(1);
	} else if(parpid != 0) {
		exit(0);
	}
	umask(0);// access to work with OS
	sid = setsid();// generate unique index of process
	if(sid < 0) {
		exit(1);
	}
	if((chdir("/")) < 0) {
		// retrun to root of OS
		exit(1);
	}
	close(STDIN_FILENO); // close aceess to stderr and stdout
	close(STDOUT_FILENO);
	close(STDERR_FILENO);*/

	g_pDB = new DBConnection();
	if (!g_pDB->isOpened()) {
		std::cout << "Could not open database\n";
		return -1;
	}
	
	using namespace qhttp::server;
    QHttpServer server(&app);
    server.listen( QHostAddress::Any, 8087, [](QHttpRequest* req, QHttpResponse* res) {
		res->addHeader("Cache-Control", "private,max-age=0");

		QString orig_path = req->url().toString();
		QString path = "";
		if(orig_path.startsWith("/api/v1/")){
			QString api_query = "/api/v1/search?query=";
			if(orig_path.startsWith(api_query)){
				QString query = orig_path.mid(api_query.length(),orig_path.length() - api_query.length());
				
				if(query.length() < 2){
					res->addHeader("Content-Type", "application/json");
					res->setStatusCode(qhttp::ESTATUS_NOT_FOUND);
					QJsonObject response;
					response["status"] = 404;
					response["errorDetail"] = "Short query";
					QJsonDocument doc(response);
					res->end(doc.toJson());
				}
				res->addHeader("Content-Type", "application/json");
				res->setStatusCode(qhttp::ESTATUS_OK);
				QJsonObject response = g_pDB->search(query);
				g_pDB->addRecord("", query, "user_query");
				QJsonDocument doc(response);
				res->end(doc.toJson());
				return;
			}else{
				res->addHeader("Content-Type", "application/json");
				res->setStatusCode(qhttp::ESTATUS_NOT_FOUND);
				QJsonObject response;
				response["status"] = 404;
				response["errorDetail"] = "Loogles not support this path";
				QJsonDocument doc(response);
				res->end(doc.toJson());
			}
			return;
		}else{
			if(orig_path.contains("?")){
				QStringList list = orig_path.split("?");
				path = list[0];
			}else{
				path = orig_path;
			}
			
			if(path == "/"){
				path = "/index.html";
			}
			
			QFile file("./www/" + path);
			
			if(!file.exists()){
				res->addHeader("Content-Type", "text/html");
				res->setStatusCode(qhttp::ESTATUS_NOT_FOUND);
				QString response = "<h1>404 Not found</h1>";
				res->end(response.toUtf8());
				return;
			}else{
				if (!file.open(QIODevice::ReadOnly)) {
					res->addHeader("Content-Type", "text/html");
					res->setStatusCode(qhttp::ESTATUS_FORBIDDEN);
					QString response = "<h1>403 Access deny</h1>";
					res->end(response.toUtf8());
					return;
				}
				QByteArray blob = file.readAll();
				if(path.endsWith(".css")){
					res->addHeader("Content-Type", "text/css");
				} else if(path.endsWith(".png")){
					res->addHeader("Content-Type", "image/png");
				} else if(path.endsWith(".js")){
					res->addHeader("Content-Type", "application/javascript");
				}else{
					res->addHeader("Content-Type", "text/html");
				}
				res->addHeader("Content-Length", QString::number(file.size()).toLatin1());
				res->setStatusCode(qhttp::ESTATUS_OK);
				res->end(blob);
			}
		}
		// automatic memory management for req/res
    });

    if ( !server.isListening() ) {
        qDebug("failed to listen");
        return -1;
    }

    return app.exec();

	// return 0; // adjd::attackDefenceJuryDaemon(cnf);
}

