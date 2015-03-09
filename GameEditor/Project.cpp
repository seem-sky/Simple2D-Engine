#include "Project.h"
#include <QtCore/QDir>

bool Project::load(const QString& path)
{
    if (isOpen())
        close();

    m_Mgr.load(path);
    m_projectPath = path;
    return true;
}

void Project::close()
{
    m_projectPath.clear();
}

bool Project::save()
{
    m_Mgr.save(m_projectPath);
    return true;
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