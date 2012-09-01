#ifndef GAME_INFO_H
#define GAME_INFO_H

#include "Point.h"
#include "IniParser.h"
#include "Logfile.h"

class GameInfo
{
public:
    GameInfo(void);
    ~GameInfo();
    std::string GetProgramName() { return m_sProgramName; }
    std::string GetDatabaseLocation() { return m_sDatabaseLocation; }
    Point<uint32> GetWindowSize() { return m_uiScreenSize; }
    bool IsWindowed() { return m_bWindowed; }

    Point<uint32> GetMapTileSize() { return m_uiMapTileSize; }

    void SaveGameInfo();

protected:
    void ReadIni();

private:
    std::string m_sLogLocationName;
    // program name
    std::string m_sProgramName;
    // screen resolution
    Point<uint32> m_uiScreenSize;
    // map tile size
    Point<uint32> m_uiMapTileSize;
    // screen windowed?
    bool m_bWindowed;
    // spritefile store location
    std::string m_sDatabaseLocation;
};
#endif