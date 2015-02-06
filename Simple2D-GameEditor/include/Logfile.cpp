#include "Logfile.h"
#include <QtCore/QTextStream>
#include <QtCore/QTime>
#include <QtCore/QDebug>

const QString LOGFILE_FILENAME               = "Logfile.log";
const QString LOGFILE_OPENING_MESSAGE        = "Logfile open and ready to write in.";
const QString LOGFILE_CLOSING_MESSAGE        = "Logfile shutting down...";

Logfile::Logfile() : TSingleton()
{
    WriteMessage(LOGFILE_OPENING_MESSAGE, false);
}

Logfile::~Logfile()
{
    WriteMessage(LOGFILE_CLOSING_MESSAGE);
}

void Logfile::WriteMessage(const QString& msg, bool append)
{
    QFile file(LOGFILE_FILENAME);
    if (file.open(append ? QIODevice::Append : QIODevice::WriteOnly))
    {
        QTextStream out(&file);
        QTime curTime = QTime::currentTime();
        QString finalMsg = QString::number(curTime.hour()) + "h " + QString::number(curTime.minute()) + "min " + QString::number(curTime.second()) + "sec " +
            QString::number(curTime.msec()) + "msec >>> " + msg + " <<<";
        out << finalMsg + "\n";
        qDebug() << finalMsg;
    }
}

void Logfile::WriteErrorMessage(const QString& msg)
{
    WriteMessage("### ERROR ### " + msg);
}

void Logfile::WriteWarningMessage(const QString& msg)
{
    WriteMessage("### Warning ### " + msg);
}