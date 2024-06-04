#ifndef LOGGER_H
#define LOGGER_H

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QLoggingCategory>
#include <QApplication>

void myLogMessageHandler(const QtMsgType type, const QMessageLogContext& context, const QString& msg);

#endif // LOGGER_H
