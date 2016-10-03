#ifndef LOG_H
#define LOG_H
#include <QString>

class Log{
	public:
		static void i(QString Tag, QString message);
		static void e(QString Tag, QString message);
		
	private:
		
};

#endif // LOG_H
