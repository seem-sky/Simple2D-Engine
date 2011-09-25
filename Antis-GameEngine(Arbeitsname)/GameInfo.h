#ifndef GAME_INFO_H
#define GAME_INFO_H

#include "FileInput.h"
#include "DefaultIniData.h"

const string GAME_DATA_GAME_INI = "Game.ini";

class CGameInfo : public TFileInput<CGameInfo>
{
public:
    CGameInfo(void);
    string GetProgramName() { return m_sProgramName; }
    void GetWindowSize(unsigned int &XSize, unsigned int &YSize);
    void GetMapTileSize(unsigned int &XSize, unsigned int &YSize);
    bool IsWindowed() { return m_bWindowed; }

    // create Game.ini with default stats
    void CreateIniByDefault();
    void SaveDataToIni();
protected:
    void InterpretFile();
    std::vector<std::string>* GetFileData();
    unsigned int GetFileDataRows();
private:
    // program name
    string m_sProgramName;
    // screen resolution
    unsigned int m_uiScreenWidth;
    unsigned int m_uiScreenHeight;
    // map tile size
    unsigned int m_uiMapTileSize_X;
    unsigned int m_uiMapTileSize_Y;
    // screen windowed?
    bool m_bWindowed;

    std::vector<std::string> m_vData;
};
#endif;
