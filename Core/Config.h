#ifndef CONFIG_H
#define CONFIG_H

#include "Singleton.h"
#include "Global.h"

class Config : public TSingleton<Config>
{
public:
    Config(void);

    void loadConfig();
    void saveConfig();

    inline QString getProjectDirectory() const { return m_projectDirectory; }
    inline void setProjectDirectory(const QString &projectDir) { m_projectDirectory = projectDir; saveConfig(); }

    void clear();

    inline void addOpenMap(const uint32 &uiMapID) { m_openMaps.insert(uiMapID); saveConfig(); }
    void removeOpenMap(const uint32 &uiMapID);
    inline const UInt32UnorderedSet getOpenMaps() const { return m_openMaps; }

    inline UInt32Point getMainWindowSize() const { return m_MainWindowSize; }
    inline void setMainWindowSize(UInt32Point size) { m_MainWindowSize = size; saveConfig(); }
    inline UInt32Point getMainWindowPos() const { return m_MainWindowPos; }
    inline void setMainWindowPos(UInt32Point pos) { m_MainWindowPos = pos; saveConfig(); }

    inline static QString getConfigDefaultPath() { return "Config.ini"; }

private:
    QString m_projectDirectory;
    UInt32UnorderedSet m_openMaps;
    UInt32Point m_MainWindowSize;
    UInt32Point m_MainWindowPos;
};
#endif