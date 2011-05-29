#include "GameInfo.h"

CGameInfo::CGameInfo(void) : TFileInput()
{
    m_sFileDirectory = "Game/";
    m_sProgramName  = "";
    m_uiScreenWidth = NULL;
    m_uiScreenHeight= NULL;
    m_bWindowed     = true;
}

void CGameInfo::InterpretFile()
{
    string sFileString;
    string sSearchString;
    size_t sFind;
    unsigned int i = 0;
    while(m_caData[i] && i < SIZE_OF_DATA)
    {
        sFileString = m_caData[i];
        switch(i)
        {
            // Get Project Name
        case 0:
            sSearchString = SEARCH_STRING_PROJECT_NAME;
            sFind = sFileString.find(sSearchString);
            if (sFind != string::npos)
            {
                sFileString.erase(sFind, sFind + sSearchString.size());
                while(sFileString[0] == 32)
                    sFileString.erase(0, 1);
            }
            m_sProgramName = sFileString;
            break;
            // Get Screen Width
        case 1:
            sSearchString = SEARCH_STRING_SCREEN_WIDTH;
            sFind = sFileString.find(sSearchString);
            if (sFind != string::npos)
            {
                sFileString.erase(sFind, sFind + sSearchString.size());
                while(sFileString[0] == 32)
                    sFileString.erase(0, 1);
            }
            m_uiScreenWidth = atoi(sFileString.c_str());
            break;
            // Get Screen Height
        case 2:
            sSearchString = SEARCH_STRING_SCREEN_HEIGHT;
            sFind = sFileString.find(sSearchString);
            if (sFind != string::npos)
            {
                sFileString.erase(sFind, sFind + sSearchString.size());
                while(sFileString[0] == 32)
                    sFileString.erase(0, 1);
            }
            m_uiScreenHeight = atoi(sFileString.c_str());
            break;
            // Is Windowed?
        case 3:
            sSearchString = SEARCH_STRING_WINDOWED;
            sFind = sFileString.find(sSearchString);
            if (sFind != string::npos)
            {
                sFileString.erase(sFind, sFind + sSearchString.size());
                while(sFileString[0] == 32)
                    sFileString.erase(0, 1);
            }
            if (atoi(sFileString.c_str()))
                m_bWindowed = false;
            else
                m_bWindowed = true;
            break;
        }
        i++;
    }
}

char* CGameInfo::GetFileData()
{
    return &m_caData[0][0];
}

unsigned int CGameInfo::GetFileDataRows()
{
    return MAX_DATA_TO_READ;
}

void CGameInfo::GetWindowSize(unsigned int &XSize, unsigned int &YSize)
{
    XSize = m_uiScreenWidth;
    YSize = m_uiScreenHeight;
}

void CGameInfo::CreateIniByDefault()
{
    ERROR_LOG("ENGINE::GAME_INFO : Unable to find " + GAME_DATA_GAME_INI + ". Create it and set values to default.");
    FILE *GameIni = NULL;
    fopen_s(&GameIni, (m_sFileDirectory + GAME_DATA_GAME_INI).c_str(), "w");
    if(GameIni)
        fprintf(GameIni, DEFAULT_INI_DATA.c_str());
    fclose(GameIni);
    m_sProgramName  = "Anti´s GameEngine";
    m_uiScreenWidth = 320;
    m_uiScreenHeight= 240;
    m_bWindowed     = true;
}
