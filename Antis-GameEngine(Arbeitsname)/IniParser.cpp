#include "IniParser.h"

bool IniParser::ReadFile(std::string ps_fileName)
{
    std::ifstream t_data(ps_fileName.c_str());
    if(!t_data)
        return false;

    DataStrings tl_FileData;
    std::string ts_dataLine;

    while(std::getline(t_data,ts_dataLine))
        tl_FileData.push_back(ts_dataLine);

    InterpretFile(&tl_FileData);

    return true;
}

void IniParser::InterpretFile(DataStrings *pl_data)
{
    if (!pl_data)
        return;

    std::string ts_indexBuffer;
    std::string ts_dataBuffer;
    std::string ts_sectorBuffer;
    DataMultimap tl_newData;
    for (DataStrings::iterator t_itr = pl_data->begin(); t_itr != pl_data->end(); ++t_itr)
    {
        // delete comments
        if ((*t_itr).find_first_of(";") <= (*t_itr).size())
            (*t_itr) = (*t_itr).substr(0, (*t_itr).find_first_of(";"));

        else if ((*t_itr).find_first_of("#") <= (*t_itr).size())
            (*t_itr) = (*t_itr).substr(0, (*t_itr).find_first_of("#"));

        // continue if empty after delete comments
        if ((*t_itr).empty())
            continue;

        // if data content
        if ((*t_itr).find_first_of("=") <= (*t_itr).size())
        {
            // work with index
            ts_indexBuffer = (*t_itr).substr(0, (*t_itr).find_first_of("="));
            // delete space
            DeleteStringFront(" ", ts_indexBuffer);
            DeleteStringBack(" ", ts_indexBuffer);
            // delete tabs
            DeleteStringFront("	", ts_indexBuffer);
            DeleteStringBack("	", ts_indexBuffer);

            // work with data
            ts_dataBuffer = (*t_itr).substr((*t_itr).find_first_of("=")+1);
            // delete space
            DeleteStringFront(" ", ts_dataBuffer);
            DeleteStringBack(" ", ts_dataBuffer);
            // delete tabs
            DeleteStringFront("	", ts_dataBuffer);
            DeleteStringBack("	", ts_dataBuffer);

            tl_newData.insert(std::make_pair(ts_indexBuffer, ts_dataBuffer));
        }
        // if sector
        else if ((*t_itr).find_first_of("[") <= (*t_itr).size() && (*t_itr).find_first_of("]") <= (*t_itr).size() &&
            (*t_itr).find_first_of("[") < (*t_itr).find_first_of("]"))
        {
            // store data
            if (!tl_newData.empty())
            {
                m_lFileData.insert(std::make_pair(ts_sectorBuffer, tl_newData));
                tl_newData.clear();
            }

            // read out new sector
            ts_sectorBuffer = (*t_itr).substr((*t_itr).find_first_of("[")+1);
            ts_sectorBuffer = ts_sectorBuffer.substr(0, ts_sectorBuffer.find_first_of("]"));
            // delete space
            DeleteStringFront(" ", ts_sectorBuffer);
            DeleteStringBack(" ", ts_sectorBuffer);
            // delete tabs
            DeleteStringFront("	", ts_sectorBuffer);
            DeleteStringBack("	", ts_sectorBuffer);
        }
    }

    // last data store
    if (!tl_newData.empty())
    {
        m_lFileData.insert(std::make_pair(ts_sectorBuffer, tl_newData));
        tl_newData.clear();
    }
}

std::string IniParser::GetString(std::string ps_key, std::string ps_sector, std::string ps_default)
{
    SectorData::iterator t_sectorItr = m_lFileData.find(ps_sector);
    if (t_sectorItr == m_lFileData.end())
        return ps_default;

    DataMultimap::iterator t_keyItr = t_sectorItr->second.find(ps_key);
    if (t_keyItr == t_sectorItr->second.end())
        return ps_default;

    return t_keyItr->second;
}

void IniParser::GetAllStrings(std::string ps_key, std::string ps_sector, DataStrings &pl_data)
{
    SectorData::iterator t_sectorItr = m_lFileData.find(ps_sector);
    if (t_sectorItr == m_lFileData.end())
        return;

    for (DataMultimap::iterator t_keyItr = t_sectorItr->second.find(ps_key); t_keyItr != t_sectorItr->second.end(); ++t_keyItr)
    {
        if (t_keyItr->first == ps_key)
            pl_data.push_back(t_keyItr->second);
    }
}

int IniParser::GetInt(std::string ps_key, std::string ps_sector, int p_default)
{
    std::string ts_result = GetString(ps_key, ps_sector, "");
    return ts_result.empty() ? p_default : atoi(ts_result.c_str());
}

bool IniParser::GetBool(std::string ps_key, std::string ps_sector, bool p_default)
{
    std::string ts_result = GetString(ps_key, ps_sector, "");
    return ts_result.empty() ? p_default : atoi(ts_result.c_str()) ? true : false;
}

uint32 IniParser::GetUInt(std::string ps_key, std::string ps_sector, uint32 p_default)
{
    std::string ts_result = GetString(ps_key, ps_sector, "");
    return ts_result.empty() || atoi(ts_result.c_str()) < 0 ? p_default : atoi(ts_result.c_str());
}

void IniParser::AddData(std::string sKey, std::string sSector, std::string sData)
{
    if (sKey.empty() || sData.empty())
        return;

    SectorData::iterator itr = m_lFileData.find(sSector);
    if (itr != m_lFileData.end())
        (*itr).second.insert(std::make_pair(sKey, sData));

    else
    {
        DataMultimap buffer;
        buffer.insert(std::make_pair(sKey, sData));
        m_lFileData.insert(std::make_pair(sSector, buffer));
    }
}

void IniParser::SaveDataToFile(std::string sFileName)
{
    FILE *GameIni = NULL;
    fopen_s(&GameIni, sFileName.c_str(), "w");
    if(!GameIni)
        return;

    for (SectorData::iterator sectorItr = m_lFileData.begin(); sectorItr != m_lFileData.end(); ++sectorItr)
    {
        fprintf(GameIni, ("[" + sectorItr->first + "]\n").c_str());
        for (DataMultimap::iterator keyItr = (*sectorItr).second.begin(); keyItr != (*sectorItr).second.end(); ++keyItr)
            fprintf(GameIni, (keyItr->first + "=" + keyItr->second + "\n").c_str());

        fprintf(GameIni, "\n");
    }

    fclose(GameIni);
}