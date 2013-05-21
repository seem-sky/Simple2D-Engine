#ifndef LOGFILE_H
#define LOGFILE_H

#include <fstream>
#include "Singleton.h"

// ToDo: add this in makro __FILE__ and __LINE__
#define BASIC_LOG Logfile::Get()->WriteMessage
#define ERROR_LOG Logfile::Get()->WriteErrorMessage

class Logfile : public TSingleton<Logfile>
{
public:
    Logfile();
    ~Logfile();
    void WriteMessage(QString msg, std::ios_base::openmode mode = std::ios::out | std::ios::app);
    void WriteErrorMessage(QString msg, std::ios_base::openmode mode = std::ios::app);
};
#endif