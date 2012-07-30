#ifndef LOGFILE_H
#define LOGFILE_H

#include <fstream>
#include "Singleton.h"
#include "StringAdditions.h"

#define BASIC_LOG if(Logfile *pLog = Logfile::Get()) pLog->WriteMessage
#define ERROR_LOG if(Logfile *pLog = Logfile::Get()) pLog->WriteErrorMessage

class Logfile : public TSingleton<Logfile>
{
public:
    Logfile();
    ~Logfile();
    void WriteMessage(std::string ps_msg, std::ios_base::openmode p_mode = std::ios::out | std::ios::app);
    void WriteErrorMessage(std::string ps_msg, std::ios_base::openmode p_mode = std::ios::app);
};
#endif