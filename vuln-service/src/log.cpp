#include "log.h"
#include <iostream>

void Log::i(QString tag, QString message){
	std::cout << tag.toStdString() << " " << message.toStdString() << "\n";
}

void Log::e(QString tag, QString message){
	std::cerr << tag.toStdString() << " " << message.toStdString() << "\n";
}
