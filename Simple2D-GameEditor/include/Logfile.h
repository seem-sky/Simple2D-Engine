#ifndef LOGFILE_H
#define LOGFILE_H

#include "Singleton.h"
#include <QtCore/QFile>

// ToDo: add this in makro __FILE__ and __LINE__
#define BASIC_LOG Logfile::get()->WriteMessage
#define ERROR_LOG Logfile::get()->WriteErrorMessage

class Logfile : public TSingleton<Logfile>
{
public:
    Logfile();
    ~Logfile();
    void WriteMessage(const QString& msg, bool append = true);
    void WriteErrorMessage(const QString& msg);
};
#endif