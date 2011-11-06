#ifndef LOGFILE_H
#define LOGFILE_H

#include <iostream>
#include "Singleton.h"

using namespace std;

const string LOGFILE_FILENAME               = "Game/Logfile.log";
const string LOGFILE_OPENING_MESSAGE        = "Logfile open and ready to write in\n";
const string LOGFILE_CLOSING_MESSAGE        = "Logfile shutting down...";

#define BASIC_LOG m_pLogfile->WriteMessage
#define ERROR_LOG m_pLogfile->WriteErrorMessage

class CLogfile : public TSingleton<CLogfile>
{
public:
    CLogfile();
    ~CLogfile();
    void WriteMessage(string sMessage);
    void WriteErrorMessage(string sMessage);
private:
    FILE *Logfile;
};
#endif