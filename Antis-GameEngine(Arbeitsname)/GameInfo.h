#ifndef GAME_INFO_H
#define GAME_INFO_H

#include "FileInput.h"
#include "DefaultIniData.h"

const string GAME_DATA_GAME_INI = "Game.ini";

// strings for searching in ini
const string SEARCH_STRING_PROJECT_NAME     = "Project Name =";
const string SEARCH_STRING_SCREEN_WIDTH     = "ScreenWidth =";
const string SEARCH_STRING_SCREEN_HEIGHT    = "ScreenHeight =";
const string SEARCH_STRING_WINDOWED         = "Windowed =";

// number of data which are stored in ini
const unsigned int MAX_DATA_TO_READ         = 4;

#define SIZE_OF_DATA sizeof(m_caData) / sizeof(char) / READ_DATA_MAX_LETTER_IN_ROW

class CGameInfo : public TFileInput<CGameInfo>
{
public:
    CGameInfo(void);
    string GetProgramName() { return m_sProgramName; }
    void GetWindowSize(unsigned int &XSize, unsigned int &YSize);
    bool IsWindowed() { return m_bWindowed; }
    // create Game.ini with default stats
    void CreateIniByDefault();
    void SaveDataToIni();
protected:
    void InterpretFile();
    char* GetFileData();
    unsigned int GetFileDataRows();
private:
    string m_sProgramName;
    unsigned int m_uiScreenWidth;
    unsigned int m_uiScreenHeight;
    bool m_bWindowed;

    char m_caData[MAX_DATA_TO_READ][READ_DATA_MAX_LETTER_IN_ROW];
};
#endif;
