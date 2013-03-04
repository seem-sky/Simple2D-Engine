#ifndef CONFIG_H
#define CONFIG_H

#include <Singleton.h>
#include "Global.h"

class Config : public TSingleton<Config>
{
public:
    Config(void);

    void loadConfig();
    void saveConfig();

    inline std::string getProjectDirectory() const { return m_sProjectDirectory; }
    inline void setProjectDirectory(const std::string &p_sProjectDir) { m_sProjectDirectory = p_sProjectDir; saveConfig(); }

    void clear();

    inline void addOpenMap(const uint32 &uiMapID) { m_openMaps.insert(uiMapID); saveConfig(); }
    void removeOpenMap(const uint32 &uiMapID);
    inline const UInt32UnorderedSet getOpenMaps() const { return m_openMaps; }

    inline Point<uint32> getMainWindowSize() const { return m_MainWindowSize; }
    inline void setMainWindowSize(Point<uint32> size) { m_MainWindowSize = size; saveConfig(); }
    inline Point<uint32> getMainWindowPos() const { return m_MainWindowPos; }
    inline void setMainWindowPos(Point<uint32> pos) { m_MainWindowPos = pos; saveConfig(); }

    inline static std::string getConfigDefaultPath() { return "Config.ini"; }

private:
    std::string m_sProjectDirectory;
    UInt32UnorderedSet m_openMaps;
    Point<uint32> m_MainWindowSize;
    Point<uint32> m_MainWindowPos;
};
#endif