#include "Logfile.h"

CLogfile::CLogfile() : Logfile(NULL), TSingleton()
{
    m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "CLogfile : ";
    fopen_s(&Logfile, LOGFILE_FILENAME.c_str(), "w");
    if(Logfile)
        fprintf(Logfile, (m_sLogLocationName + LOGFILE_OPENING_MESSAGE).c_str());
    fclose(Logfile);
    Logfile = NULL;
}

CLogfile::~CLogfile()
{
    WriteMessage(m_sLogLocationName + LOGFILE_CLOSING_MESSAGE);
}

void CLogfile::WriteMessage(std::string sMessage)
{
    sMessage += "\n";
    fopen_s(&Logfile, LOGFILE_FILENAME.c_str(), "a");
    if(Logfile)
        fprintf(Logfile, sMessage.c_str());
    fclose(Logfile);
    Logfile = NULL;
}

void CLogfile::WriteErrorMessage(std::string sMessage)
{
    sMessage = "ERROR! " + sMessage + "\n";
    fopen_s(&Logfile, LOGFILE_FILENAME.c_str(), "a");
    if(Logfile)
        fprintf(Logfile, sMessage.c_str());
    fclose(Logfile);
    Logfile = NULL;
}