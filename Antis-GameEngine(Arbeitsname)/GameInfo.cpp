#include "GameInfo.h"

CGameInfo::CGameInfo(void) : TFileInput()
{
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "CGameInfo : ";
    m_sFileDirectory    = "Game/";
    m_sProgramName      = "";
    m_uiScreenWidth     = NULL;
    m_uiScreenHeight    = NULL;
    m_bWindowed         = true;
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
                m_bWindowed = true;
            else
                m_bWindowed = false;
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
    ERROR_LOG( m_sLogLocationName + "Unable to find " + m_sFileDirectory + GAME_DATA_GAME_INI + ". Create it and set values to default.");
    m_sProgramName  = DEFAULT_PROJECT_NAME;
    m_uiScreenWidth = DEFAULT_SCREEN_WIDTH;
    m_uiScreenHeight= DEFAULT_SCREEN_HEIGHT;
    m_bWindowed     = DEFAULT_WINDOWED;
    SaveDataToIni();
}

void CGameInfo::SaveDataToIni()
{
    FILE *GameIni = NULL;
    fopen_s(&GameIni, (m_sFileDirectory + GAME_DATA_GAME_INI).c_str(), "w");
    if(GameIni)
        fprintf(GameIni, (SEARCH_STRING_PROJECT_NAME + m_sProgramName + "\n" + SEARCH_STRING_SCREEN_WIDTH + to_string(m_uiScreenWidth) + 
            "\n" + SEARCH_STRING_SCREEN_HEIGHT + to_string(m_uiScreenHeight) + "\n" + SEARCH_STRING_WINDOWED + to_string(m_bWindowed)).c_str());
    fclose(GameIni);

}
