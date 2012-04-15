#include "GameInfo.h"

GameInfo::GameInfo(void) : m_bWindowed(false)
{
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "GameInfo : ";

    ReadIni();
}

GameInfo::~GameInfo()
{
    SaveGameInfo();
}

void GameInfo::ReadIni()
{
    IniParser newIni;
    newIni.readFile(GAME_DATA_GAME_INI);

    m_sProgramName = newIni.getString("ProgramName", "Engine", "Anti´s Game Engine");
    m_sDatabaseLocation = newIni.getString("DatabaseLocation", "Engine", "Game/GameDatabase.gbse");
    m_uiScreenSize.x = newIni.getUINT("ScreenWidth", "Engine", 320);
    m_uiScreenSize.y = newIni.getUINT("ScreenHeight", "Engine", 240);
    m_bWindowed = newIni.getBool("Windowed?", "Engine", false);

    m_uiMapTileSize.x = newIni.getUINT("MapTileSizeX", "Map", 32);
    m_uiMapTileSize.y = newIni.getUINT("MapTileSizeY", "Map", 32);

    BASIC_LOG(m_sLogLocationName + "Successfully read and store " + GAME_DATA_GAME_INI + ".");
}

void GameInfo::SaveGameInfo()
{
    IniParser newIni;
    newIni.addData("ProgramName", "Engine", m_sProgramName);
    newIni.addData("DatabaseLocation", "Engine", m_sDatabaseLocation);
    newIni.addData("ScreenWidth", "Engine", to_string(m_uiScreenSize.x));
    newIni.addData("ScreenHeight", "Engine", to_string(m_uiScreenSize.y));
    newIni.addData("Windowed?", "Engine", to_string(m_bWindowed));
    newIni.addData("MapTileSizeX", "Map", to_string(m_uiMapTileSize.x));
    newIni.addData("MapTileSizeY", "Map", to_string(m_uiMapTileSize.y));

    newIni.saveDataToFile(GAME_DATA_GAME_INI);
}