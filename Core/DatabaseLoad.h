#ifndef DATABASE_LOAD_H
#define DATABASE_LOAD_H

#include "XML_Reader.h"
#include "Global.h"
#include "Database.h"

class DatabaseLoad : public ActiveObject<DATABASE::Database>, public XML::XML_Reader
{
public:
    DatabaseLoad(std::string &p_sFileName);

protected:
    virtual void Run();
};
#endif