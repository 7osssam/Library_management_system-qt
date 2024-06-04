#include "mainwindow.h"
#include <QApplication>
#include "logger.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	// Install custom message handler
	qInstallMessageHandler(myLogMessageHandler);
	// Install custom message pattern
	qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss,zzz} [%{type}] %{category}: %{message}");

	MainWindow w;
	w.show();
	return a.exec();
}
