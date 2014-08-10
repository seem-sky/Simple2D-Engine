#ifndef CONFIG_H
#define CONFIG_H

#include "Singleton.h"
#include <Typedefs.h>
#include <QtCore/QString>
#include <unordered_set>

class Config : public TSingleton<Config>
{
public:
    Config(void);

    void loadConfig();
    void saveConfig();

    inline QString getProjectDirectory() const { return m_projectDirectory; }
    inline void setProjectDirectory(const QString& projectDir) { m_projectDirectory = projectDir; saveConfig(); }

    void clear();

    inline void addOpenMap(const uint32& uiMapID) { m_openMaps.insert(uiMapID); saveConfig(); }
    void removeOpenMap(const uint32& uiMapID);
    inline const std::unordered_set<uint32> getOpenMaps() const { return m_openMaps; }

    inline GEOMETRY::Point<uint32> getMainWindowSize() const { return m_MainWindowSize; }
    inline void setMainWindowSize(GEOMETRY::Point<uint32> size) { m_MainWindowSize = size; saveConfig(); }
    inline GEOMETRY::Point<uint32> getMainWindowPos() const { return m_MainWindowPos; }
    inline void setMainWindowPos(GEOMETRY::Point<uint32> pos) { m_MainWindowPos = pos; saveConfig(); }

    inline static QString getConfigDefaultPath() { return "Config.ini"; }

private:
    QString m_projectDirectory;
    std::unordered_set<uint32> m_openMaps;
    GEOMETRY::Point<uint32> m_MainWindowSize;
    GEOMETRY::Point<uint32> m_MainWindowPos;
};
#endif