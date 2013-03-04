#include "IniParser.h"
#include "StringAdditions.h"

bool IniParser::readFile(const std::string &sFileName)
{
    std::ifstream file(sFileName.c_str());

    std::string sDataLine;
    std::string sDataBuffer;
    std::string sSectorBuffer;
    DataMultimap newData;
    while (std::getline(file, sDataLine))
    {
        // delete comments
        uint32 uiSign = sDataLine.find_first_of(';');
        if (uiSign != std::string::npos)
            sDataLine.substr(uiSign);
        uiSign = sDataLine.find_first_of('#');
        if (uiSign != std::string::npos)
            sDataLine.substr(uiSign);
        if (sDataLine.empty())
            continue;

        uint32 uiPos = sDataLine.find_first_of('=');
        if (uiPos != std::string::npos)
        {
            std::string sIndexBuffer = sDataLine.substr(0, uiPos);
            // delete space
            deleteStringFront(" ", sIndexBuffer);
            deleteStringBack(" ", sIndexBuffer);
            // delete tabs
            deleteStringFront("	", sIndexBuffer);
            deleteStringBack("	", sIndexBuffer);

            sDataLine = sDataLine.substr(uiPos+1);
            // delete space
            deleteStringFront(" ", sIndexBuffer);
            deleteStringBack(" ", sIndexBuffer);
            // delete tabs
            deleteStringFront("	", sIndexBuffer);
            deleteStringBack("	", sIndexBuffer);

            newData.insert(std::make_pair(sIndexBuffer, sDataLine));
        }
        else 
        {
            uiPos = sDataLine.find_first_of('[');
            uint32 uiEndPos = sDataLine.find_first_of(']');
            if (uiPos != std::string::npos && uiEndPos != std::string::npos && uiPos+1 < uiEndPos)
            {
                // store sDataLine
                if (!newData.empty())
                {
                    m_FileData.insert(std::make_pair(sSectorBuffer, newData));
                    newData.clear();
                }

                // read out new sector
                sSectorBuffer = sDataLine.substr(uiPos+1);
                sSectorBuffer = sSectorBuffer.substr(0, sSectorBuffer.find_first_of(']'));
                // delete space
                deleteStringFront(" ", sSectorBuffer);
                deleteStringBack(" ", sSectorBuffer);
                // delete tabs
                deleteStringFront("	", sSectorBuffer);
                deleteStringBack("	", sSectorBuffer);
            }
        }
    }
    // store sDataLine
    if (!newData.empty())
    {
        m_FileData.insert(std::make_pair(sSectorBuffer, newData));
        newData.clear();
    }

    file.close();
    return true;
}

std::string IniParser::getString(const std::string &sKey, const std::string &sSector, const std::string sDefault)
{
    SectorData::const_iterator sectorItr = m_FileData.find(sSector);
    if (sectorItr == m_FileData.end())
        return sDefault;
    DataMultimap::const_iterator keyItr = sectorItr->second.find(sKey);
    if (keyItr == sectorItr->second.end())
        return sDefault;
    return keyItr->second;
}

void IniParser::getAllStrings(const std::string &sKey, const std::string &sSector, StdStringVector &data)
{
    SectorData::const_iterator sectorItr = m_FileData.find(sSector);
    if (sectorItr == m_FileData.end())
        return;

    for (DataMultimap::const_iterator keyItr = sectorItr->second.find(sKey); keyItr != sectorItr->second.end() && keyItr->first == sKey; ++keyItr)
        data.push_back(keyItr->second);
}

int IniParser::getInt(const std::string &sKey, const std::string &sSector, const int32 defaultValue)
{
    std::string sResult = getString(sKey, sSector, "");
    return sResult.empty() ? defaultValue : atoi(sResult.c_str());
}

bool IniParser::getBool(const std::string &sKey, const std::string &sSector, const bool defaultValue)
{
    std::string sResult = getString(sKey, sSector, "");
    return sResult.empty() ? defaultValue : atoi(sResult.c_str()) ? true : false;
}

uint32 IniParser::getUInt(const std::string &sKey, const std::string &sSector, const uint32 defaultValue)
{
    std::string sResult = getString(sKey, sSector, "");
    return sResult.empty() ? 0 : atoi(sResult.c_str());
}

void IniParser::addData(const std::string &sKey, const std::string &sSector, const std::string &sData)
{
    if (sKey.empty() || sData.empty())
        return;

    SectorData::iterator itr = m_FileData.find(sSector);
    if (itr != m_FileData.end())
        (*itr).second.insert(std::make_pair(sKey, sData));

    else
    {
        DataMultimap temp;
        temp.insert(std::make_pair(sKey, sData));
        m_FileData.insert(std::make_pair(sSector, temp));
    }
}

void IniParser::saveDataToFile(std::string sFileName)
{
    std::ofstream file(sFileName.c_str(), std::ifstream::out | std::ifstream::trunc);
    for (SectorData::iterator sectorItr = m_FileData.begin(); sectorItr != m_FileData.end(); ++sectorItr)
    {
        file << "[" + sectorItr->first + "]\n";
        for (DataMultimap::iterator keyItr = (*sectorItr).second.begin(); keyItr != (*sectorItr).second.end(); ++keyItr)
            file << keyItr->first + "=" + keyItr->second + "\n";

        file << "\n";
    }

    file.close();
}