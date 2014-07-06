#include "Config.h"
#include "IniParser.h"

Config::Config(void) : TSingleton()
{}

void Config::clear()
{
    m_projectDirectory.clear();
    m_openMaps.clear();
    saveConfig();
}

void Config::removeOpenMap(const uint32& uiMapID)
{
    auto itr = m_openMaps.find(uiMapID);
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
    m_MainWindowSize.getX() = parser.getUInt("MainWindowSizeX", "EditorSettings");
    m_MainWindowSize.getY() = parser.getUInt("MainWindowSizeY", "EditorSettings");
    m_MainWindowPos.getX() = parser.getUInt("MainWindowPosX", "EditorSettings");
    m_MainWindowPos.getY() = parser.getUInt("MainWindowPosY", "EditorSettings");
    QStringVector openMaps;
    parser.getAllStrings("Map", "OpenMaps", openMaps);
    for (uint32 i = 0; i < openMaps.size(); ++i)
        m_openMaps.insert(openMaps.at(i).toUInt());
}

void Config::saveConfig()
{
    IniParser parser;
    parser.addData("Directory", "Project", m_projectDirectory);
    parser.addData("MainWindowSizeX", "EditorSettings", m_MainWindowSize.getX());
    parser.addData("MainWindowSizeY", "EditorSettings", m_MainWindowSize.getY());
    parser.addData("MainWindowPosX", "EditorSettings", m_MainWindowPos.getX());
    parser.addData("MainWindowPosY", "EditorSettings", m_MainWindowPos.getY());
    for (auto itr = m_openMaps.begin(); itr != m_openMaps.end(); ++itr)
        parser.addData("Map", "OpenMaps", QString::number(*itr));
    parser.saveDataToFile(getConfigDefaultPath());
}