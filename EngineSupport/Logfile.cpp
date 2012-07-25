#include "Logfile.h"

const std::string LOGFILE_FILENAME               = "Logfile.log";
const std::string LOGFILE_OPENING_MESSAGE        = "Logfile open and ready to write in";
const std::string LOGFILE_CLOSING_MESSAGE        = "Logfile shutting down...";

CLogfile::CLogfile() : TSingleton()
{
    m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "Logfile : ";
    WriteMessage(m_sLogLocationName + LOGFILE_OPENING_MESSAGE, std::ios::out);
}

CLogfile::~CLogfile()
{
    WriteMessage(m_sLogLocationName + LOGFILE_CLOSING_MESSAGE);
}

void CLogfile::WriteMessage(std::string ps_msg, std::ios_base::openmode p_mode)
{
    ps_msg += "\n";
    std::fstream t_file;
    t_file.open(LOGFILE_FILENAME.c_str(), p_mode);
    t_file << ps_msg.c_str();
    t_file.close();
}

void CLogfile::WriteErrorMessage(std::string ps_msg, std::ios_base::openmode p_mode)
{
    ps_msg = "ERROR! " + ps_msg;
    WriteMessage(ps_msg, p_mode);
}