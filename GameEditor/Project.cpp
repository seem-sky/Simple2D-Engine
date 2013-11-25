#include "Project.h"
#include <QtCore/QDir>

bool Project::loadProject(const QString &path)
{
    if (isOpenProject())
        closeCurrentProject();

    if (m_DBMgr.loadDatabase(path))
    {
        m_projectPath = path;
        return true;
    }
    return false;
}

void Project::closeCurrentProject()
{
    m_projectPath.clear();
    m_DBMgr.clear();
}

bool Project::saveCurrentProject()
{
    return m_DBMgr.saveDatabase(m_projectPath);
}

bool Project::createNewProject(const QString &path)
{
    QDir dir(path);
    dir.mkpath(path);
    dir.mkdir("Game");
    dir.mkdir("Maps");
    dir.mkdir("Textures");
    return true;
}