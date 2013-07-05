#ifndef INI_PARSER_H
#define INI_PARSER_H

#include "Global.h"
#include <QtCore/QString>

typedef std::multimap<QString, QString> DataMultimap;
typedef std::map<QString, DataMultimap> SectorData;
typedef std::vector<QString> StringVector;

class IniParser
{
private:
    SectorData m_FileData;

public:
    /*#####
    ## ini reader
    #####*/
    // open and read file
    bool readFile(const QString &fileName);

    QString getString(const QString &key, const QString &sector, const QString defaultString = "");
    void getAllStrings(const QString &key, const QString &sector, StringVector &data);

    int32 getInt(const QString &key, const QString &sector, const int32 defaultValue = 0);
    uint32 getUInt(const QString &key, const QString &sector, const uint32 defaultValue = 0);
    bool getBool(const QString &key, const QString &sector, const bool defaultValue = false);

    /*#####
    ## ini writer
    #####*/
    template <typename T>
    void addData(const QString &key, const QString &sector, const T &data) { addData(key, sector, QString::number(data)); };
    void addData(const QString &key, const QString &sector, const QString &sData);
    void saveDataToFile(QString sFileName);
};
#endif