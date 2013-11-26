#ifndef PROJECT_H
#define PROJECT_H

#include "DatabaseMgr.h"

class Project
{
public:
    bool createNew(const QString &path);
    bool load(const QString &path);
    void close();
    bool save();

    inline bool isOpen() const { return !m_projectPath.isEmpty(); }

    inline QString getPath() const { return m_projectPath; }

    DATABASE::DatabaseMgr& getDatabaseMgr() { return m_DBMgr; }

private:
    QString m_projectPath;
    DATABASE::DatabaseMgr m_DBMgr;
};
typedef std::shared_ptr<Project> ProjectPtr;
#endif