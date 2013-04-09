#ifndef LOGFILE_H
#define LOGFILE_H

#include <fstream>
#include "Singleton.h"
#include "StringAdditions.h"

// ToDo: add this in makro __FILE__ and __LINE__
#define BASIC_LOG Logfile::Get()->WriteMessage
#define ERROR_LOG Logfile::Get()->WriteErrorMessage

class Logfile : public TSingleton<Logfile>
{
public:
    Logfile();
    ~Logfile();
    void WriteMessage(std::string ps_msg, std::ios_base::openmode p_mode = std::ios::out | std::ios::app);
    void WriteErrorMessage(std::string ps_msg, std::ios_base::openmode p_mode = std::ios::app);
};
#endif