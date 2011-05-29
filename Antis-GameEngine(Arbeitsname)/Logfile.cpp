#include "Logfile.h"

CLogfile::CLogfile()
{
    Logfile = NULL;
    fopen_s(&Logfile, LOGFILE_FILENAME.c_str(), "w");
    if(Logfile)
        fprintf(Logfile, LOGFILE_OPENING_MESSAGE.c_str());
    fclose(Logfile);
    Logfile = NULL;
}

CLogfile::~CLogfile()
{
    WriteMessage(LOGFILE_CLOSING_MESSAGE);
}

void CLogfile::WriteMessage(string sMessage)
{
    sMessage += "\n";
    fopen_s(&Logfile, LOGFILE_FILENAME.c_str(), "a");
    if(Logfile)
        fprintf(Logfile, sMessage.c_str());
    fclose(Logfile);
    Logfile = NULL;
}