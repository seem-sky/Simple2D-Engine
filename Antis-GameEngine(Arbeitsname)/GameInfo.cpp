#include "GameInfo.h"

// const strings
const std::string GAME_DATA_GAME_INI =              "Game/Game.ini";
const std::string GAME_DATA_PROGRAM_NAME =          "ProgramName";
const std::string GAME_DATA_DATABASE_LOCATION =     "DatabaseLocation";
const std::string GAME_DATA_SCREEN_WIDTH =          "ScreenWidth";
const std::string GAME_DATA_SCREEN_HEIGHT =         "ScreenHeight";
const std::string GAME_DATA_WINDOWED =              "Windowed?";
const std::string GAME_DATA_TILE_SIZE_X =           "MapTileSizeX";
const std::string GAME_DATA_TILE_SIZE_Y =           "MapTileSizeY";

const std::string INI_SECTOR_ENGINE =               "Engine";
const std::string INI_SECTOR_MAP =                  "Map";

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

    m_sProgramName = newIni.getString(GAME_DATA_PROGRAM_NAME, INI_SECTOR_ENGINE, "Anti´s Game Engine");
    m_sDatabaseLocation = newIni.getString(GAME_DATA_DATABASE_LOCATION, INI_SECTOR_ENGINE, "Game/GameDatabase.gbse");
    m_uiScreenSize.x = newIni.getUINT(GAME_DATA_SCREEN_WIDTH, INI_SECTOR_ENGINE, 320);
    m_uiScreenSize.y = newIni.getUINT(GAME_DATA_SCREEN_HEIGHT, INI_SECTOR_ENGINE, 240);
    m_bWindowed = newIni.getBool(GAME_DATA_WINDOWED, INI_SECTOR_ENGINE, false);

    m_uiMapTileSize.x = newIni.getUINT(GAME_DATA_TILE_SIZE_X, INI_SECTOR_MAP, 32);
    m_uiMapTileSize.y = newIni.getUINT(GAME_DATA_TILE_SIZE_Y, INI_SECTOR_MAP, 32);

    BASIC_LOG(m_sLogLocationName + "Successfully read and store " + GAME_DATA_GAME_INI + ".");
}

void GameInfo::SaveGameInfo()
{
    IniParser newIni;
    newIni.addData(GAME_DATA_PROGRAM_NAME, INI_SECTOR_ENGINE, m_sProgramName);
    newIni.addData(GAME_DATA_DATABASE_LOCATION, INI_SECTOR_ENGINE, m_sDatabaseLocation);
    newIni.addData(GAME_DATA_SCREEN_WIDTH, INI_SECTOR_ENGINE, ToString(m_uiScreenSize.x));
    newIni.addData(GAME_DATA_SCREEN_HEIGHT, INI_SECTOR_ENGINE, ToString(m_uiScreenSize.y));
    newIni.addData(GAME_DATA_WINDOWED, INI_SECTOR_ENGINE, ToString(m_bWindowed));
    newIni.addData(GAME_DATA_TILE_SIZE_X, INI_SECTOR_MAP, ToString(m_uiMapTileSize.x));
    newIni.addData(GAME_DATA_TILE_SIZE_Y, INI_SECTOR_MAP, ToString(m_uiMapTileSize.y));

    newIni.saveDataToFile(GAME_DATA_GAME_INI);
}