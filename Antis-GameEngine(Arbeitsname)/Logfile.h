#ifndef LOGFILE_H
#define LOGFILE_H

#include <iostream>
#include "Singleton.h"
#include "StringAdditions.h"

const std::string LOGFILE_FILENAME               = "Game/Logfile.log";
const std::string LOGFILE_OPENING_MESSAGE        = "Logfile open and ready to write in\n";
const std::string LOGFILE_CLOSING_MESSAGE        = "Logfile shutting down...";

#define BASIC_LOG if(CLogfile *pLog = CLogfile::Get()) pLog->WriteMessage
#define ERROR_LOG if(CLogfile *pLog = CLogfile::Get()) pLog->WriteErrorMessage

class CLogfile : public TSingleton<CLogfile>
{
public:
    CLogfile();
    ~CLogfile();
    void WriteMessage(std::string sMessage);
    void WriteErrorMessage(std::string sMessage);
private:
    FILE *Logfile;
};
#endif