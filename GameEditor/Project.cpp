#include "Project.h"
#include <QtCore/QDir>

bool Project::load(const QString& path)
{
    if (isOpen())
        close();

    if (m_DBMgr.loadDatabase(path))
    {
        m_projectPath = path;
        return true;
    }
    return false;
}

void Project::close()
{
    m_projectPath.clear();
    m_DBMgr.clear();
}

bool Project::save()
{
    return m_DBMgr.saveDatabase(m_projectPath);
}

bool Project::createNew(const QString& path)
{
    QDir dir(path);
    dir.mkpath(path);
    dir.mkdir("Game");
    dir.mkdir("Maps");
    dir.mkdir("Textures");
    return true;
}