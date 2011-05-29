#ifndef LOGFILE_H
#define LOGFILE_H

#include <string>
#include <iostream>
#include "Singleton.h"

using namespace std;

#define BASIC_LOG m_pLogfile->WriteMessage
#define ERROR_LOG m_pLogfile->WriteMessage

const string LOGFILE_FILENAME           = "Game/Logfile.log";
const string LOGFILE_OPENING_MESSAGE    = "ENGINE:LOGFILE::Logfile open and ready to write in\n";
const string LOGFILE_CLOSING_MESSAGE    = "ENGINE:LOGFILE::Logfile shutting down...";

class CLogfile : public TSingleton<CLogfile>
{
public:
    CLogfile();
    ~CLogfile();
    void WriteMessage(string sMessage);
private:
    FILE *Logfile;
};
#endif