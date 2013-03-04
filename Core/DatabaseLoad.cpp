#include "DatabaseLoad.h"
#include "XML_Reader.h"
#include "Database.h"
#include <fstream>

using namespace DATABASE;
using namespace XML;

DatabaseLoad::DatabaseLoad(std::string &p_sFileName) : XML_Reader(p_sFileName), ActiveObject(Database::Get()) {}

void DatabaseLoad::Run()
{
    SetThreadState(THREAD_IN_PROGRESS);
    CoObject t_CoObj;
    if (!t_CoObj.InitCo() || !ReadFile())
    {
        SetThreadState(THREAD_FAILED);
        return;
    }

    SetThreadState(THREAD_DONE);
}