#include "logger.h"

void myLogMessageHandler(const QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
	QFile logFile("app.log");
	if (logFile.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		logFile.write(qUtf8Printable(qFormatLogMessage(type, context, msg) + "\n"));
	}
}