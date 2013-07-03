#include "Logfile.h"
#include <QtCore/QTextStream>
#include <QtCore/QTime>

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

void Logfile::WriteMessage(const QString &msg, bool append)
{
    QFile file(LOGFILE_FILENAME);
    if (file.open(append ? QIODevice::Append : QIODevice::WriteOnly))
    {
        QTextStream out(&file);
        QTime curTime = QTime::currentTime();
        out << curTime.hour() << "h " << curTime.minute() << "min " << curTime.second() << "sec " << curTime.msec() << "msec >>> " << msg << "\n";
    }
}

void Logfile::WriteErrorMessage(const QString &msg)
{
    WriteMessage("###ERROR### " + msg);
}