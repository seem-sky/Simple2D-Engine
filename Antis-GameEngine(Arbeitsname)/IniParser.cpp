#include "IniParser.h"

bool IniParser::readFile(std::string ps_fileName)
{
    std::ifstream t_data(ps_fileName.c_str());
    if(!t_data)
        return false;

    DataStrings tl_FileData;
    std::string ts_dataLine;

    while(std::getline(t_data,ts_dataLine))
        tl_FileData.push_back(ts_dataLine);

    interpretFile(&tl_FileData);

    return true;
}

void IniParser::interpretFile(DataStrings *pl_data)
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
            deleteStringFront(" ", ts_indexBuffer);
            deleteStringBack(" ", ts_indexBuffer);
            // delete tabs
            deleteStringFront("	", ts_indexBuffer);
            deleteStringBack("	", ts_indexBuffer);

            // work with data
            ts_dataBuffer = (*t_itr).substr((*t_itr).find_first_of("=")+1);
            // delete space
            deleteStringFront(" ", ts_dataBuffer);
            deleteStringBack(" ", ts_dataBuffer);
            // delete tabs
            deleteStringFront("	", ts_dataBuffer);
            deleteStringBack("	", ts_dataBuffer);

            tl_newData.insert(std::make_pair(ts_indexBuffer, ts_dataBuffer));
        }
        // if sector
        else if ((*t_itr).find_first_of("[") <= (*t_itr).size() && (*t_itr).find_first_of("]") <= (*t_itr).size() &&
            (*t_itr).find_first_of("[") < (*t_itr).find_first_of("]"))
        {
            // store data
            if (!tl_newData.empty())
            {
                ml_fileData.insert(std::make_pair(ts_sectorBuffer, tl_newData));
                tl_newData.clear();
            }

            // read out new sector
            ts_sectorBuffer = (*t_itr).substr((*t_itr).find_first_of("[")+1);
            ts_sectorBuffer = ts_sectorBuffer.substr(0, ts_sectorBuffer.find_first_of("]"));
            // delete space
            deleteStringFront(" ", ts_sectorBuffer);
            deleteStringBack(" ", ts_sectorBuffer);
            // delete tabs
            deleteStringFront("	", ts_sectorBuffer);
            deleteStringBack("	", ts_sectorBuffer);
        }
    }

    // last data store
    if (!tl_newData.empty())
    {
        ml_fileData.insert(std::make_pair(ts_sectorBuffer, tl_newData));
        tl_newData.clear();
    }
}

std::string IniParser::getString(std::string ps_key, std::string ps_sector, std::string ps_default)
{
    SectorData::iterator t_sectorItr = ml_fileData.find(ps_sector);
    if (t_sectorItr == ml_fileData.end())
        return ps_default;

    DataMultimap::iterator t_keyItr = t_sectorItr->second.find(ps_key);
    if (t_keyItr == t_sectorItr->second.end())
        return ps_default;

    return t_keyItr->second;
}

void IniParser::getAllStrings(std::string ps_key, std::string ps_sector, DataStrings &pl_data)
{
    SectorData::iterator t_sectorItr = ml_fileData.find(ps_sector);
    if (t_sectorItr == ml_fileData.end())
        return;

    for (DataMultimap::iterator t_keyItr = t_sectorItr->second.find(ps_key); t_keyItr != t_sectorItr->second.end(); ++t_keyItr)
    {
        if (t_keyItr->first == ps_key)
            pl_data.push_back(t_keyItr->second);
    }
}

int IniParser::getInt(std::string ps_key, std::string ps_sector, int p_default)
{
    std::string ts_result = getString(ps_key, ps_sector, "");
    return ts_result.empty() ? p_default : atoi(ts_result.c_str());
}

bool IniParser::getBool(std::string ps_key, std::string ps_sector, bool p_default)
{
    std::string ts_result = getString(ps_key, ps_sector, "");
    return ts_result.empty() ? p_default : atoi(ts_result.c_str()) ? true : false;
}

UINT IniParser::getUINT(std::string ps_key, std::string ps_sector, UINT p_default)
{
    std::string ts_result = getString(ps_key, ps_sector, "");
    return ts_result.empty() || atoi(ts_result.c_str()) < 0 ? p_default : atoi(ts_result.c_str());
}

void IniParser::addData(std::string sKey, std::string sSector, std::string sData)
{
    if (sKey.empty() || sData.empty())
        return;

    SectorData::iterator itr = ml_fileData.find(sSector);
    if (itr != ml_fileData.end())
        (*itr).second.insert(std::make_pair(sKey, sData));

    else
    {
        DataMultimap buffer;
        buffer.insert(std::make_pair(sKey, sData));
        ml_fileData.insert(std::make_pair(sSector, buffer));
    }
}

void IniParser::saveDataToFile(std::string sFileName)
{
    FILE *GameIni = NULL;
    fopen_s(&GameIni, sFileName.c_str(), "w");
    if(!GameIni)
        return;

    for (SectorData::iterator sectorItr = ml_fileData.begin(); sectorItr != ml_fileData.end(); ++sectorItr)
    {
        fprintf(GameIni, ("[" + sectorItr->first + "]\n").c_str());
        for (DataMultimap::iterator keyItr = (*sectorItr).second.begin(); keyItr != (*sectorItr).second.end(); ++keyItr)
            fprintf(GameIni, (keyItr->first + "=" + keyItr->second + "\n").c_str());

        fprintf(GameIni, "\n");
    }

    fclose(GameIni);
}