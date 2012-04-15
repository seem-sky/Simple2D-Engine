#ifndef GAME_INFO_H
#define GAME_INFO_H

#include "Point.h"
#include "IniParser.h"
#include "Logfile.h"

const std::string GAME_DATA_GAME_INI = "Game/Game.ini";

class GameInfo
{
public:
    GameInfo(void);
    ~GameInfo();
    std::string GetProgramName() { return m_sProgramName; }
    std::string GetDatabaseLocation() { return m_sDatabaseLocation; }
    Point<UINT> GetWindowSize() { return m_uiScreenSize; }
    bool IsWindowed() { return m_bWindowed; }

    Point<UINT> GetMapTileSize() { return m_uiMapTileSize; }

    void SaveGameInfo();

protected:
    void ReadIni();

private:
    std::string m_sLogLocationName;
    // program name
    std::string m_sProgramName;
    // screen resolution
    Point<UINT> m_uiScreenSize;
    // map tile size
    Point<UINT> m_uiMapTileSize;
    // screen windowed?
    bool m_bWindowed;
    // spritefile store location
    std::string m_sDatabaseLocation;
};
#endif