#ifndef INI_PARSER_H
#define INI_PARSER_H

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <windows.h>
#include "Global.h"

typedef std::multimap<std::string, std::string> DataMultimap;
typedef std::map<std::string, DataMultimap> SectorData;
typedef std::vector<std::string> DataStrings;

class IniParser
{
private:
    SectorData m_lFileData;

protected:
    // interpret file data
    virtual void InterpretFile(DataStrings *pl_data);

public:
    IniParser() { }
    ~IniParser() { }

    /*#####
    ## ini reader
    #####*/
    // open and read file
    bool ReadFile(std::string ps_fileName);

    static inline void DeleteStringFront(char* ps_sign, std::string &ps_data)
    {
        ps_data = ps_data.substr(ps_data.find_first_not_of(ps_sign));
    }

    static inline void DeleteStringBack(char* ps_sign, std::string &ps_data)
    {
        ps_data = ps_data.substr(0, ps_data.find_last_not_of(ps_sign)+1);
    }

    std::string GetString(std::string ps_key, std::string ps_sector, std::string ps_default = "");
    void GetAllStrings(std::string ps_key, std::string ps_sector, DataStrings &pl_data);

    int GetInt(std::string ps_key, std::string ps_sector, int p_default = 0);
    uint32 GetUInt(std::string ps_key, std::string ps_sector, uint32 p_default = 0);
    bool GetBool(std::string ps_key, std::string ps_sector, bool p_default = false);

    /*#####
    ## ini writer
    #####*/
    void AddData(std::string sKey, std::string sSector, std::string sData);
    void SaveDataToFile(std::string sFileName);
};
#endif