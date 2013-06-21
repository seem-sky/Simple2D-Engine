#include "Logfile.h"
#include <QtCore/QTextStream>

const QString LOGFILE_FILENAME               = "Logfile.log";
const QString LOGFILE_OPENING_MESSAGE        = "Logfile open and ready to write in";
const QString LOGFILE_CLOSING_MESSAGE        = "Logfile shutting down...";

Logfile::Logfile() : TSingleton()
{
    m_logLocationName = LOGFILE_ENGINE_LOG_NAME + "Logfile : ";
    WriteMessage(m_logLocationName + LOGFILE_OPENING_MESSAGE, false);
}

Logfile::~Logfile()
{
    WriteMessage(m_logLocationName + LOGFILE_CLOSING_MESSAGE);
}

void Logfile::WriteMessage(const QString &msg, bool append)
{
    // ToDo: rewrite logfile output
    QFile file(LOGFILE_FILENAME);
    if (file.open(append ? QIODevice::Append : QIODevice::WriteOnly))
    {
        QTextStream out(&file);
        out << msg << "\n";
    }
    /*ps_msg += "\n";
    std::fstream t_file;
    t_file.open(LOGFILE_FILENAME.c_str(), p_mode);
    t_file << ps_msg.c_str();
    t_file.close();*/
}

void Logfile::WriteErrorMessage(const QString &msg)
{
    WriteMessage("ERROR! " + msg);
}