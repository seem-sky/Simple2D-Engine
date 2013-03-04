#ifndef INI_PARSER_H
#define INI_PARSER_H

#include <fstream>
#include "Global.h"

typedef std::multimap<std::string, std::string> DataMultimap;
typedef std::map<std::string, DataMultimap> SectorData;

class IniParser
{
private:
    SectorData m_FileData;

public:
    /*#####
    ## ini reader
    #####*/
    // open and read file
    bool readFile(const std::string &sFileName);

    std::string getString(const std::string &sKey, const std::string &sSector, const std::string sDefault = "");
    void getAllStrings(const std::string &sKey, const std::string &sSector, StdStringVector &data);

    int32 getInt(const std::string &sKey, const std::string &sSector, const int32 defaultValue = 0);
    uint32 getUInt(const std::string &sKey, const std::string &sSector, const uint32 defaultValue = 0);
    bool getBool(const std::string &sKey, const std::string &sSector, const bool defaultValue = false);

    /*#####
    ## ini writer
    #####*/
    template <typename T>
    void addData(const std::string &sKey, const std::string &sSector, const T &data) { addData(sKey, sSector, ToString(data)); };
    void addData(const std::string &sKey, const std::string &sSector, const std::string &sData);
    void saveDataToFile(std::string sFileName);
};
#endif