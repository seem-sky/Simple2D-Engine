#include "Config.h"
#include "IniParser.h"

Config::Config(void) : TSingleton()
{
    m_logLocationName = LOGFILE_ENGINE_LOG_NAME + "Config : ";
}

void Config::clear()
{
    m_projectDirectory.clear();
    m_openMaps.clear();
    saveConfig();
}

void Config::removeOpenMap(const uint32 &uiMapID)
{
    UInt32UnorderedSet::iterator itr = m_openMaps.find(uiMapID);
    if (itr != m_openMaps.end())
        m_openMaps.erase(itr);
    saveConfig();
}

void Config::loadConfig()
{
    IniParser parser;
    if (!parser.readFile(getConfigDefaultPath()))
        return;

    m_projectDirectory = parser.getString("Directory", "Project");
    m_MainWindowSize.x = parser.getUInt("MainWindowSizeX", "EditorSettings");
    m_MainWindowSize.y = parser.getUInt("MainWindowSizeY", "EditorSettings");
    m_MainWindowPos.x = parser.getUInt("MainWindowPosX", "EditorSettings");
    m_MainWindowPos.y = parser.getUInt("MainWindowPosY", "EditorSettings");
    StringVector openMaps;
    parser.getAllStrings("Map", "OpenMaps", openMaps);
    for (uint32 i = 0; i < openMaps.size(); ++i)
        m_openMaps.insert(openMaps.at(i).toUInt());
}

void Config::saveConfig()
{
    IniParser parser;
    parser.addData("Directory", "Project", m_projectDirectory);
    parser.addData("MainWindowSizeX", "EditorSettings", m_MainWindowSize.x);
    parser.addData("MainWindowSizeY", "EditorSettings", m_MainWindowSize.y);
    parser.addData("MainWindowPosX", "EditorSettings", m_MainWindowPos.x);
    parser.addData("MainWindowPosY", "EditorSettings", m_MainWindowPos.y);
    for (UInt32UnorderedSet::const_iterator itr = m_openMaps.begin(); itr != m_openMaps.end(); ++itr)
        parser.addData("Map", "OpenMaps", QString::number(*itr));
    parser.saveDataToFile(getConfigDefaultPath());
}