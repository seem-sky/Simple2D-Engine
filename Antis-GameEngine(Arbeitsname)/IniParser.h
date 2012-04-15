#ifndef INI_PARSER_H
#define INI_PARSER_H

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <windows.h>

typedef std::multimap<std::string, std::string> DataMultimap;
typedef std::map<std::string, DataMultimap> SectorData;
typedef std::vector<std::string> DataStrings;

class IniParser
{
private:
    SectorData ml_fileData;

protected:
    // interpret file data
    virtual void interpretFile(DataStrings *pl_data);

public:
    IniParser() { }
    ~IniParser() { }

    /*#####
    ## ini reader
    #####*/
    // open and read file
    bool readFile(std::string ps_fileName);

    static inline void deleteStringFront(char* ps_sign, std::string &ps_data)
    {
        ps_data = ps_data.substr(ps_data.find_first_not_of(ps_sign));
    }

    static inline void deleteStringBack(char* ps_sign, std::string &ps_data)
    {
        ps_data = ps_data.substr(0, ps_data.find_last_not_of(ps_sign)+1);
    }

    std::string getString(std::string ps_key, std::string ps_sector, std::string ps_default = "");
    void getAllStrings(std::string ps_key, std::string ps_sector, DataStrings &pl_data);

    int getInt(std::string ps_key, std::string ps_sector, int p_default = 0);
    UINT getUINT(std::string ps_key, std::string ps_sector, UINT p_default = 0);
    bool getBool(std::string ps_key, std::string ps_sector, bool p_default = false);

    /*#####
    ## ini writer
    #####*/
    void addData(std::string sKey, std::string sSector, std::string sData);
    void saveDataToFile(std::string sFileName);
};
#endif