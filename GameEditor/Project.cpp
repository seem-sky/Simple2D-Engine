#include "Project.h"
#include <QtCore/QDir>

Project::Project() : m_pDBMgr(new DATABASE::DatabaseMgr())
{}

bool Project::loadProject(const QString &path)
{
    if (isOpenProject())
        closeCurrentProject();

    if (m_pDBMgr->loadDatabase(path, DATABASE::TILE_SET_DATABASE))
    {
        m_projectPath = path;
        return true;
    }
    return false;
}

void Project::closeCurrentProject()
{
    m_projectPath.clear();
    m_pDBMgr->clear();
}

bool Project::saveCurrentProject()
{
    return m_pDBMgr->saveDatabase(m_projectPath);
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