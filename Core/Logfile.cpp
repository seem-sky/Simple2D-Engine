#include "Logfile.h"

const QString LOGFILE_FILENAME               = "Logfile.log";
const QString LOGFILE_OPENING_MESSAGE        = "Logfile open and ready to write in";
const QString LOGFILE_CLOSING_MESSAGE        = "Logfile shutting down...";

Logfile::Logfile() : TSingleton()
{
    m_logLocationName = LOGFILE_ENGINE_LOG_NAME + "Logfile : ";
    WriteMessage(m_logLocationName + LOGFILE_OPENING_MESSAGE, std::ios::out);
}

Logfile::~Logfile()
{
    WriteMessage(m_logLocationName + LOGFILE_CLOSING_MESSAGE);
}

void Logfile::WriteMessage(QString msg, std::ios_base::openmode mode)
{
    // ToDo: rewrite logfile output
    /*ps_msg += "\n";
    std::fstream t_file;
    t_file.open(LOGFILE_FILENAME.c_str(), p_mode);
    t_file << ps_msg.c_str();
    t_file.close();*/
}

void Logfile::WriteErrorMessage(QString msg, std::ios_base::openmode mode)
{
    msg = "ERROR! " + msg;
    WriteMessage(msg, mode);
}