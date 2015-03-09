#ifndef PROJECT_H
#define PROJECT_H

#include "Database/Manager.h"

class Project
{
public:
    bool createNew(const QString& path);
    bool load(const QString& path);
    void close();
    bool save();

    inline bool isOpen() const { return !m_projectPath.isEmpty(); }

    inline QString getPath() const { return m_projectPath; }

    database::Manager& getDatabaseMgr() { return m_Mgr; }

private:
    QString m_projectPath;
    database::Manager m_Mgr;
};
#endif