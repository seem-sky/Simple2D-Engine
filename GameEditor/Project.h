#ifndef PROJECT_H
#define PROJECT_H

#include "DatabaseMgr.h"

class Project
{
public:
    Project();

    bool createNewProject(const QString &path);
    bool loadProject(const QString &path);
    void closeCurrentProject();
    bool saveCurrentProject();

    inline bool isOpenProject() const { return !m_projectPath.isEmpty(); }

    inline DATABASE::DatabaseMgrPtr getDatabaseMgr() { return m_pDBMgr; }
    inline DATABASE::ConstDatabaseMgrPtr getDatabaseMgr() const { return m_pDBMgr; }

    inline QString getProjectPath() const { return m_projectPath; }

private:
    QString m_projectPath;
    DATABASE::DatabaseMgrPtr m_pDBMgr;
};
typedef boost::shared_ptr<Project> ProjectPtr;
#endif