#include "GameInfo.h"

// strings for searching in ini
const string SEARCH_STRING_PROJECT_NAME     = "Project Name =";
const string SEARCH_STRING_SCREEN_WIDTH     = "ScreenWidth =";
const string SEARCH_STRING_SCREEN_HEIGHT    = "ScreenHeight =";
const string SEARCH_STRING_WINDOWED         = "Windowed =";
const string SEARCH_STRING_MAPTILESIZE_X    = "MapTileSizeX =";
const string SEARCH_STRING_MAPTILESIZE_Y    = "MapTileSizeY =";

CGameInfo::CGameInfo(void) : TFileInput()
{
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "CGameInfo : ";
    m_sFileDirectory    = "Game/";
    m_sProgramName      = "";
    m_uiScreenWidth     = NULL;
    m_uiScreenHeight    = NULL;
    m_uiMapTileSize_X   = NULL;
    m_uiMapTileSize_Y   = NULL;
    m_bWindowed         = true;
}

void CGameInfo::InterpretFile()
{
    string sFileString;
    string sSearchString;
    size_t sFind;

    for (unsigned int i = 0; i < m_vData.size(); ++i)
    {
        sFileString = m_vData.at(i);
        // if sFileString is empty, continue with next line
        if (sFileString.empty())
            continue;

        // read out program name
        if ((sFind = sFileString.find(SEARCH_STRING_PROJECT_NAME)) < sFileString.size())
        {
            sSearchString = SEARCH_STRING_PROJECT_NAME;
            sFileString.erase(sFind, sFind + sSearchString.size());
            while(sFileString[0] == 32)
                sFileString.erase(0, 1);

            m_sProgramName = sFileString;
        }
        // read screen width
        else if ((sFind = sFileString.find(SEARCH_STRING_SCREEN_WIDTH)) < sFileString.size())
        {
            sSearchString = SEARCH_STRING_SCREEN_WIDTH;
            sFileString.erase(sFind, sFind + sSearchString.size());
            while(sFileString[0] == 32)
                sFileString.erase(0, 1);

            m_uiScreenWidth = atoi(sFileString.c_str());
        }
        // read screen height
        else if ((sFind = sFileString.find(SEARCH_STRING_SCREEN_HEIGHT)) < sFileString.size())
        {
            sSearchString = SEARCH_STRING_SCREEN_HEIGHT;
            sFileString.erase(sFind, sFind + sSearchString.size());
            while(sFileString[0] == 32)
                sFileString.erase(0, 1);

            m_uiScreenHeight = atoi(sFileString.c_str());
        }
        // windowed?
        else if ((sFind = sFileString.find(SEARCH_STRING_WINDOWED)) < sFileString.size())
        {
            sSearchString = SEARCH_STRING_WINDOWED;
            sFileString.erase(sFind, sFind + sSearchString.size());
            while(sFileString[0] == 32)
                sFileString.erase(0, 1);

            if (atoi(sFileString.c_str()))
                m_bWindowed = true;
            else
                m_bWindowed = false;
        }
        // read out maptile size x
        else if ((sFind = sFileString.find(SEARCH_STRING_MAPTILESIZE_X)) < sFileString.size())
        {
            sSearchString = SEARCH_STRING_MAPTILESIZE_X;
            sFileString.erase(sFind, sFind + sSearchString.size());
            while(sFileString[0] == 32)
                sFileString.erase(0, 1);

            m_uiMapTileSize_X = atoi(sFileString.c_str());
        }
        // read out maptile size y
        else if ((sFind = sFileString.find(SEARCH_STRING_MAPTILESIZE_Y)) < sFileString.size())
        {
            sSearchString = SEARCH_STRING_MAPTILESIZE_Y;
            sFileString.erase(sFind, sFind + sSearchString.size());
            while(sFileString[0] == 32)
                sFileString.erase(0, 1);

            m_uiMapTileSize_Y = atoi(sFileString.c_str());
        }
    }
}

std::vector<std::string>* CGameInfo::GetFileData()
{
    return &m_vData;
}

unsigned int CGameInfo::GetFileDataRows()
{
    return 0;
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

void CGameInfo::GetMapTileSize(unsigned int &XSize, unsigned int &YSize)
{
    XSize = m_uiMapTileSize_X;
    YSize = m_uiMapTileSize_Y;
}
