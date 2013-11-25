#ifndef PROJECT_H
#define PROJECT_H

#include "DatabaseMgr.h"

class Project
{
public:
    bool createNewProject(const QString &path);
    bool loadProject(const QString &path);
    void closeCurrentProject();
    bool saveCurrentProject();

    inline bool isOpenProject() const { return !m_projectPath.isEmpty(); }

    inline QString getProjectPath() const { return m_projectPath; }

    const DATABASE::DatabaseMgr& getDatabaseMgr() const { return m_DBMgr; }

private:
    QString m_projectPath;
    DATABASE::DatabaseMgr m_DBMgr;
};
typedef std::shared_ptr<Project> ProjectPtr;
#endif